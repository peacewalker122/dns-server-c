#include <stdlib.h>
#include <string.h>

#ifndef HEADER
#define HEADER

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

char *big_endian(short val) {
  char *buf = malloc(2);
  buf[0] = val >> 8;
  buf[1] = val & 0xFF;
  return buf;
}

short little_endian(char *buf) { return (buf[0] << 8) | buf[1]; }

char *serialize(struct header *header) {
  char *buf = malloc(12);
  char *id = big_endian(header->id);
  char *qcount = big_endian(header->qdcount);
  char *acount = big_endian(header->ancount);
  char *nscount = big_endian(header->nscount);
  char *arcount = big_endian(header->arcount);

  memcpy(buf, id, 2);
  buf[2] = (header->qr << 7) | (header->opcode << 3) | (header->aa << 2) |
           (header->tc << 1) | header->rd;
  buf[3] = (header->ra << 6) | (header->z << 3) | header->rcode;
  memcpy(buf + 4, qcount, 2);
  memcpy(buf + 6, acount, 2);
  memcpy(buf + 8, nscount, 2);
  memcpy(buf + 10, arcount, 2);

  return buf;
}

#endif
