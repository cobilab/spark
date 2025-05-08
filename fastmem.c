#include "fastmem.h"
#include "defs.h"
#include <stdlib.h>
#include <math.h>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FAST_MEM *CreateFastMem(uint32_t size, uint32_t cardinality){

  FAST_MEM *FM = (FAST_MEM *) Calloc(1, sizeof(FAST_MEM));

  FM->length        = size;
  FM->idx           = (uint32_t *) Calloc(FM->length, sizeof(uint32_t));
  FM->sym           = (uint8_t  *) Calloc(FM->length, sizeof(uint8_t));
  FM->normalization = (double) FM->length * log2(cardinality);
  FM->pmodel        = CreatePModel(cardinality);

  return FM;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RemoveFastMem(FAST_MEM *FM){
  RemovePModel(FM->pmodel);
  free(FM->idx);
  free(FM->sym);
  free(FM);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
