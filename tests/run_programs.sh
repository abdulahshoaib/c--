#!/usr/bin/env sh
# Run the parser over the sample programs and diff each result against its
# expected output. Usage: run_programs.sh <parser-binary>
set -u

BIN="${1:?usage: run_programs.sh <parser-binary>}"
ROOT="$(dirname "$0")"
status=0

for cmm in "$ROOT"/programs/*.cmm; do
  name="$(basename "$cmm")"
  expected_file="${cmm%.cmm}.expected"

  if [ ! -f "$expected_file" ]; then
    echo "FAIL $name (missing $(basename "$expected_file"))"
    status=1
    continue
  fi

  actual="$("$BIN" "$cmm" 2>&1)"
  expected="$(cat "$expected_file")"

  if [ "$actual" = "$expected" ]; then
    echo "PASS $name"
  else
    echo "FAIL $name"
    echo "  expected: $expected"
    echo "  actual:   $actual"
    status=1
  fi
done

exit "$status"
