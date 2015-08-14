CFLAGS  := $(shell pkg-config --cflags opencv)
CFLAGS  += -pthread

LDFLAGS := $(shell pkg-config --libs opencv)
LDFLAGS += -lboost_filesystem 
LDFLAGS += -lboost_system
LDFLAGS += -lpthread

all:
	g++ -std=c++11 $(CFLAGS) capture_face.cpp $(LDFLAGS) -o capture.bin 
	g++ -std=c++11 $(CFLAGS) cut_face.cpp $(LDFLAGS) -o cut.bin 
	g++ -std=c++11 $(CFLAGS) picture_face.cpp $(LDFLAGS) -o picture.face.bin 
clean:
	rm -rf *.bin *.avi *.png rm -rf output

