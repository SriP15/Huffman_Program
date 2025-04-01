#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "code.h"
#include <inttypes.h>

//Initializing a Code
Code code_init(void) {
    Code code;
    code.top = 0;
    for (uint32_t i = 0; i < MAX_CODE_SIZE; i += 1) {
        code.bits[i] = 0;
    }
    return code;
}
//Returning the size of a code, given a pointer to a Code c
uint32_t code_size(Code *c) {
    return c->top;
}
//Returning true/false if, given a pointer to a Code, c is empty
bool code_empty(Code *c) {
    if (c->top == 0) {
        return true;
    }
    return false;
}

//Returning true/false if, given a pointer to a Code, c is full
bool code_full(Code *c) {
    if (code_size(c) == 256) {
        return true;
    }
    return false;
}
//Sets a bit in a Code *c, and returns true/false if it is set or not
bool code_set_bit(Code *c, uint32_t i) {
    if (i > 255) {
        return false;
    }
    c->bits[i / 8] |= (0x1 << (i % 8));
    return true;
}
//Clearing a bit in a code
bool code_clr_bit(Code *c, uint32_t i) {
    if (i > 255) {
        return false;
    }
    c->bits[i / 8] &= ~(0x1 << (i % 8));
    return true;
}
//Getting a bit from a code
bool code_get_bit(Code *c, uint32_t i) {
    if (i > 255) {
        return false;
    }
    uint8_t temp = c->bits[i / 8];
    temp &= (0x1 << (i % 8));
    temp = (temp >> (i % 8));
    if (temp == 1) {
        return true;
    }
    return false;
}
//Pushing a bit to a code
bool code_push_bit(Code *c, uint8_t bit) {
    if (code_full(c)) {
        return false;
    }
    if (bit == 1) {
        code_set_bit(c, c->top);
    } else {
        code_clr_bit(c, c->top);
    }
    c->top += 1;
    return true;
}
//Popping a bit from a code
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {
        return false;
    }
    c->top -= 1;
    *bit = code_get_bit(c, c->top);
    code_clr_bit(c, c->top);
    return true;
}

//Testing if functions above worked
void code_print(Code *c) {
    uint8_t bit = 0;
    //adding values to first index (0)
    code_push_bit(c, 1);
    printf("size is %" PRIu32 "\n", code_size(c));
    code_push_bit(c, 0);
    code_push_bit(c, 1);
    code_push_bit(c, 1);
    code_push_bit(c, 0);
    code_push_bit(c, 1);
    code_push_bit(c, 0);
    code_push_bit(c, 1);

    printf("size is %" PRIu32 "\n", code_size(c));

    printf("\n\n\n\n");
    //adding values to second index (1)
    code_push_bit(c, 1);
    code_push_bit(c, 1);
    code_push_bit(c, 0);
    code_push_bit(c, 1);
    code_push_bit(c, 0);
    code_push_bit(c, 1);

    printf("size is %" PRIu32 "\n", code_size(c));

    printf("\n\n\n\n");
    //popping values from second index (1)
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);

    printf("size is %" PRIu32 "\n", code_size(c));

    printf("\n\n\n\n");
    //popping values from first index (0)
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);
    code_pop_bit(c, &bit);
    printf("popped bit is %" PRIu8 "\n", bit);

    printf("size is %" PRIu32 "\n", code_size(c));

    //trying to pop empty stack
    printf("can bit be popped: %d.\n", code_pop_bit(c, &bit));
    printf("popped bit is %" PRIu8 "\n", bit);
}
