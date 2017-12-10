CC = gcc
CFLAGS = -Wall

all: main

main: main.o dict_builder.o syntax_builder.o splitter.o post.o network.o output.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c global.h
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) *.o

run: all
	cat data.txt | ./main dict.csv syntax.csv > result.txt
