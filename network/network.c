#include "global.h"


bool syntax_analyzer(const WORD *words, const SYNTAX *syntax) {
  fprintf(stderr, "[INFO]: Syntax checking started.\n");

  int i, j;
  bool transOK = false;
  for (i = 0; words[i+1].type != DUMMY.type; i++) {
    fprintf(stderr, "[DEBUG]: %s (%s) -> %s (%s) => ", words[i].type, words[i].word, words[i+1].type, words[i+1].word); fflush(stderr);
    for (j = 0; syntax[j].current != DUMMY_SYNTAX.current; j++) {
      if (strcmp(words[i].type, syntax[j].current) == 0) {
        if (strcmp(words[i+1].type, syntax[j].next) == 0) {
          fprintf(stderr, "\x1b[42mOK\x1b[49m\n"); // [DEBUG]:
          transOK = true;
          break;
        }
      }
    }

    if (transOK == true) {
      transOK = false;
    } else {
      fprintf(stderr, "\x1b[41mNG\x1b[49m\n"); // [DEBUG]:
      fprintf(stderr, "\x1b[31m[WARN]: Syntax error detected.\x1b[39m\n");
      return false;
    }
  }

  fprintf(stderr, "[INFO]: Syntax checking finished.\n");
  return true;
}
