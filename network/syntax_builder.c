#include "global.h"


SYNTAX* build_syntax(const char *srcfile) {
  const int FGETS_MARGIN = 2; // do NOT decrease to < 2 (do not change)

  fprintf(stderr, "[INFO]: building syntax database...");
  fflush(stderr);

  FILE *fp;
  if ((fp = fopen(srcfile, "r")) == NULL) {
    fprintf(stderr, "\n[ERROR]: Failed to open syntax file.\n");
    exit(EXIT_FAILURE);
  }

  // get file size
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp) + FGETS_MARGIN;
  fseek(fp, 0, SEEK_SET);

  // checking csv and copying to memory
  char *bin = malloc(sizeof(char) * size);
  int count = 0;
  char *ptr;
  int remain;
  int entry = 0;
  char c;
  for (ptr = bin, remain = size; fgets(ptr, remain, fp) != NULL; /**/) {
    if (sscanf(ptr, " %*[^,],%*s%c%n", &c, &count) == 1) { // csv format checking
      entry++;
      // count = strlen(ptr);
      ptr[count - 1] = '\0'; // %c (CR or LF (or space?)) -> '\0'
      ptr += count;
      remain -= count;
    } else {
      ; // do nothing
    }
  }

  SYNTAX *syntax = malloc(sizeof(SYNTAX) * (entry + 1));
  int n[4] = {0};
  int i = 0;
  for (ptr = bin; sscanf(ptr, " %n%*[^,]%n,%n%*s%n", &n[0], &n[1], &n[2], &n[3]) != EOF; ptr += n[3] + 1) {
    // fprintf(stderr, "%d, %d, %d, %d\n", n[0], n[1], n[2], n[3]);
    syntax[i].current = ptr + n[0];
    syntax[i].next    = ptr + n[2];
    i++;
    ptr[n[1]] = '\0';
    ptr[n[3]] = '\0';
  }
  syntax[i] = DUMMY_SYNTAX;

  fclose(fp);
  fprintf(stderr, " done.\n");
  fprintf(stderr, "[INFO]: Syntax database has %d entries.\n", i);

  /*
  // write out to syntax.bin (for debug)
  if ((fp = fopen("syntax.bin", "wb")) == NULL) {
    fprintf(stderr, "\n[ERROR:] Failed to write syntax file.\n");
    exit(EXIT_FAILURE);
  }
  fwrite(syntax, sizeof(SYNTAX), entry + 1, fp);
  fwrite(bin, sizeof(char), size, fp);
  fclose(fp);
  */

  return syntax;
}
