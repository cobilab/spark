#ifndef RAND_H_INCLUDED
#define RAND_H_INCLUDED

#include <stdlib.h>
#include <limits.h>
#include "defs.h"

#define RAND_DEV "/dev/urandom"

typedef struct{
  uint8_t   open;
  uint32_t  number;
  uint32_t  idx;
  FILE      *source;
  }
RAND;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RAND        *CreateRand    (void);
uint32_t    GetRandNumber  (RAND *);
void        RemoveRand     (RAND *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
#endif
