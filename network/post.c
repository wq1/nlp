#include "global.h"


bool type_analyzer(WORD *words, const WORD *dict) {
  fprintf(stderr, "[INFO]: Part-of-Speech tagging (POST) has been executed.\n");

  int i, j;
  bool typeFound = false;
  bool noUnknown = true;
  for (i = 1; words[i].word != DUMMY.word; i++) { // words[0] is START
    fprintf(stderr, "[DEBUG]: %s => ", words[i].word); fflush(stderr);
    for (j = 0; dict[j].word != DUMMY.word; j++) {
      if (strcmp(words[i].word, dict[j].word) == 0) {
        fprintf(stderr, "%s\n", dict[j].type); // [DEBUG]:
        words[i].type = dict[j].type;
        typeFound = true;
        break;
      }
    }

    if (typeFound == true) {
      typeFound = false;
    } else {
      fprintf(stderr, "\x1b[41m?\x1b[49m\n"); // [DEBUG]:
      fprintf(stderr, "\x1b[31m[ERROR]: Unknown word \"%s\" detected.\x1b[39m\n", words[i].word);
      noUnknown = false;
      words[i].type = DUMMY.type;
    }
  }

  fprintf(stderr, "[INFO]: POST finished.\n");
  return noUnknown;
}
