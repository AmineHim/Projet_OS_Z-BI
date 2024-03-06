.PHONY: all clean
CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lm -lgmp -g
TARGET = testprojet 
OBJFILES = test.o sha256.o sha256_utils.o


all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
