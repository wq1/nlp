#include "global.h"


WORD* sentence_splitter(char *sentence) {
  char *ptr;
  int n[2] = {0};

  // check wordage
  int wordage = 0;
  for (ptr = sentence; sscanf(ptr, " %*s%n", &n[1]) != EOF; ptr += n[1]) {
    wordage++;
  }
  if (wordage == 0) {
    return &DUMMY;
  }

  // memory allocation
  WORD *words = malloc(sizeof(WORD) * (wordage + 2));
  // wordage + 2?: last word will be DUMMY; first word will be START.

  // word separation
  int i = 1; // words[0] (i = 0) is START
  n[1] = 0;
  for (ptr = sentence; sscanf(ptr, " %n%*s%n", &n[0], &n[1]) != EOF; ptr += n[1] + 1) {
    words[i].word = ptr + n[0];
    i++;
    if (ptr[n[1]] == '\0') { // if reach the end of sentence
      break;
    } else {
      ptr[n[1]] = '\0';
    }
  }

  // head and tail editing
  words[0] = START;
  words[i] = DUMMY;

  return words;
}
