#ifndef FASTMEM_H_INCLUDED
#define FASTMEM_H_INCLUDED

#include <stdlib.h>
#include <limits.h>
#include "defs.h"
#include "mem.h"
#include "pmodels.h"

typedef struct{
  uint32_t  length;
  uint32_t  *idx;
  uint8_t   *sym;
  double    normalization;
  PModel    *pmodel;
  }
FAST_MEM;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FAST_MEM    *CreateFastMem   (uint32_t, uint32_t);
void        RemoveFastMem    (FAST_MEM *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
#endif
