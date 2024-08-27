# Compiler
CC = gcc
CFLAGS = -Wall -Werror -g

PROGRAM = simpleHttp

SRCS = main.c my_queue.c

TEST_DEPS = my_queue.o my_queue_test.o

OBJS = $(SRCS:.c=.o)

all: $(PROGRAM) test

$(PROGRAM): $(OBJS)
		$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_DEPS)
		$(CC) $(CFLAGS) -o test $(TEST_DEPS)

clean:
	rm -f $(OBJS) $(PROGRAM) test $(TEST_DEPS)

.PHONY: all clean