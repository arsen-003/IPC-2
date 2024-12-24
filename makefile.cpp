CXX: = g++
CXXFLAGS : = -std = c++11
TARGET : = prime - calculator

all : $(TARGET)

$(TARGET) : prime_calculator.cpp
$(CXX) $(CXXFLAGS) - o $(TARGET) prime_calculator.cpp

clean :
rm - f $(TARGET)

