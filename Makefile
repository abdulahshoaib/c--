CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
CPPFLAGS += -Iinclude

BUILD := build
BIN   := $(BUILD)/lexer

LIB_SRCS := src/token.cpp src/lexer.cpp
LIB_OBJS := $(LIB_SRCS:src/%.cpp=$(BUILD)/%.o)
MAIN_OBJS := $(BUILD)/main.o
DEPS := $(LIB_OBJS:.o=.d) $(MAIN_OBJS:.o=.d)

.PHONY: all clean

all: $(BIN)

$(BUILD)/%.o: src/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

$(BIN): $(MAIN_OBJS) $(LIB_OBJS)
	$(CXX) $^ -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)

-include $(DEPS)
