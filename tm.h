#ifndef TM_H_INCLUDED
#define TM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "rand.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define DEFAULT_TAPE_GUARD            10
#define DEFAULT_TAPE_LENGTH           200000
#define DEFAULT_TAPE_INITIAL_POSITION 100000
#define MAXIMUM_MOVES                 3
#define MIN_DIST_TIME                 80

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct{
  uint32_t  min_position;
  uint32_t  max_position;
  uint32_t  guard;
  uint32_t  length;
  uint8_t   *string;    
  uint32_t  current_position;
  uint64_t  time;
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
  uint8_t   *out_string;
  uint8_t   *rev_map;
  uint32_t  counts[256];
  uint32_t  size;
  }
ALPHABET;

typedef struct{
  TAPE      *tape;
  RULE      **rules;
  ALPHABET  *alphabet;
  uint8_t   halt;
  uint8_t   *moves;
  uint32_t  alphabet_size;
  uint32_t  number_of_states;
  uint32_t  max_time;
  uint32_t  max_amplitude;
  uint32_t  min_amplitude;
  uint32_t  current_state;
  uint32_t  initial_state;
  int32_t   initial_position;
  double    max_distribution;
  uint32_t  rand_type;
  uint8_t   mode;
  }
TM;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint32_t    GetAmplitude         (TAPE *);
void        PrintTapeFile        (TM *, uint8_t *);
void        PrintActions         (TM *);
void        PrintAlphabet        (TM *);
void        PrintStates          (TM *);
void        PrintTM              (TM *);
void        PrintTape            (TM *);
void        PrintTapeInWritter   (TM *, FILE *);
void        PrintRulesInWritter  (TM *, FILE *);
void        PrintTapePres        (TM *, double);
TM          *CreateTM            (uint8_t, uint8_t *, uint32_t, uint32_t, 
		                  uint32_t, uint32_t, uint32_t, double, 
				  uint8_t, uint32_t, int32_t);
void        LoadTMRules          (TM *, uint8_t *);
void        LoadTMRandTape       (TM *, RAND *);
void        LoadTMTape           (TM *, uint8_t *);
void        ResetTape            (TM *);
uint8_t     RandFillTM           (TM *, RAND *);
uint8_t     UpdateTM             (TM *);
int8_t      EvalTapeDist         (uint32_t, TM *);
void        ResetDist            (TM *);
void        RemoveTM             (TM *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

