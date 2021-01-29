#include <stdio.h>
#include <stdlib.h>
#include "misc.h"

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - F L O G 2 - - - - - - - - - - - - - - - - -

uint32_t FLog2(uint64_t i){

  uint32_t n, m, k = 32, o = (i & (i - 1)) ? 1 : 0;
  static const uint64_t sizes[6] =
  { 0x0000000000000002ull, 0x000000000000000Cull, 0x00000000000000F0ull,
    0x000000000000FF00ull, 0x00000000FFFF0000ull, 0xFFFFFFFF00000000ull };

  for(n = 6 ; n-- ; ){
    o += (m = (i & *(sizes+n)) ? k : 0);
    i >>= m;
    k >>= 1;
    }

  return o;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - N B Y T E S  I N  F I L E - - - - - - - - - - - -

uint64_t NBytesInFile(uint8_t *name){
  uint64_t size = 0;
  FILE *F = Fopen(name, "r");
  fseeko(F, 0, SEEK_END);
  size = ftello(F);
  fclose(F);
  return size;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - F O P E N - - - - - - - - - - - - - - - - -

FILE *Fopen(const char *path, const char *mode){

  FILE *file = fopen(path, mode);

  if(file == NULL){
    fprintf(stderr, "Error: opening: %s (mode %s).\nDoes the file exist?\n",
    path, mode);
    exit(1);
    }

  return file;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

