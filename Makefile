# Compiler
CC = gcc
CFLAGS = -Wall -Werror -g

PROGRAM = simpleHttp

SRCS = main.c

OBJS = $(SRCS:.c=.o)

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
		$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(PROGRAM)

.PHONY: all clean