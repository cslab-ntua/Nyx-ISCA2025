CXX=g++
CXXFLAGS=-std=c++11
LDFLAGS = -I.
TARGET = clean_nyx

SOURCES := $(shell find . -name '*.cpp')
HEADERS := $(shell find . -name '*.h')

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(SOURCES)


clean:
	rm -f $(TARGET) *~
