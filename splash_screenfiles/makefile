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
	$(LD) -o spectrareads splash_screen.o $(LDFLAGS)
	$(LD) -o spectraread spectraread.o $(LDFLAGS)
splash_screen.o: splash_screen.c
	$(CC) -c $(CCFLAGS) splash_screen.c $(GTKLIB) -o splash_screen.o
spectraread.o: spectraread.cpp
	$(CC) -c $(CCFLAGS) spectraread.cpp -o spectraread.o
clean:
	rm spectraread.o
	rm splash_screen.o
	rm spectraread
	rm splash_screen
