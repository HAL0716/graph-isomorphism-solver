CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -g

SRCS = $(shell find src -name '*.cpp')
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))
TARGET = build/main

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean