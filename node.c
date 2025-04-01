#include <inttypes.h>
#include "node.h"
#include <stdio.h>
#include <stdlib.h>

//Dynamically allocates memory for a Node struct, and initializes a node.
//Returns a node pointer.
//
//symbol: a uint8_t that has the symbol of the node.
//frequency: a uint64_t that has the frequency of the node.
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (node) {
        node->symbol = symbol;
        node->frequency = frequency;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

//Deletes a Node, setting it equal to a NULL pointer afterwards.
//Returns void.
//
//n: a pointer to a pointer of a node.
//The node must already be created for it to be deleted.
void node_delete(Node **n) {
    free(*n);
    *n = NULL;
}

//Joins two nodes together, returning a pointer to a parent node with a '$' symbol,
//and a frequency of the two nodes combined.
//Returns a Node pointer.
//
//left: a Node pointer pointing to the left node.
//right: a Node pointer pointing to the right node.
Node *node_join(Node *left, Node *right) {
    Node *parent_node = node_create('$', (left->frequency + right->frequency));
    parent_node->left = left;
    parent_node->right = right;
    return parent_node;
}

//A debugger function that prints a node, and its children
//Node must have two children for this function.
//Returns void.
//
//n: a parent node that has two children
void node_print(Node *n) {
    printf("node parent symbol is %" PRIu8 ", node parent frequency is %" PRIu64 "\n", n->symbol,
        n->frequency);
    printf("node left symbol is %" PRIu8 ", node left frequency is %" PRIu64 "\n", n->left->symbol,
        n->left->frequency);
    printf("node right symbol is %" PRIu8 ", node right frequency is %" PRIu64 "\n",
        n->right->symbol, n->right->frequency);
}
