CFLAGS=-std=c11 -g -fno-common -Wreturn-type -I lib/

TARGET=bin/myon
SRCS=$(wildcard build/*.c)
OBJS=$(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): lib/myon.h

clean:
	rm -rf myon
	rm -f build/*.o
