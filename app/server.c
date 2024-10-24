#include "../include/header.h"
#include "../include/question.h"
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define REUSE_PORT = 1;

void print_hex(const char *data, size_t length) {
  for (size_t i = 0; i < length; i++) {
    printf("%02X ", (unsigned char)data[i]);
  }
  printf("\n");
}

int main() {
  // Disable output buffering
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.
  printf("Logs from your program will appear here!\n");

  // Uncomment this block to pass the first stage
  int udpSocket, client_addr_len;
  struct sockaddr_in clientAddress;

  udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (udpSocket == -1) {
    printf("Socket creation failed: %s...\n", strerror(errno));
    return 1;
  }

  // Since the tester restarts your program quite often, setting
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(udpSocket, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) <
      0) {
    printf("SO_REUSEPORT failed: %s \n", strerror(errno));
    return 1;
  }

  struct sockaddr_in serv_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(2053),
      .sin_addr = {htonl(INADDR_ANY)},
  };

  if (bind(udpSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
    printf("Bind failed: %s \n", strerror(errno));
    return 1;
  }

  int bytesRead;
  char buffer[512];
  socklen_t clientAddrLen = sizeof(clientAddress);

  while (1) {
    // Receive data
    bytesRead = recvfrom(udpSocket, buffer, sizeof(buffer), 0,
                         (struct sockaddr *)&clientAddress, &clientAddrLen);
    if (bytesRead == -1) {
      perror("Error receiving data");
      break;
    }

    buffer[bytesRead] = '\0';
    printf("Received %d bytes: %s\n", bytesRead, buffer);

    struct question question;
    question.name = "google.com";

    // Create an empty response
    struct header resp;
    resp.id = 1234;
    resp.qr = 1;
    resp.aa = 0;
    resp.tc = 0;
    resp.rd = 0;
    resp.ra = 0;
    resp.z = 0;
    resp.opcode = 0;
    resp.qdcount = 1;
    resp.ancount = 0;
    resp.nscount = 0;
    resp.arcount = 0;

    char *response = serialize(&resp);
    char *question_buf = dezerialize_question(&question);

    print_hex(question_buf, strlen(question_buf));

    response = realloc(response, strlen(response) + strlen(question_buf) + 12);
    strcat(response, question_buf);
    free(question_buf);

    // Send response
    if (sendto(udpSocket, response, 12, sizeof(response),
               (struct sockaddr *)&clientAddress,
               sizeof(clientAddress)) == -1) {
      perror("Failed to send response");
    }
    free(response);
  }

  close(udpSocket);

  return 0;
}
