CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
CPPFLAGS += -Iinclude

BUILD := build
BIN   := $(BUILD)/lexer

LIB_SRCS := src/token.cpp src/lexer.cpp
LIB_OBJS := $(LIB_SRCS:src/%.cpp=$(BUILD)/%.o)

MAIN_OBJS := $(BUILD)/main.o

UNIT_BINS := $(BUILD)/test_lexer
UNIT_OBJS := $(UNIT_BINS:=.o)

DEPS := $(LIB_OBJS:.o=.d) $(MAIN_OBJS:.o=.d) $(UNIT_OBJS:.o=.d)

.PHONY: all test unit clean
.SECONDARY:

all: $(BIN)

$(BUILD)/test_%.o: tests/unit/test_%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -Itests/unit -MMD -MP -c $< -o $@

$(BUILD)/%.o: src/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

$(BIN): $(MAIN_OBJS) $(LIB_OBJS)
	$(CXX) $^ -o $@

$(BUILD)/test_%: $(BUILD)/test_%.o $(LIB_OBJS)
	$(CXX) $^ -o $@

$(BUILD):
	mkdir -p $(BUILD)

unit: $(UNIT_BINS)
	@set -e; for t in $(UNIT_BINS); do echo "== $$t"; ./$$t; done

test: unit

clean:
	rm -rf $(BUILD)

-include $(DEPS)
