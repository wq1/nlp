#include "global.h"

// global variables
int CELL_LENGTH;


int getIndex(const int x, const int y) {
// this method for upper triangular matrix
  int index = 0;
  int column = 0;
  int i, j;
  for (i = 0, j = 0; i < WORDAGE && j < WORDAGE; /* */) {
    if (i == x && j == y) {
      return index;
    }

    if (j == WORDAGE - 1) {
      i = 0;
      j = ++column;
    } else {
      i++;
      j++;
    }
    index++;
  }

  return -1;
}


bool setNewArrayIfNeeded(TYPE **type, int *type_index, int *array_size, const int *k) {
  if (*type_index >= (*array_size - 1)) { // -1?: last element is preserved for dummy element.
    *array_size *= 2; // new array will be expanded.
    fprintf(stderr, "[DEBUG]: cells[index].type is full. increasing array size to %d elements.\n", *array_size);
    if (*k == *type_index) {
      free(*type);
    }
    *type = malloc(sizeof(TYPE) * *array_size);

    int i;
    for (i = 0; i < *array_size; i++) { // init type[] with DUMMY_CELL.type[0]
      memcpy(&(*type)[i], &DUMMY_CELL.type[0], sizeof(TYPE));
    }

    *type_index = 0;
    return true;
  } else {
    return false;
  }
}


CELL* syntax_analyzer(const WORD *words, const SYNTAX *syntax) {
  fprintf(stderr, "[INFO]: CYK parser started.\n");

  // memory allocation
  CELL_LENGTH = (WORDAGE * (WORDAGE + 1)) / 2; // upper triangular matrix (n*(n+1)/2)
  if (CELL_LENGTH == 0) {
    fprintf(stderr, "\x1b[31m[WARN]: CYK parser stopped (word not found).\x1b[39m\n");
    return NULL;
  }
  CELL *cells = malloc(sizeof(CELL) * CELL_LENGTH);

  int index;

  // init cells
  for (index = 0; index < CELL_LENGTH; index++) {
    cells[index] = DUMMY_CELL; // for unused-member initializing
    cells[index].index = index;
  }

  int i, j; // for loop
  int k; // cells[index].type[k]
  int p, q, a; // comparison cells[a] (x, y) = (p, q)
  int u, v, b; // comparison cells[b] (x, y) = (u, v)
  int l, m, n; // for loop
  int column = 0;
  index = 0;
  int array_size = DEFAULT_LENGTH_OF_ARRAY;
  TYPE *type = malloc(sizeof(TYPE) * array_size);
  for (i = 0; i < array_size; i++) { // init type[] with DUMMY_CELL.type[0]
    memcpy(&type[i], &DUMMY_CELL.type[0], sizeof(TYPE));
  }
  int type_index = 0;
  for (i = 0, j = column; i < WORDAGE && j < WORDAGE; /* */) {
    outer: // outer loop
    cells[index].type = &type[type_index];
    if (type_index == 0) {
      cells[index].type_is_new_array = true;
    }
    k = 0;

    // (cells[index].word <= words[index].word)
    if (index < WORDAGE) {
      cells[index].word = words[index].word;
      for (i = 0; words[index].type[i] != DUMMY.type[0]; i++) {
        if (setNewArrayIfNeeded(&type, &type_index, &array_size, &k) == true) {
          goto outer; // continue from outer loop
        }

        cells[index].type[k].index = k;
        cells[index].type[k].type = words[index].type[i];
        cells[index].type[k].cell = &cells[index];
        fprintf(stderr, "[DEBUG]: %s%d \"%s\"\n", cells[index].type[k].type, cells[index].index + 1, cells[index].word);
        k++;
        type_index++;
      }
    }

    // ex.) S -> NP + VP; VP -> VP + NP
    p = i;
    v = j;
    for (q = i, u = i + 1; q < j && u <= j; q++, u++) {
      a = getIndex(p, q);
      b = getIndex(u, v);
      // fprintf(stderr, "[DEBUG]: %d: (%d, %d) = %d, (%d, %d) = %d\n", index+1, p+1, q+1, a+1, u+1, v+1, b+1);

      for (l = 0; cells[a].type[l].type != DUMMY_CELL.type[0].type; l++) {
        for (m = 0; cells[b].type[m].type != DUMMY_CELL.type[0].type; m++) {
          for (n = 0; syntax[n].lhs != DUMMY_SYNTAX.lhs; n++) {
            if (strcmp(cells[a].type[l].type, syntax[n].rhs1) == 0 && strcmp(cells[b].type[m].type, syntax[n].rhs2) == 0) {
              if (setNewArrayIfNeeded(&type, &type_index, &array_size, &k) == true) {
                goto outer; // continue from outer loop
              }

              cells[index].type[k].index = k;
              cells[index].type[k].type = syntax[n].lhs;
              cells[index].type[k].cell = &cells[index];
              cells[index].type[k].prev_left_type = &cells[a].type[l];
              cells[index].type[k].prev_right_type = &cells[b].type[m];
              fprintf(stderr, "[DEBUG]: %s%d -> %s%d + %s%d\n",
                cells[index].type[k].type, cells[index].type[k].cell->index + 1,
                cells[index].type[k].prev_left_type->type, cells[index].type[k].prev_left_type->cell->index + 1,
                cells[index].type[k].prev_right_type->type, cells[index].type[k].prev_right_type->cell->index + 1
              );
              k++;
              type_index++;
            }
          }
        }
      }
    }

    // self-referencing (ex.) VP -> VERB; S -> VP
    for (l = 0; cells[index].type[l].type != DUMMY_CELL.type[0].type; l++) {
      for (n = 0; (syntax[n].lhs != DUMMY_SYNTAX.lhs); n++) {
        if (strcmp(cells[index].type[l].type, syntax[n].rhs1) == 0 && syntax[n].rhs2 == DUMMY_SYNTAX.rhs2) {
          if (setNewArrayIfNeeded(&type, &type_index, &array_size, &k) == true) {
            goto outer; // continue from outer loop
          }

          cells[index].type[k].index = k;
          cells[index].type[k].type = syntax[n].lhs;
          cells[index].type[k].cell = &cells[index];
          cells[index].type[k].prev_left_type = &cells[index].type[l];
          fprintf(stderr, "[DEBUG]: %s%d -> %s%d\n",
            cells[index].type[k].type, cells[index].type[k].cell->index + 1,
            cells[index].type[k].prev_left_type->type, cells[index].type[k].prev_left_type->cell->index + 1
          );
          k++;
          type_index++;
        }
      }
    }

    if (j == WORDAGE - 1) {
      i = 0;
      j = ++column;
    } else {
      i++;
      j++;
    }

    if (k == 0) { // when k == 0, the cell is empty.
      if (type_index == 0) { // type_index == 0 means the head of type[].
        type_index++;
      } else {
        cells[index].type--; // cells[index].type points last dummy element of previous cell (cells[index - 1])
      }                      //   to save memory resource
    } else {
      type_index++;  // make last element dummy
    }

    setNewArrayIfNeeded(&type, &type_index, &array_size, &k);
    index++;
  }

  fprintf(stderr, "[INFO]: CYK parser stopped.\n");
  return cells;
}
