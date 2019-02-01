TARGET=splash_screen
CC=g++
DEBUG=-g
OPT=-O0
WARN=-Wall
PTHREAD=-pthread
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe -fpermissive -fPIC
GTKLIB=`pkg-config --cflags --libs gtk+-3.0`
LD=g++
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic -fPIC
OBJS=splash_screen.o spectraread.o
all: $(OBJS)
#	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
	$(LD) -o peakss splash_screen.o $(LDFLAGS)
	$(LD) -o peaks peaks.o $(LDFLAGS)
splash_screen.o: splash_screen.c
	$(CC) -c $(CCFLAGS) splash_screen.c $(GTKLIB) -o splash_screen.o
spectraread.o: peaks.cpp
	$(CC) -c $(CCFLAGS) peaks.cpp -o peaks.o
clean:
	rm peaks.o
	rm splash_screen.o
	rm peaks
	rm peakss
