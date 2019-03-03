CC = g++

SRCS = $(wildcard *.cpp ../src/*.cpp)
OBJS = $(SRCS:.cpp = .o)

INCLUDES = -I../include 
CCFLAGS = -g -Wall -O0 

OUTPUT = topurl_test

all:$(OUTPUT)

$(OUTPUT) : $(OBJS)
    $(CC) $^ -o $@ $(INCLUDES)

%.o : %.cpp
    $(CC) -E $< $(CCFLAGS)

clean:
    rm -rf *.out *.o topurl_test

.PHONY:clean
