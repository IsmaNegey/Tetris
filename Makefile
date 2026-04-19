CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
INCLUDES = -lncurses

TARGET = build/program

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)

all : build $(TARGET)

build:
	mkdir -p build

$(TARGET) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(INCLUDES)


build/%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)