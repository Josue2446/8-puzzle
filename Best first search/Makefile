CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Wpedantic -Wshadow -g \
						-Wnull-dereference -Wold-style-definition -Wno-unused-parameter \
						-Wunreachable-code -Werror -Wdeclaration-after-statement \
						-Wno-return-local-addr -Wuninitialized

TARGET := main
SRCS := main.cpp
OBJS := $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

rebuild: clean all
