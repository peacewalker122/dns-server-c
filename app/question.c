#include "../include/question.h"
#include "../include/header.h"
#include <stddef.h> // Use <stddef.h> for C instead of <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void question_init(struct question *question, char *name, unsigned short type,
                   unsigned short clas) {
  question->name = name;
  question->type = type;
  question->clas = clas;
}

char *dezerialize_question(struct question *question) {
  int length = 0;
  char *result = NULL;
  char *name = question->name;
  int name_len = strlen(name);

  // Allocate a buffer to hold the output. Start small, we'll expand as needed.
  result = malloc(1);
  char buf[64];      // Temporary buffer for each label
  int buf_index = 0; // Index for the temporary buffer

  for (int i = 0; i <= name_len; i++) {
    if (name[i] == '.' || name[i] == '\0') {
      // We've reached the end of a label
      result = realloc(result, length + buf_index + 2);
      result[length++] = buf_index; // Set the length of the label

      // Copy the buffer content to result
      for (int j = 0; j < buf_index; j++) {
        result[length++] = buf[j];
      }
      buf_index = 0; // Reset the temporary buffer
    } else {
      // Add character to the temporary buffer
      if (buf_index <
          63) { // Prevent overflows (DNS label limit is 63 characters)
        buf[buf_index++] = name[i];
      }
    }
  }

  // Add the null terminator for the end of the name
  result = realloc(result, length + 1);
  result[length++] = 0;

  // Convert type and class to network byte order (big-endian)
  char *type = big_endian(question->type);
  char *clas = big_endian(question->clas);

  // Allocate space for type and class (2 bytes each)
  result = realloc(result, length + 4);
  memcpy(result + length, type, 2);
  memcpy(result + length + 2, clas, 2);

  length += 4;

  return result;
}
