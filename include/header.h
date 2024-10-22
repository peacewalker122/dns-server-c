// header.h
#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <string.h>

// Define the `struct header`
struct header {
    unsigned short id;
    unsigned char qr : 1;
    unsigned char opcode : 4;
    unsigned char aa : 1;
    unsigned char tc : 1;
    unsigned char rd : 1;
    unsigned char ra : 1;
    unsigned char z : 3;
    unsigned char rcode : 4;
    unsigned short qdcount;
    unsigned short ancount;
    unsigned short nscount;
    unsigned short arcount;
};

// Function prototypes
char *big_endian(short val);
short little_endian(char *buf);
char *serialize(struct header *header);

#endif
