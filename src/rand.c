#include <stdio.h>
#include <stdlib.h>
#include "rand.h"
#include "mem.h"
#include "misc.h"

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - C R E A T E   R A N D - - - - - - - - - - - - -  

RAND *CreateRand(uint8_t type){
  
  RAND *R = (RAND *) Calloc(1, sizeof(RAND));

  R->type   = type;
  R->open   = 1;
  R->number = 0;
  R->idx    = 0;
  R->source = NULL;
  
  if(R->type == 0){
    R->source = Fopen(RAND_DEV, "r");
    if(R->source == NULL){
      fprintf(stderr, "Error: unable to open %s\n", RAND_DEV);
      exit(1);
      }  
    }

  return R;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - G E T   R A N D - - - - - - - - - - - - - - -

uint32_t GetRandNumber(RAND *R){

  if(R->type == 0)
    {	  
    if(R->open != 1){
      fprintf(stderr, "Error: file was not opened\n");
      exit(1);
      }	  

    size_t bytes_read = fread(&R->number, sizeof(R->number), 1, R->source);
    }
  else
    {
    R->number = rand();
    }
  
  R->idx++;

  return R->number;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - R E M O V E   R A N D - - - - - - - - - - - - - 

void RemoveRand(RAND *R){

  if(R->type == 0) fclose(R->source);
  Free(R);

  return;
  }

////////////////////////////////////////////////////////////////////////////////

