#include "global.h"

int WORDAGE;


WORD* sentence_splitter(char *sentence) {
  char *ptr;
  int n[2] = {0};

  // check wordage
  WORDAGE = 0;
  for (ptr = sentence; sscanf(ptr, " %*s%n", &n[1]) != EOF; ptr += n[1]) {
    WORDAGE++;
  }
  if (WORDAGE == 0) {
    return &DUMMY;
  }

  // memory allocation
  WORD *words = malloc(sizeof(WORD) * (WORDAGE + 1));
  // WORDAGE + 1?: last word will be DUMMY.

  // word separation
  int i = 0;
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
  words[i] = DUMMY;

  return words;
}
