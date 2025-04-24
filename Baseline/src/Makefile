CXX=g++ -O3
CXXFLAGS=-std=c++11
LDFLAGS = -I/home/users/pmiliad/source/build/install/include/opencv4/
LDFLAGS += -I.
LINK = -L$(OPENCV_LIB) -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui
TARGET = baseline

SOURCES := $(shell find . -name '*.cpp')
HEADERS := $(shell find . -name '*.h')

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(SOURCES) $(LINK)


clean:
	rm -f $(TARGET) *.o *~
