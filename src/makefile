CC = g++

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp = .o)

INCLUDES = -I../include
CCFLAGS = -g -Wall -O0

OUTPUT = topurl

all:$(OUTPUT)

$(OUTPUT) : $(OBJS)
	$(CC) $^ -o $@ $(INCLUDES)

%.o : %.c
	$(CC) -c $< $(CCFLAGS)

clean:
	rm -rf *.out *.o

.PHONY:clean
