CC = gcc
CFLAGS = -Wall

all: main

main: main.o dict_builder.o syntax_builder.o splitter.o post.o cyk.o output.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c global.h
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) *.o

run: all
	cat child.txt | ./main dict.csv syntax_child.csv > result.txt
	cat telescope.txt | ./main dict.csv syntax_telescope.csv >> result.txt
	cat flies.txt | ./main dict.csv syntax_flies.csv >> result.txt
