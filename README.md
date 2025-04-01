# Assignment 6: Huffman Coding
This assignment essentially simulates a Huffman Encoder. Given an input file, it compresses it (and can also decompress it.) This program is dependent on two main programs for its functionality: encode and decode. The file encode compressed the file, and decode decompresses it.

## How to build the program:
To build the program, type `$ make` or `$ make all`. This builds the two encode and decode programs. Optionally, typing `$ make encode`, and `$ make decode` builds the two programs individually.

## How to run the program:
Before running the program, to clean it, type, `$ make clean`. To run the encode program, type `$ ./encode`. To run the decode program, type `$ ./decode`.

## Command-line Options:
The command-line options for both the encode and decode programs are the same:
 - -h : Prints the usage message, and purpose of the program.
 - -i (input file) : This is the input file for the encrypt/decrypt programs. For the encrypt program, it is the uncompressed file. For the decrypt program, it is the compressed file. The default for both files is standard input.
 - -o (output file) : This is the file to display the output. For the encrypt program, it is the compressed file. For the decrypt program, it is the de-compressed file. The default for both files is standard output.
 - v: This is the verbose mode. It prints out the compressions statistics to standard output.

## Scan-build:
Scan build revealed no errors when I ran it.
