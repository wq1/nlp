#include "global.h"


void _print_sexps(const TYPE *type, int *word_num, int *type_num, int indent, int depth) {
  if (type->type == DUMMY_CELL.type[0].type) {
    return;
  }

  if (indent > 0) {
    int i, j;
    printf("\n");
    for (i = 0; i < depth; i++) {
      for (j = 0; j < indent; j++) {
        printf(" ");
      }
    }
  }
  printf("(%s%d", type->type, (type->cell)->index + 1);

  if ((type->cell)->word != DUMMY_CELL.word) {
    if (type->prev_left_type == &DUMMY_CELL.type[0]) {
      printf(" \"%s\"", (type->cell)->word);
      if (type_num != NULL) {
        //fprintf(stderr, "%d, ", type->index);
        type_num[(*word_num)++] = type->index;
      }
    }
  }

  _print_sexps(type->prev_left_type, word_num, type_num, indent, depth + 1);
  _print_sexps(type->prev_right_type, word_num, type_num, indent, depth + 1);
  printf(")");

  return;
}


void print_sexps(const TYPE *type, int *type_num, int indent) {
  int word_num = 0;
  _print_sexps(type, &word_num, type_num, indent, 0);
  printf("\n");
  return;
}


// legacy
void print_words(const WORD *words, const size_t buf_size, const int *type_num) {
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
    if (type_num == NULL) {
      count[2] += snprintf(line[2] + count[2], remain, "%s", words[i].type[0]);
    } else {
      count[2] += snprintf(line[2] + count[2], remain, "%s", words[i].type[type_num[i]]);
    }

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
  // fprintf(stderr, "[INFO]:\n");
  for (i = 0; i < LINES; i++) {
    if (buf_size - count[i] > WORD_SEPARATION) {
      line[i][count[i] - WORD_SEPARATION] = '\0'; // remove last padding
    } else {
      line[i][count[i]] = '\0';
    }
    fprintf(stdout, "%s\n", line[i]);
  }

  free(line[0]);
  return;
}
