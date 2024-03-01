CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra

TARGET = main
SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)
