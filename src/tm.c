#include <string.h>
#include "tm.h"
#include "mem.h"

// =============================================================================
// - - - - - - - - - - - C A L C U L A T E   A M P L I T U D E - - - - - - - - -

uint32_t GetAmplitude(TAPE *TP){
  return TP->maximum_position - TP->minimum_position;
  }

void PrintActions(TM *T){

  uint32_t x;

  fprintf(stderr, " Actions        : ");
  for(x = 0 ; x < MAXIMUM_MOVES ; ++x)
    if(x == MAXIMUM_MOVES-1) fprintf(stderr, " %c",  T->moves[x]);
    else                     fprintf(stderr, " %c,", T->moves[x]);
  fprintf(stderr, "\n");

  return;
  }

// =============================================================================
// - - - - - - - - - - - O U T P U T   T A P E   T O   F I L E - - - - - - - - -

void PrintTapeFile(TM *T, uint8_t *filename){
  
  uint32_t x;
  
  FILE *OUT = fopen(filename, "w");
  for(x = T->tape->minimum_position ; x < T->tape->maximum_position ; ++x)
    fprintf(OUT, "%c", T->alphabet->string[T->tape->string[x]]);
  fclose(OUT);
  
  return;
  }

// =============================================================================
// - - - - - - - - - - - - - P R I N T   A L P H A B E T - - - - - - - - - - - -

void PrintAlphabet(TM *T){

  uint32_t x;

  fprintf(stderr, " Alphabet       : ");
  for(x = 0 ; x < T->alphabet_size ; ++x)
    if(x == T->alphabet_size-1) fprintf(stderr, " %c",  T->alphabet->string[x]);
    else                        fprintf(stderr, " %c,", T->alphabet->string[x]);
  fprintf(stderr, "\n");

  return;
  }

// =============================================================================
// - - - - - - - - - - - - - - P R I N T   S T A T E S - - - - - - - - - - - - -

void PrintStates(TM *T){

  uint32_t x;

  fprintf(stderr, " States         : ");
  for(x = 0 ; x < T->number_of_states ; ++x)
    if(x == T->number_of_states-1) fprintf(stderr, " %u",  x);
    else                           fprintf(stderr, " %u,", x);
  fprintf(stderr, "\n");

  return;
  }

// =============================================================================
// - - - - - - - - - - - - - - - P R I N T   T A P E - - - - - - - - - - - - - -

void PrintTape(TM *T){

  int x;

  for(x = T->tape->minimum_position ; x < T->tape->maximum_position ; ++x)
    fprintf(stderr, "%c", T->alphabet->string[T->tape->string[x]]);
  fprintf(stderr, "\n");

  return;
  }

// =============================================================================
// - - - - - - P R I N T   T A P E   F O R   P R E S E N T A T I O N - - - - - -

void PrintTapePres(TM *T, double delay){

  int x;

  usleep(delay);

  fprintf(stderr, " Step [%u] -> Tape: ", T->tape->steps);
  for(x = T->tape->minimum_position ; x < T->tape->maximum_position ; ++x)
    fprintf(stderr, "%c", T->alphabet->string[T->tape->string[x]]);
  fprintf(stderr, "\r");

  return;
  }

// =============================================================================
// - - - - - - - - - - - - - - - - P R I N T   T M - - - - - - - - - - - - - - -

void PrintTM(TM *T){

  int x, y;
    
  fprintf(stderr, "\n");
  fprintf(stderr, "  \t");
  for(x = 0 ; x < T->number_of_states ; ++x)
    fprintf(stderr, "  %2u     ", x);
  fprintf(stderr, "\n");

  for(x = 0 ; x < T->alphabet_size ; ++x){
    fprintf(stderr, "%2c\t", T->alphabet->string[x]);
    for(y = 0 ; y < T->number_of_states ; ++y){
      fprintf(stderr, "%2c %c %-2u  ", T->alphabet->string[T->rules[x][y].new_write], 
      T->rules[x][y].move, T->rules[x][y].new_state);
      }
    fprintf(stderr, "\n");
    }

  return;
  }

// =============================================================================
// - - - - - - - - - - - - - - - C R E A T E   T M - - - - - - - - - - - - - - -

