#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool  int
#define true  1
#define false 0

#define MAX_TYPE 10
// parser can find up to MAX_TYPE trees

typedef struct {
  char *word;
  char *type[MAX_TYPE + 1];
} WORD;

typedef struct { // ex.) NP -> DET + NOUN
  char *lhs;  // left-hand side  (NP)
  char *rhs1; // right-hand side (DET)
  char *rhs2; // right-hand side (NOUN)
} SYNTAX;

typedef struct type {
  int index;
  char *type;
  struct cell *cell;
  struct type *prev_left_type;
  struct type *prev_right_type;
} TYPE;

typedef struct cell {
  int index;
  char *word;
  TYPE type[MAX_TYPE + 1]; // +1?: last element is dummy
} CELL;

extern WORD DUMMY;
extern SYNTAX DUMMY_SYNTAX;
extern CELL DUMMY_CELL;
extern int WORDAGE; // the number or words
extern int CELL_LENGTH; // the number or cells

bool type_analyzer(WORD *words, const WORD *dict);
CELL* syntax_analyzer(const WORD *words, const SYNTAX *syntax);
WORD* sentence_splitter(char *sentence);
WORD* build_dict(const char *srcfile);
SYNTAX* build_syntax(const char *srcfile);
void print_words(const WORD *words, const size_t buf_size, const int *type_num);
void print_sexps(const TYPE *type, int *type_num);

#endif
