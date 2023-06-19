CC = gcc
CFLAGS = -g -O2
CPPFLAGS =
LDFLAGS =

OBJS = main.o vector.o bitset.o

all: labyrinth

labyrinth: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o$@ $^

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -MD -c -o$@ $<

clean:
	-rm -f labyrinth $(OBJS) $(OBJS:.o=.d)

.PHONY: all clean

-include $(OBJS:.o=.d)
