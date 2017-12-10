#include "global.h"


SYNTAX* build_syntax(const char *srcfile) {
  const int FGETS_MARGIN = 2; // do NOT decrease to < 2 (do not change)

  fprintf(stderr, "[INFO]: building phrase structure rules database...");
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
    // csv format checking
    if (sscanf(ptr, " %*[^,],%*s%c%n", &c, &count)        == 1 || // a,b,
        sscanf(ptr, " %*[^,],%*[^,],%*s%c%n", &c, &count) == 1) { // a,b,c
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
  int n[6] = {0};
  int i = 0;
  ptr = bin;
  for (;;) { // infinite loop
    if (sscanf(ptr, " %n%*[^,]%n,%n%*[^,]%n,%n%*s%n", &n[0], &n[1], &n[2], &n[3], &n[4], &n[5]) != EOF) {
      // fprintf(stderr, "%d, %d, %d, %d\n", n[0], n[1], n[2], n[3], n[4], n[5]);
      syntax[i].lhs = ptr + n[0];
      syntax[i].rhs1 = ptr + n[2];
      syntax[i].rhs2 = ptr + n[4];
      i++;
      ptr[n[1]] = '\0';
      ptr[n[3]] = '\0';
      ptr[n[5]] = '\0';
      ptr += n[5] + 1;
    } else
    if (sscanf(ptr, " %n%*[^,]%n,%n%*s%n", &n[0], &n[1], &n[2], &n[3]) != EOF) {
      // fprintf(stderr, "%d, %d, %d, %d\n", n[0], n[1], n[2], n[3]);
      syntax[i].lhs = ptr + n[0];
      syntax[i].rhs1 = ptr + n[2];
      syntax[i].rhs2 = DUMMY_SYNTAX.rhs2;
      i++;
      ptr[n[1]] = '\0';
      ptr[n[3]] = '\0';
      if (ptr[n[3] - 1] == ',') {
        // remove comma (rhs1 may include comma at its tail)
        ptr[n[3] - 1] = '\0';
      }
      ptr += n[3] + 1;
    } else {
      break;
    }
  }
  syntax[i] = DUMMY_SYNTAX;

  fclose(fp);
  fprintf(stderr, " done.\n");
  fprintf(stderr, "[INFO]: Syntax database has %d phrase structure rules.\n", i);

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
