#include "global.h"

// global variables
WORD DUMMY;
SYNTAX DUMMY_SYNTAX;
CELL DUMMY_CELL;


int main(int argc, char **argv) {
  fprintf(stderr, "[INFO]: This CYK parser can find up to %d trees (determined at compile time)\n", MAX_TYPE);

  size_t max_line_length;
  if (argc > 3) {
    max_line_length = strtoul(argv[3], NULL, 10);
  } else {
    max_line_length = 1024;
  }
  fprintf(stderr, "[INFO]: up to %ld characters are allowed for each sentence\n", max_line_length - 1);

  fprintf(stderr, "[INFO]: each sentence must be separated by newline character \'\\n\'\n");
  fprintf(stderr, "[INFO]: each word must be separated by whitespace characters\n");

  // init global variables
  int i;
  DUMMY.word = "";
  DUMMY.type = (char **)"?";
  DUMMY_SYNTAX.lhs = "";
  DUMMY_SYNTAX.rhs1 = "";
  DUMMY_SYNTAX.rhs2 = "";
  DUMMY_CELL.index = -1;
  DUMMY_CELL.word = "";
  for (i = 0; i <= MAX_TYPE; i++) { // type[MAX_TYPE + 1] is dummy
    DUMMY_CELL.type[i].index = i;
    DUMMY_CELL.type[i].type = "";
    DUMMY_CELL.type[i].cell = &DUMMY_CELL;
    DUMMY_CELL.type[i].prev_left_type = &DUMMY_CELL.type[i];
    DUMMY_CELL.type[i].prev_right_type = &DUMMY_CELL.type[i];
  }

  WORD *dict = build_dict(argv[1]);
  SYNTAX *syntax = build_syntax(argv[2]);

  WORD *words = NULL;
  char *sentence = malloc(sizeof(char) * max_line_length);

  while (fgets(sentence, max_line_length, stdin) != NULL) {
    fprintf(stderr, "[INFO]: Parsing started.\n");
    words = sentence_splitter(sentence);

    bool typeOK = type_analyzer(words, dict);
    // print_words(words, max_line_length, NULL);

    CELL *cells = NULL;
    int *type_num = malloc(sizeof(int) * WORDAGE);
    bool sentenceOK = false;
    if (typeOK == true) {
      cells = syntax_analyzer(words, syntax);
      if (cells != NULL) {
        for (i = 0; cells[CELL_LENGTH - 1].type[i].type != DUMMY_CELL.type[i].type; i++) {
          if (strcmp(cells[CELL_LENGTH - 1].type[i].type, "S") == 0) {
            sentenceOK = true;
            print_sexps(&(cells[CELL_LENGTH - 1].type[i]), type_num, 0);
            // printf("\n");
            // print_words(words, max_line_length, type_num);
            // printf("\n");
          }
        }
        if (sentenceOK == true) {
          fprintf(stderr, "[INFO]: \x1b[42mSentence OK\x1b[49m\x1b[32m (sentence is acceptable.)\x1b[39m\n");
        } else {
          fprintf(stderr, "\x1b[31m[WARN]: \x1b[39m\x1b[41mSentence NG\x1b[49m\x1b[31m (sentence is not acceptable.)\x1b[39m\n");
        }
        fprintf(stderr, "[INFO]: Parsing finished.\n");
      } else {
        fprintf(stderr, "\x1b[31m[WARN]: Parsing aborted.\x1b[39m\n");
      }
    } else {
      fprintf(stderr, "\x1b[31m[WARN]: can't start syntax checking because of unknown words.\x1b[39m\n");
      fprintf(stderr, "\x1b[31m[WARN]: Parsing aborted.\x1b[39m\n");
    }

    free(cells);
    free(type_num);
    for (i = 0; words[i].word != DUMMY.word; i++) {
      if (words[i].type_is_new_array == true) {
        free(words[i].type);
      }
    }
    free(words);
  }

  fprintf(stderr, "[INFO]: exiting...");
  fflush(stderr);

  /*
  // no need?
  free(sentence);
  free(syntax[0].lhs);
  free(syntax);
  free(dict[0].word);
  free(dict[0].type);
  free(dict);
  */

  fprintf(stderr, " Goodbye.\n");

  return 0;
}
