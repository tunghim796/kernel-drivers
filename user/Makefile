# CC = gcc
CFLAGS = -Wall -Wextra 

SRCS = main.c
OBJS = $(SRCS:.c=.o)
DEPS = file1.h file2.h

TARGET = user

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f $(OBJS) $(TARGET)