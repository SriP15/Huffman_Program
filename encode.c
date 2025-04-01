#include "node.h"
#include "code.h"
#include "defines.h"
#include "pq.h"
#include "huffman.h"
#include "stack.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <string.h>
#include "io.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include "header.h"
#include <sys/types.h>

//This function prints the usage message and synopsis of the encoder program
//to standard error. It takes in the file name as its sole argument, and returns void.
//
//val: A string denoting the name of the file when called.
void usage() {
    fprintf(stderr, "SYNOPSIS\n"
                    "A Huffman encoder.\n"
                    "Compresses a file using the Huffman coding algorithm.\n"
                    "\n"
                    "USAGE\n"
                    "  ./encode [-h] [-i infile] [-o outfile]\n"
                    "\n"
                    "OPTIONS\n"
                    "  -h             Program usage and help.\n"
                    "  -v             Print compression statistics.\n"
                    "  -i infile      Input file to compress.\n"
                    "  -o outfile     Output of compressed data.\n");
}

//Parses command-line options, and compresses a file, given a input file.
//Returns a 0 or 1 depending on succesful exit of program.
//
//argc: int that stores number of command-line options passed
//argv stores command-line options passed
int main(int argc, char **argv) {
    bool verbose = false;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    int bytes = 0;
    int write_byte = 0;
    uint64_t hist[ALPHABET] = { 0 };
    uint8_t buf[BLOCK];
    Node *root;
    Code table[ALPHABET];
    memset(table, 0, ALPHABET * sizeof(table[0]));
    uint32_t unique_symbols = 0;
    Header header;
    struct stat file_stats;
    float space_savings = 0;
    //int header_bytes = 0;
    int opt = 0;

    //Parsing command-line options.
    while ((opt = getopt(argc, argv, "vhi:o:")) != -1) {
        switch (opt) {
        case 'v': verbose = true; break;
        case 'i': infile = open(optarg, O_RDONLY); break;
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT); break;
        case 'h':
            usage();
            return (EXIT_FAILURE);
            break;
        default: usage(); return (EXIT_FAILURE);
        }
    }
    //Creating a temporary file if the input file is not seekable.
    //I learned about how to create temporary files from attending Eugene's section.
    if (lseek(infile, 0, SEEK_CUR) == -1) {
        char temp_file_name[1024];
        snprintf(temp_file_name, 1024, "/tmp/encode%d.tmp", getpid());
        int temp_file = open(temp_file_name, O_CREAT | O_TRUNC | O_RDWR, 0600);
        unlink(temp_file_name);

        do {
            bytes = read_bytes(infile, buf, BLOCK);
            write_byte = write_bytes(temp_file, buf, bytes);
            if (write_byte == -1) {
                break;
            }
        } while (bytes > 0);

        if (lseek(temp_file, 0, SEEK_SET) == 0) {
            close(infile);
            infile = temp_file;
        }
    }

    //Reading in input file to create a histogram of symbol frequencies.
    do {
        bytes = read_bytes(infile, buf, BLOCK);
        for (int32_t i = 0; i < bytes; i += 1) {
            hist[buf[i]] += 1;
        }
    } while (bytes > 0);

    //Incrementing count of hist[0] and hist[255] by 1.
    hist[0] += 1;
    hist[255] += 1;

    //Counting the number of unique symbols.
    for (uint32_t i = 0; i < ALPHABET; i += 1) {
        if (hist[i] > 0) {
            unique_symbols += 1;
        }
        //         printf("hist[%" PRIu32 "] = %" PRIu64 "\n",i, hist[i]);
    }

    //Building a Huffman tree, and storing the returned root value in root.
    root = build_tree(hist);

    //Building a codes table.
    build_codes(root, table);
    /*printf("********codes table*********\n");
	for (int i= 0; i < ALPHABET; i+=1) {
		//if (table[i].bits[0] != 0) { 
		  //   unique_symbols += 1;
		//}
		printf("top is %" PRIu32 "\n", table[i].top);
		printf("table[%d %c] = ",i, i);
		for (int8_t x = 31; x >= 0; x-=1) {
				printf("%" PRIu8 " ", table[i].bits[x]);
		}
		printf("\n");
	}
	printf("********codes table*********\n");
*/
    //Getting file statistics using fstat.
    fstat(infile, &file_stats);
    header.magic = MAGIC;
    header.tree_size = (3 * unique_symbols) - 1;
    header.permissions = file_stats.st_mode;
    header.file_size = file_stats.st_size;

    //Setting file permissions of output file to be that of the input file.
    fchmod(outfile, header.permissions);

    //Writing the header to the output file.
    bytes_written += write_bytes(outfile, (uint8_t *) &header, sizeof(Header));

    //Dumping the tree to the output file.
    dump_tree(outfile, root);

    lseek(infile, 0, SEEK_SET);
    //Writing the codes of each symbol to the output file.

    do {
        bytes = read_bytes(infile, buf, BLOCK);
        for (int32_t i = 0; i < bytes; i += 1) {
            write_code(outfile, &table[buf[i]]);
        }

    } while (bytes > 0);

    //Flushing the codes to ensure all bytes are written.
    flush_codes(outfile);

    if (verbose) {
        space_savings = 100 * (1 - (((float) bytes_written) / ((float) header.file_size)));
        printf("Uncompressed file size: %" PRIu64 " bytes\n", header.file_size);
        printf("Compressed file size: %" PRIu64 " bytes\n", (bytes_written));
        printf("Space Saving: %.2f%%\n", space_savings);
    }
    delete_tree(&root);
    //Closing the files
    close(infile);
    close(outfile);
    return 0;
}
