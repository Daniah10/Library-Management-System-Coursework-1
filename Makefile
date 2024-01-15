CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCS = library_management.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = library_management

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
