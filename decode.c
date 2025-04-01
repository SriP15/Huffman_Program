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

//This function prints the usage message and synopsis of the decoder program
//to standard error. It takes in the file name as its sole argument, and returns void.
//
//val: A string denoting the name of the file when called.
void usage() {
    fprintf(stderr, "SYNOPSIS\n"
                    "A Huffman decoder.\n"
                    "Decompresses a file using the Huffman coding algorithm.\n"
                    "\n"
                    "USAGE\n"
                    "  ./decode [-h] [-i infile] [-o outfile]\n"
                    "\n"
                    "OPTIONS\n"
                    "  -h             Program usage and help.\n"
                    "  -v             Print compression statistics.\n"
                    "  -i infile      Input file to decompress.\n"
                    "  -o outfile     Output of decompressed data.\n");
}

//Parses command-line options, and decompresses a file, given a compressed file.
//Returns a 0 or 1 depending on succesful exit of program.
//
//argc: int that stores number of command-line options passed
//argv stores command-line options passed
int main(int argc, char **argv) {
    bool verbose = false;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    int bytes = 0;
    uint8_t bit = 1;
    Node *root;
    Node *root2;
    Code table[ALPHABET];
    memset(table, 0, ALPHABET * sizeof(table[0]));
    Header header;
    uint64_t huff_tree = 0;
    uint64_t header_bytes = 0;
    uint64_t decoded_symbols = 0;
    float space_savings = 0;
    int opt = 0;

    //Parsing command-line options.
    while ((opt = getopt(argc, argv, "vhi:o:")) != -1) {
        switch (opt) {
        case 'v': verbose = true; break;
        case 'i':
            //setting infile to user-given input file.
            infile = open(optarg, O_RDONLY);
            break;
        case 'o':
            //setting outfile to user-given output file.
            outfile = open(optarg, O_WRONLY | O_CREAT);
            break;
        case 'h':
            //calling usage function.
            usage();
            return (EXIT_FAILURE);
            break;
        default:
            //If no correct options were chosen, call usage() and terminate.
            usage();
            return (EXIT_FAILURE);
        }
    }

    //Read in header from the input file.
    header_bytes = read_bytes(infile, (uint8_t *) &header, sizeof(Header));
    if (bytes == -1) {
        printf("[Decode]: Unable to read header.\n");
        return EXIT_FAILURE;
    }
    //Check if magic number is incorrect
    if (header.magic != MAGIC) {
        printf("Magic number is incorrect.\n");
        return EXIT_FAILURE;
    }

    //Setting file permissions of output file to be that of the input file.
    fchmod(outfile, header.permissions);

    //Creating and initializing a tree array
    uint8_t tree[header.tree_size];
    memset(tree, 0, header.tree_size * sizeof(uint8_t));

    //Reading in Huffman Tree to tree array
    huff_tree = read_bytes(infile, tree, header.tree_size);

    //Re-building the Huffman tree, and setting the root variable.
    root = rebuild_tree(header.tree_size, tree);

    //De-compressing the input file.
    if (!root) {
        printf("[Decode]: No root found\n");
        return EXIT_FAILURE;
    }
    root2 = root;
    while (decoded_symbols != header.file_size) {
        read_bit(infile, &bit);
        if (bit == 0) {
            if (root2) {
                root2 = root2->left;
            }
        } else if (bit == 1) {
            if (root2) {
                root2 = root2->right;
            }
        }

        if ((root2) && !(root2->left) && !(root2->right)) {
            uint8_t root_symbol[1] = { root2->symbol };
            write_bytes(outfile, root_symbol, 1);
            root2 = root;
            decoded_symbols += 1;
        }
    }
    bytes_read += (huff_tree + header_bytes);

    if (verbose) {
        space_savings = 100 * (1 - (((float) bytes_read) / ((float) header.file_size)));
        printf("Compressed file size: %" PRIu64 " bytes\n", bytes_read);
        printf("Decompressed file size: %" PRIu64 " bytes\n", header.file_size);
        printf("Space Saving: %.2f%%\n", space_savings);
    }
    delete_tree(&root);

    //Closing the files
    close(infile);
    close(outfile);
}
