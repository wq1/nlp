#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool  int
#define true  1
#define false 0

typedef struct {
  char *word;
  char *type;
} WORD;

typedef struct {
  char *current;
  char *next;
} SYNTAX;

extern WORD START;
extern WORD DUMMY;
extern SYNTAX DUMMY_SYNTAX;

bool type_analyzer(WORD *words, const WORD *dict);
bool syntax_analyzer(const WORD *words, const SYNTAX *syntax);
WORD* sentence_splitter(char *sentence);
WORD* build_dict(const char *srcfile);
SYNTAX* build_syntax(const char *srcfile);
void print_words(const WORD *words, const size_t buf_size);

#endif
