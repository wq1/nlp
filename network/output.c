#include "global.h"


void print_words(const WORD *words, const size_t buf_size) {
  const int WORD_SEPARATION = 2;
  const int LINES = 3;

  int i;

  // memory allocation
  char *line[LINES];
  line[0] = malloc(sizeof(char) * buf_size * LINES); // get heap area

  for (i = 1; i < LINES; i++) {
    line[i] = line[0] + i * buf_size;
  }

  // initializing couters and lines
  int count[LINES];
  for (i = 0; i < LINES; i++) {
    line[i][0] = '\0';
    count[i] = 0;
  }

  // padding
  int max = 0;
  int j, k;
  int remain = buf_size;
  for (i = 0; words[i].word != DUMMY.word; i++) {
    count[0] += snprintf(line[0] + count[0], remain, "%s", words[i].word);
    count[1] += snprintf(line[1] + count[1], remain, "|");
    count[2] += snprintf(line[2] + count[2], remain, "%s", words[i].type);

    // getting the longest elements
    for (j = 0; j < LINES; j++) {
      if (count[j] > buf_size - 1) { // minus 1?: last element must be terminated with '\0'
        // Because snprintf doesn't return the actual number of written characters,
        // count[] may exceeds buf_size.
        count[j] = buf_size - 1;
      }
      if (max < count[j]) {
        max = count[j];
      }
    }

    // padding with spaces
    for (j = 0; j < LINES; j++) {
      while (count[j] < max) {
        line[j][count[j]++] = ' ';
      }
    }
    // now, count[] == max

    remain = buf_size - max;

    if (remain > WORD_SEPARATION) {
      for (j = 0; j < LINES; j++) {
        for (k = 0; k < WORD_SEPARATION; k++) {
          line[j][count[j]++] = ' ';
        }
      }
      remain -= WORD_SEPARATION;
    } else {
      break;
    }
  }

  // terminating and printing each line
  for (i = 0; i < LINES; i++) {
    if (buf_size - count[i] > WORD_SEPARATION) {
      line[i][count[i] - WORD_SEPARATION] = '\0'; // remove last padding
    } else {
      line[i][count[i]] = '\0';
    }
    printf("%s\n", line[i]);
  }

  free(line[0]);
  return;
}
