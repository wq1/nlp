#include "global.h"


bool type_analyzer(WORD *words, const WORD *dict) {
  fprintf(stderr, "[INFO]: Part-of-Speech tagging (POST) has been executed.\n");

  int i, j, k;
  bool typeFound = false;
  bool noUnknown = true;

  int array_size = DEFAULT_LENGTH_OF_ARRAY;
  char **type = malloc(sizeof(char *) * array_size);
  int type_index = 0;
  for (i = 0; words[i].word != DUMMY.word; i++) {
    outer: // outer loop
    words[i].type = &type[type_index];
    if (type_index == 0) {
      words[i].type_is_new_array = true;
    }
    k = 0;
    // fprintf(stderr, "[DEBUG]: %s => ", words[i].word); fflush(stderr);
    for (j = 0; dict[j].word != DUMMY.word; j++) {
      if (strcmp(words[i].word, dict[j].word) == 0) {
        typeFound = true;
        if (type_index >= (array_size - 1)) { // -1?: last element is preserved for dummy element.
          array_size *= 2; // new array will be expanded.
          fprintf(stderr, "[DEBUG]: words[i].type is full. increasing array size to %d elements.\n", array_size);
          if (k == type_index) {
            free(type);
          }
          type = malloc(sizeof(char *) * array_size);
          type_index = 0;
          goto outer; // continue from outer loop
        }
        words[i].type[k] = dict[j].type[0];
        // fprintf(stderr, "%s\n", words[i].type[k]); // [DEBUG]:
        k++;
        type_index++;
      }
    }
    words[i].type[k] = DUMMY.type[0];
    k++;
    type_index++;

    if (typeFound == true) {
      typeFound = false;
    } else {
      fprintf(stderr, "[DEBUG]: %s => ", words[i].word);
      fprintf(stderr, "\x1b[41m?\x1b[49m\n"); // [DEBUG]:
      fprintf(stderr, "\x1b[31m[ERROR]: Unknown word \"%s\" detected.\x1b[39m\n", words[i].word);
      noUnknown = false;
    }
  }

  fprintf(stderr, "[INFO]: POST finished.\n");
  return noUnknown;
}
