#ifndef TM_H_INCLUDED
#define TM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define DEFAULT_TAPE_GUARD            10
#define DEFAULT_TAPE_LENGTH           60000
#define DEFAULT_TAPE_INITIAL_POSITION 30000
#define MAXIMUM_MOVES                 3

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct{
  uint32_t  minimum_position;
  uint32_t  maximum_position;
  uint32_t  guard;
  uint32_t  length;
  uint8_t   *string;    
  uint32_t  current_position;
  uint64_t  steps;
  }
TAPE;

typedef struct{
  uint8_t   new_write;
  uint8_t   move;
  uint32_t  new_state;
  }
RULE;

typedef struct{
  uint8_t   *string;
  }
ALPHABET;

typedef struct{
  TAPE      *tape;
  RULE      **rules;
  ALPHABET  *alphabet;
  uint8_t   *moves;
  uint32_t  alphabet_size;
  uint32_t  number_of_states;
  uint32_t  maximum_steps;
  uint32_t  maximum_amplitude;
  uint32_t  minimum_amplitude;
  uint32_t  current_state;
  uint32_t  initial_state;
  uint8_t   mode;
  }
TM;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint32_t    GetAmplitude    (TAPE *);
void        PrintTapeFile   (TM *, uint8_t *);
void        PrintActions    (TM *);
void        PrintAlphabet   (TM *);
void        PrintStates     (TM *);
void        PrintTM         (TM *);
void        PrintTape       (TM *);
void        PrintTapePres   (TM *, double);
TM          *CreateTM       (uint32_t, uint32_t, uint32_t, uint32_t,
	                     uint32_t, uint8_t, uint32_t);
uint8_t     RandFillTM      (TM *);
uint8_t     UpdateTM        (TM *);
void        RemoveTM        (TM *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