TM *CreateTM(uint32_t alphabet_size, uint32_t number_of_states, 
             uint32_t maximum_steps, uint32_t maximum_amplitude, 
	 uint32_t minimum_amplitude, uint8_t mode, uint32_t initial_state){
  
  uint32_t x;

  TM *T = (TM *) Calloc(1, sizeof(TM));

  T->alphabet_size     = alphabet_size;
  T->number_of_states  = number_of_states;
  T->maximum_steps     = maximum_steps;
  T->maximum_amplitude = maximum_amplitude;
  T->minimum_amplitude = minimum_amplitude;
  T->initial_state     = initial_state;
  T->current_state     = T->initial_state;
  T->mode              = mode;
  T->moves             = (uint8_t *) Calloc(MAXIMUM_MOVES + 1, sizeof(uint8_t));
  T->moves[0]          = '<';
  T->moves[1]          = '>';
  T->moves[2]          = '=';

  T->rules = (RULE **) Calloc(T->alphabet_size, sizeof(RULE *));
  for(x = 0 ; x < T->alphabet_size ; ++x)
    T->rules[x] = (RULE *) Calloc(T->number_of_states, sizeof(RULE));

  T->tape = (TAPE *) Calloc(1, sizeof(TAPE));

  assert(T->maximum_steps > T->tape->guard);

  T->tape->steps            = 0;
  T->tape->guard            = DEFAULT_TAPE_GUARD;
  T->tape->length           = T->maximum_steps * 2 + 1;
  T->tape->current_position = T->maximum_steps + 1;
  T->tape->minimum_position = T->tape->current_position - T->tape->guard;
  T->tape->maximum_position = T->tape->current_position + T->tape->guard;
  T->tape->string           = (uint8_t *) Calloc(T->tape->length + T->tape->guard, 
		              sizeof(uint8_t));

  T->alphabet = (ALPHABET *) Calloc(1, sizeof(ALPHABET));
  
  T->alphabet->string = (uint8_t *) Calloc(256, sizeof(uint8_t));
  for(x = 0 ; x < T->alphabet_size ; ++x)
    T->alphabet->string[x] = (uint8_t) x + 'A';
  
  return T;
  }

// =============================================================================
// - - - - - - - - - - - - - - - U P D A T E   T M - - - - - - - - - - - - - - -

uint8_t UpdateTM(TM *T){

  if(T->tape->steps > T->maximum_steps || 
     T->maximum_amplitude < GetAmplitude(T->tape))
    return 1;
  else if(T->tape->current_position == T->tape->length || 
	  T->tape->current_position == 0)
    return 2;

  RULE *R = &T->rules[T->tape->string[T->tape->current_position]][T->current_state];

  T->tape->string[T->tape->current_position] = R->new_write;
  
  switch(R->move){
    case '<': T->tape->current_position--; break;
    case '>': T->tape->current_position++; break;
    case '=': break;
    default: fprintf(stderr, "Error: TM moves restricted to {<, >, =}\n"); 
    exit(1);
    }

  T->current_state = R->new_state;

  if(T->tape->current_position > T->tape->maximum_position)
    T->tape->maximum_position = T->tape->current_position;

  if(T->tape->current_position < T->tape->minimum_position)
    T->tape->minimum_position = T->tape->current_position;

  T->tape->steps++;

  return 0;
  }

// =============================================================================
// - - - - - - - - - - - - - R A N D O M   F I L L   T M - - - - - - - - - - - -

uint8_t RandFillTM(TM *T){

  uint32_t x, y;

  for(x = 0 ; x < T->alphabet_size ; ++x)
    for(y = 0 ; y < T->number_of_states ; ++y){
      T->rules[x][y].new_write = rand() % T->alphabet_size;
      T->rules[x][y].move      = T->moves[rand() % MAXIMUM_MOVES];
      T->rules[x][y].new_state = rand() % T->number_of_states;
      }

  T->tape->guard            = DEFAULT_TAPE_GUARD;
  T->tape->length           = T->maximum_steps * 2 + 1;
  T->tape->current_position = T->maximum_steps + 1;
  T->tape->minimum_position = T->tape->current_position - T->tape->guard;
  T->tape->maximum_position = T->tape->current_position + T->tape->guard;
  T->tape->steps            = 0;

  memset((void *) T->tape->string, 0, T->tape->length + 
		  T->tape->guard * sizeof(char));

  return 0;
  }

// =============================================================================
// - - - - - - - - - - - - - - - - R E M O V E   T M - - - - - - - - - - - - - -

void RemoveTM(TM *T){

  uint32_t x;

  for(x = 0 ; x < T->alphabet_size ; ++x)
    Free(T->rules[x]);
  Free(T->rules);
  Free(T->tape->string);
  Free(T->tape);
  Free(T->moves);
  Free(T);

  return;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
