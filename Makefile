CFLAGS  := $(shell pkg-config --cflags opencv)

LDFLAGS := $(shell pkg-config --libs opencv)
LDFLAGS += -lboost_filesystem 
LDFLAGS += -lboost_system

all:
	g++ -std=c++11 $(CFLAGS) capture_face.cpp $(LDFLAGS) -o capture.bin 
	g++ -std=c++11 $(CFLAGS) cut_face.cpp $(LDFLAGS) -o cut.bin 
	g++ -std=c++11 $(CFLAGS) picture_face.cpp $(LDFLAGS) -o picture.face.bin 
	g++ -std=c++11 $(CFLAGS) iteration.cpp $(LDFLAGS) -o iteration.bin 
clean:
	rm -rf *.bin *.avi *.png rm -rf output

