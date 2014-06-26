SRCS = read_sensors.c

PROJ_NAME=read_sensors

CC = gcc
CFLAGS = -g -02 -Wall
CFLAGS = -I../source

OBJS = $(SRCS:.c=.o)

.PHONY: proj

all: proj

proj: $(PROJ_NAME)

$(PROJ_NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -r $(PROJ_NAME)
	find . -name \*.o -type f -delete

