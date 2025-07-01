CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Itests

SRC = $(wildcard src2/*.cpp)
TESTS = $(wildcard tests/test_*.cpp)
OBJ_DIR = build

OBJS = $(SRC:src/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJS = $(TESTS:tests/%.cpp=$(OBJ_DIR)/tests/%.o)

TARGET = $(OBJ_DIR)/main
TEST_TARGET = $(OBJ_DIR)/test_runner

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

MAIN_OBJ = $(OBJ_DIR)/main.o
OTHER_OBJS = $(filter-out $(MAIN_OBJ), $(OBJS))

$(TEST_TARGET): $(TEST_OBJS) $(OTHER_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/tests/%.o: tests/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

.PHONY: all test clean