TARGET=main
CC=g++
DEBUG=-g
OPT=-O0
WARN=-Wall
PTHREAD=-pthread
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe -fpermissive -fPIC  -rdynamic -Wl,--export-dynamic -lm
GTKLIB=`pkg-config --cflags --libs gtk+-3.0`
LD=g++
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic -fPIC -std=c99 -pthread -L../bin_modules/pygtkdatabox/gtkdatabox/install/lib -lgtkdatabox -I../bin_modules/pygtkdatabox/gtkdatabox/install/include -pipe -fPIC -W -pedantic -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED -DGSEAL_ENABLED -DGTK_DISABLE_SINGLE_INCLUDES -lm
OBJS=main.o
all: $(OBJS)
#	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
	$(LD) -o main main.o $(LDFLAGS)
main.o: main.c
	$(CC) -c $(CCFLAGS) $(LDFLAGS) -I../bin_modules/pygtkdatabox/gtkdatabox/install/include main.c $(GTKLIB) -o main.o
clean:
	rm main.o
