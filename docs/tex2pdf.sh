#!/usr/bin/env bash

set -euo pipefail

if [[ $# -lt 1 ]]; then
    echo "Usage: $0 <file.tex>"
    exit 1
fi

INPUT="$1"

if [[ ! -f "$INPUT" ]]; then
    echo "Error: File not found: $INPUT"
    exit 1
fi

if ! command -v pdflatex &>/dev/null; then
    echo "Error: pdflatex is not installed."
    exit 1
fi

# Build inside the document's directory so relative \input / \include
# and image paths resolve the way the author wrote them.
DIR="$(cd "$(dirname "$INPUT")" && pwd)"
BASENAME="$(basename "$INPUT" .tex)"

# Run pdflatex from the source directory, but write every auxiliary file
# (.aux, .log, .toc, ...) into a throwaway directory so the only artifact
# left next to the source is the PDF.
BUILD_DIR="$(mktemp -d)"
trap 'rm -rf "$BUILD_DIR"' EXIT

cd "$DIR"

# Two passes: the first writes the .aux, the second resolves the table of
# contents and hyperref cross-references.
pdflatex \
    -interaction=nonstopmode \
    -halt-on-error \
    -file-line-error \
    -output-directory="$BUILD_DIR" \
    "$BASENAME.tex"

pdflatex \
    -interaction=nonstopmode \
    -halt-on-error \
    -file-line-error \
    -output-directory="$BUILD_DIR" \
    "$BASENAME.tex"

if [[ ! -f "$BUILD_DIR/$BASENAME.pdf" ]]; then
    echo "Error: pdflatex ran but produced no PDF." >&2
    exit 1
fi

cp "$BUILD_DIR/$BASENAME.pdf" "$DIR/$BASENAME.pdf"

echo "Generated: $DIR/$BASENAME.pdf"
