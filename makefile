
CC = gcc
CFLAGS = -Wall -Werror -std=c90 -g

# Source files
SRCS = pre_assembler1.c part2_functions.c part2read.c pre_assembler_functions.c shared_functions.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
EXEC = pre_assembler

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) -lm  # Added -lm to link the math library

# Compile .c files to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: clean all
