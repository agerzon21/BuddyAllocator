CC = gcc
CFLAGS = -Wall
TARGET = asst1
OBJECTS = mymalloc.o memgrind.o
DEPS = mymalloc.h

default: $(TARGET)
all: default

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -lm -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)