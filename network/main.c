#include "global.h"

WORD START;
WORD DUMMY;
SYNTAX DUMMY_SYNTAX;


int main(int argc, char **argv) {
  const size_t DEFAULT_LINE_LENGTH = 1024;

  size_t max_line_length;
  if (argc > 3) {
    max_line_length = strtoul(argv[3], NULL, 10);
  } else {
    max_line_length = DEFAULT_LINE_LENGTH;
  }
  fprintf(stderr, "[INFO]: up to %ld characters are allowed for each sentence\n", max_line_length - 1);

  fprintf(stderr, "[INFO]: each sentence must be separated by newline character \'\\n\'\n");
  fprintf(stderr, "[INFO]: each word must be separated by whitespace characters\n");

  // init global variables
  DUMMY.word = "";
  DUMMY.type = "?";
  DUMMY_SYNTAX.current = "";
  DUMMY_SYNTAX.next = "";
  START.word = " ";
  START.type = "start";

  WORD *dict = build_dict(argv[1]);
  SYNTAX *syntax = build_syntax(argv[2]);

  WORD *words = NULL;
  char *sentence = malloc(sizeof(char) * max_line_length);

  while (fgets(sentence, max_line_length, stdin) != NULL) {
    fprintf(stderr, "[INFO]: Parsing started.\n");
    words = sentence_splitter(sentence);

    bool typeOK = type_analyzer(words, dict);
    print_words(words, max_line_length);

    if (typeOK == true) {
      if (syntax_analyzer(words, syntax) == true) {
        fprintf(stderr, "[INFO]: \x1b[42mSentence OK\x1b[49m\x1b[32m (sentence is acceptable.)\x1b[39m\n");
      } else {
        fprintf(stderr, "\x1b[31m[WARN]: \x1b[39m\x1b[41mSentence NG\x1b[49m\x1b[31m (sentence is not acceptable.)\x1b[39m\n");
      }
      fprintf(stderr, "[INFO]: Parsing finished.\n");
    } else {
      fprintf(stderr, "\x1b[31m[WARN]: can't start syntax checking because of unknown words.\x1b[39m\n");
      fprintf(stderr, "\x1b[31m[WARN]: Parsing aborted.\x1b[39m\n");
    }

    free(words);
  }

  fprintf(stderr, "[INFO]: exiting...");
  fflush(stderr);

  /*
  // no need?
  free(sentence);
  free(syntax[0].current);
  free(syntax);
  free(dict[0].word);
  free(dict);
  */

  fprintf(stderr, " Goodbye.\n");

  return 0;
}
