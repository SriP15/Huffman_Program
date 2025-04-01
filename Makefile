CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: encode decode

node: node.o
	$(CC) -o node node.o

pq: pq.o node.o
	$(CC) -o pq pq.o node.o

stack: stack.o node.o
	$(CC) -o stack stack.o node.o

code: code.o
	$(CC) -o code code.o

io: io.o code.o
	$(CC) -o io io.o code.o

huffman: huffman.o code.o pq.o node.o io.o stack.o
	$(CC) -o huffman huffman.o code.o pq.o node.o io.o stack.o

encode: encode.o huffman.o code.o pq.o node.o io.o stack.o
	$(CC) -o encode encode.o huffman.o code.o pq.o node.o io.o stack.o

decode: decode.o huffman.o code.o pq.o node.o io.o stack.o
	$(CC) -o decode decode.o huffman.o code.o pq.o node.o io.o stack.o
	
node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

pq.o: pq.c pq.h node.h
	$(CC) $(CFLAGS) -c pq.c

stack.o: stack.c stack.h node.h
	$(CC) $(CFLAGS) -c stack.c

code.o: code.c code.h defines.h
	$(CC) $(CFLAGS) -c code.c

io.o: io.c code.h
	$(CC) $(CFLAGS) -c io.c

huffman.o: huffman.c huffman.h defines.h pq.h node.h code.h io.h stack.h
	$(CC) $(CFLAGS) -c huffman.c

encode.o: encode.c huffman.h defines.h pq.h node.h code.h io.h stack.h header.h
	$(CC) $(CFLAGS) -c encode.c

decode.o: decode.c huffman.h defines.h pq.h node.h code.h io.h stack.h header.h
	$(CC) $(CFLAGS) -c decode.c

clean:
	rm -f  encode
	rm -f  decode
	rm -f  *.o
	
format:
	clang-format -i -style=file *.h
	clang-format -i -style=file *.c

