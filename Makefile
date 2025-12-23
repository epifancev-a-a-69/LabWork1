MAIN = bmp
TEST = test
CXX = g++
CXXFLAGS = -lgtest -lgtest_main -pthread -Werror -Wpedantic -Wall -Wextra
SRC_1 = bmp_image.cpp main.cpp
SRC_2 = bmp_image.cpp test.cpp
OBJ_1 = $(SRC_1:.cpp=.o)
OBJ_2 = $(SRC_2:.cpp=.o)

.PHONY: all clean

all: $(MAIN) $(TEST)

$(MAIN): $(OBJ_1)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(TEST): $(OBJ_2)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)


clean:
	rm -f $(OBJ_1) $(MAIN) $(OBJ_2) $(TEST) clockwise.bmp counter_clockwise.bmp gaussian.bmp test_output.bmp
