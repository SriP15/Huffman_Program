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

static Code c;
PriorityQueue *pq;
Stack *stack;
//The pseudocode for the functions in this file were given in the Assignment 6 document.

//Builds a Huffman tree, given a histogram of symbol frequencies.
//Returns a pointer to the root of the tree.
//
//hist: an array of uint64_t values that store the frequencies of each symbol (a histogram) in a file.
Node *build_tree(uint64_t hist[static ALPHABET]) {
    pq = pq_create(64000);
    Node *left;
    Node *right;
    Node *parent;
    Node *root;
    for (uint32_t i = 0; i < (uint32_t) 256; i += 1) {
        if (hist[i] > 0) {
            Node *node = node_create(i, hist[i]);
            enqueue(pq, node);
        }
    }
    while (pq_size(pq) >= 2) {
        dequeue(pq, &left);
        dequeue(pq, &right);
        parent = node_join(left, right);
        enqueue(pq, parent);
    }
    dequeue(pq, &root);
    return (root);
}

//Builds codes for each symbol in a file given the root of the tree.
//Returns nothing (void.)
//
//root: pointer to the root Node of the tree.
//table: an array to store the Codes of each symbol in the file.
void build_codes(Node *root, Code table[static ALPHABET]) {
    uint8_t bit = 0;

    if (root) {
        if (!(root->left) && !(root->right)) {
            table[root->symbol] = c;
        } else {
            code_push_bit(&c, 0);
            build_codes(root->left, table);
            code_pop_bit(&c, &bit);

            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c, &bit);
        }
    }
}

//Dumps a Huffman tree to outfile.
//Returns nothing (void.)
//
//outfile: a file descriptor to dump the tree to.
//root: pointer to the root Node of the tree.
void dump_tree(int outfile, Node *root) {

    if (root) {
        uint8_t L[1] = { 'L' };
        uint8_t I[1] = { 'I' };
        uint8_t root_symbol[1] = { root->symbol };
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);

        if (!(root->left) && !(root->right)) {
            bytes_written += write_bytes(outfile, L, 1);
            bytes_written += write_bytes(outfile, root_symbol, 1);
        } else {
            bytes_written += write_bytes(outfile, I, 1);
        }
    }
}

//Re-builds a Huffman tree given the number of bytes in a tree, and
//an array of each byte in the Huffman tree.
//Returns a pointer to the root Node of the tree.
//
//nbytes: a uint16_t value indicating the number of bytes in a tree.
//tree: an array of uint8_ts, containing each byte of the Huffman tree.
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    stack = stack_create(64000);
    Node *right;
    Node *left;
    Node *parent;
    Node *root;
    for (uint16_t i = 0; i < nbytes; i += 1) {
        if (tree[i] == 'L') {
            Node *node = node_create(tree[i + 1], 0);
            i += 1;
            stack_push(stack, node);
        } else if (tree[i] == 'I') {
            stack_pop(stack, &right);
            stack_pop(stack, &left);
            parent = node_join(left, right);
            stack_push(stack, parent);
        }
    }
    stack_pop(stack, &root);
    return (root);
}

//Deletes a tree by using post-order traversal.
//Returns nothing (void).
//
//root: a pointer to a pointer of a root.
//I learned how to delete trees from Professor Long's lecture
//on trees and tries.
void delete_tree(Node **root) {
    if (*root) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);
        node_delete(root);
        root = NULL;
    }
    //pq_delete(&pq);
}
