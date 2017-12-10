#include "global.h"


bool type_analyzer(WORD *words, const WORD *dict) {
  fprintf(stderr, "[INFO]: Part-of-Speech tagging (POST) has been executed.\n");

  int i, j, k;
  bool typeFound = false;
  bool noUnknown = true;
  for (i = 0; words[i].word != DUMMY.word; i++) {
    k = 0;
    // fprintf(stderr, "[DEBUG]: %s => ", words[i].word); fflush(stderr);
    for (j = 0; dict[j].word != DUMMY.word; j++) {
      if (strcmp(words[i].word, dict[j].word) == 0) {
        typeFound = true;
        if (k >= MAX_TYPE) {
          fprintf(stderr, "\x1b[31m[WARN]: words[%d].type[MAX_TYPE] overflow (the result may be wrong)\x1b[39m\n", i);
          break;
        }
        // fprintf(stderr, "%s\n", dict[j].type); // [DEBUG]:
        words[i].type[k++] = dict[j].type[0];
      }
    }
    words[i].type[k] = DUMMY.type[k];

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
