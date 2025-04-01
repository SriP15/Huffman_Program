#include "code.h"
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "defines.h"
#include "io.h"

//Initializing global variables
uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
static uint8_t buffer[BLOCK] = { 0 };
static uint32_t index = 0;

//Reads nbytes number of bytes from infile, and stores it to buf.
//Returns an integer that reveals the number of bytes that were read.
//
//infile: file descriptor of the infile.
//buf: an array of uint8_ts that read values are stored to.
//nbytes: an integer number of bytes to read into the buffer.
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int64_t bytes = 0;
    uint64_t local_bytes_read = 0;
    do {
        bytes = read(infile, buf + local_bytes_read, nbytes - local_bytes_read);

        if (bytes == -1) {
            printf("[read_bytes]: Read failed.\n");
            break;
        }
        local_bytes_read += bytes;
    } while (((int) local_bytes_read < nbytes) && (bytes != 0));

    return (local_bytes_read);
}

//Reads nbytes number of bytes from buf to outfile.
//Returns an integer that reveals the number of bytes that were read.
//
//outfile: file descriptor of the outfile.
//buf: an array of uint8_ts that values to write are stored in.
//nbytes: an integer number of bytes to write to the outfile.
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int64_t bytes = 0;
    int64_t local_bytes_written = 0;

    do {
        bytes = write(outfile, buf + local_bytes_written, nbytes - local_bytes_written);
        if (bytes == -1) {
            printf("[write bytes]: Write failed.\n");
            break;
        }
        local_bytes_written += bytes;
    } while (((int) local_bytes_written < nbytes) && (bytes != 0));

    return (local_bytes_written);
}
//Reads a bit at a time from infile.
//Returns an boolean indicating if the entire file was read. Returns true
//if the file was not read entirely. Returns false if the file was read entirely.
//
//infile: file descriptor of input file to read from.
//bit: a pointer to a uint8_t value to store the read bit value to.
bool read_bit(int infile, uint8_t *bit) {
    int bytes;
    if (bytes_read == 0) {
        bytes = read_bytes(infile, buffer, BLOCK);
        bytes_read += bytes;
    } else {
        bytes = bytes_read;
    }
    uint32_t file_end = (BLOCK * 8);
    if (bytes < BLOCK) {
        file_end = (8 * bytes) + 1;
    }
    uint8_t temp = buffer[index / 8];
    temp &= (0x1 << (index % 8));
    temp = (temp >> (index % 8));
    *bit = temp;
    index += 1;

    if (index == 8 * (sizeof(buffer))) {
        bytes = read_bytes(infile, buffer, BLOCK);
        bytes_read += bytes;
        index = 0;
        if (bytes < BLOCK) {
            file_end = (8 * bytes) + 1;
        }
    }
    if (index != file_end) {
        return true;
    } else {
        return false;
    }
}

//I learned about writing and flushing codes from attending Eugene's section.

//Writes the given code c to outfile.
//Returns nothing (void.)
//
//outfile: file descriptor of output file to write to.
//c: pointer to a code whose value must be written to outfile.
void write_code(int outfile, Code *c) {
    uint8_t bit;
    uint32_t ind = 0;
    do {
        bit = code_get_bit(c, ind);
        //printf("\nc bit is %" PRIu32 "\n", bit);
        if (bit == 1) {
            buffer[index / 8] |= (0x1 << (index % 8));
        } else {
            buffer[index / 8] &= ~(0x1 << (index % 8));
        }

        //printf("buffer[%" PRIu32 "] = %" PRIu8 "\n", index/8, buffer[index/8]);

        index += 1;
        ind += 1;
    } while ((index != 8 * (sizeof(buffer))) && (ind < c->top));

    if (index == 8 * (sizeof(buffer))) {
        flush_codes(outfile);
        index = 0;
    }
}
//Writes any bytes still left in the buffer to outfile.
//Returns nothing (void.)
//
//outfile: file descriptor of output file to write buffered values to.
void flush_codes(int outfile) {
    if (index) {
        uint32_t print_bytes;
        //printf("index is %" PRIu32 "\n",index);
        if (index % 8 == 0) {
            print_bytes = (index / 8);
        } else {
            print_bytes = (index / 8) + 1;

            for (uint32_t b = (index % 8) + 1; b <= 8; b += 1) {
                buffer[index / 8] &= ~(0x1 << b);
            }
        }
        bytes_written += write_bytes(outfile, buffer, print_bytes);
    }
}
