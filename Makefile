CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
INCLUDES = -lncurses

TARGET = program

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(INCLUDES)


%.o : %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)