#include <string.h>
#include "tm.h"
#include "mem.h"

// =============================================================================
// - - - - - - - - - - - C A L C U L A T E   A M P L I T U D E - - - - - - - - -

uint32_t GetAmplitude(TAPE *TP){
  return TP->maximum_position - TP->minimum_position + 1;
  }

// =============================================================================
// - - - - - - - - - - - - - - P R I N T   A C T I O N S - - - - - - - - - - - -

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
// - - - - - - - - - - - - - P R I N T   A L P H A B E T - - - - - - - - - - - -

void PrintAlphabet(TM *T){

  uint32_t x;

  fprintf(stderr, " Alphabet       : ");
  for(x = 0 ; x < T->alphabet_size ; ++x)
    if(x == T->alphabet_size-1) 
      fprintf(stderr, " %c",  T->alphabet->out_string[x]);
    else 
      fprintf(stderr, " %c,", T->alphabet->out_string[x]);
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
// - - - - - - - - - - - O U T P U T   T A P E   T O   F I L E - - - - - - - - -

void PrintTapeFile(TM *T, uint8_t *filename){

  uint32_t x;

  FILE *OUT = fopen(filename, "w");
  for(x = T->tape->minimum_position ; x < T->tape->maximum_position ; ++x)
    fprintf(OUT, "%c", T->alphabet->out_string[T->tape->string[x]]);
  fprintf(OUT, "\n");
  fclose(OUT);

  return;
  }

// =============================================================================
// - - - - - - - - - P R I N T   T A P E   I N   W R I T T E R - - - - - - - - - 

void PrintTapeInWritter(TM *T, FILE *F){

  uint32_t x;

  for(x = T->tape->minimum_position ; x < T->tape->maximum_position ; ++x)
    fprintf(F, "%c", T->alphabet->out_string[T->tape->string[x]]);
  fprintf(F, "\n");

  return;
  }

// =============================================================================
// - - - - - - - - - P R I N T   R U L E S   I N   W R I T T E R - - - - - - - -

void PrintRulesInWritter(TM *T, FILE *F){

  uint32_t x, y;

  for(x = 0 ; x < T->alphabet_size ; ++x){
    for(y = 0 ; y < T->number_of_states ; ++y){
      fprintf(F, "%u%c%u,",
      T->alphabet->string[T->rules[x][y].new_write],
      T->rules[x][y].move, T->rules[x][y].new_state);
      }
    }

  fprintf(F, "\t");

  return;
  }

// =============================================================================
// - - - - - - - - - - - - L O A D   R U L E S   I N   T M - - - - - - - - - - -

void LoadTMRules(TM *T, uint8_t *name){

  uint32_t n, x = 0, y = 0, idx = 0, new_write, new_state;
  uint8_t c, rule[20], move;

  FILE *F = fopen(name, "r");
 
  while((c = fgetc(F)) != EOF && c != '\n' && c != ' '){
    rule[idx++] = c;
    if(c == ','){  
      rule[idx] = '\0';
      idx = 0;
      if(sscanf(rule, "%u%c%u,", &new_write, &move, &new_state) == 3){
        T->rules[x][y].new_write = new_write;
        T->rules[x][y].move      = move;
        T->rules[x][y].new_state = new_state;
        if(++y == T->number_of_states){
          y = 0;	
	  ++x;
          }
        }
      }
    }

  fclose(F);
  
  return;
  }

// =============================================================================
// - - - - - - - - - - - - - - - P R I N T   T A P E - - - - - - - - - - - - - -

void PrintTape(TM *T){

  int x;

  for(x = T->tape->minimum_position ; x < T->tape->maximum_position ; ++x)
    fprintf(stderr, "%c", T->alphabet->out_string[T->tape->string[x]]);
  fprintf(stderr, "\n");

  return;
  }

// =============================================================================
// - - - - - - P R I N T   T A P E   F O R   P R E S E N T A T I O N - - - - - -

void PrintTapePres(TM *T, double delay){

  int x;

  usleep(delay);

  fprintf(stderr, " Time [%u] | Tape: ", T->tape->time);
  for(x = T->tape->minimum_position ; x < T->tape->maximum_position ; ++x)
    fprintf(stderr, "%c", T->alphabet->out_string[T->tape->string[x]]);
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
    fprintf(stderr, "%2c\t", T->alphabet->out_string[x]);
    for(y = 0 ; y < T->number_of_states ; ++y){
      fprintf(stderr, "%2c %c %-2u  ", 
      T->alphabet->out_string[T->rules[x][y].new_write], 
      T->rules[x][y].move, T->rules[x][y].new_state);
      }
    fprintf(stderr, "\n");
    }

  return;
  }

// =============================================================================
// - - - - - - - - - - - - - - - C R E A T E   T M - - - - - - - - - - - - - - -

TM *CreateTM(uint8_t *alphabet, uint32_t alphabet_size, uint32_t 
  number_of_states, uint32_t maximum_time, uint32_t maximum_amplitude, 
  uint32_t minimum_amplitude, uint8_t mode, uint32_t initial_state){
  
  uint32_t x;

  TM *T = (TM *) Calloc(1, sizeof(TM));

  T->alphabet_size     = alphabet_size;
  T->number_of_states  = number_of_states;
  T->maximum_time      = maximum_time;
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

  assert(T->maximum_time > T->tape->guard);

  T->tape->time            = 0;
  T->tape->guard            = DEFAULT_TAPE_GUARD;
  T->tape->length           = T->maximum_time * 2 + 1;
  T->tape->current_position = T->maximum_time + 1;
  T->tape->minimum_position = T->tape->current_position - T->tape->guard;
  T->tape->maximum_position = T->tape->current_position + T->tape->guard;
  T->tape->string           = (uint8_t *) Calloc(T->tape->length + T->tape->guard, 
		              sizeof(uint8_t));

  T->alphabet = (ALPHABET *) Calloc(1, sizeof(ALPHABET));

  T->alphabet->string     = (uint8_t *) Calloc(256, sizeof(uint8_t));
  T->alphabet->out_string = (uint8_t *) Calloc(256, sizeof(uint8_t));
  
  for(x = 0 ; x < T->alphabet_size ; ++x)
    T->alphabet->string[x] = (uint8_t) x;
  
  if(!strcmp(alphabet, "-")){
    for(x = 0 ; x < T->alphabet_size ; ++x)
      T->alphabet->out_string[x] = (uint8_t) x + 'A';
    }
  else{
    if(strlen(alphabet) < T->alphabet_size){
      fprintf(stderr, "Error: alphabet is smaller than the alphabet size!\n");
      exit(1);
      }
    for(x = 0 ; x < T->alphabet_size ; ++x)
      T->alphabet->out_string[x] = (uint8_t) alphabet[x];
    }
  
  return T;
  }

// =============================================================================
// - - - - - - - - - - - - - - - U P D A T E   T M - - - - - - - - - - - - - - -

uint8_t UpdateTM(TM *T){

  if(T->tape->time > T->maximum_time || 
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

  T->tape->time++;

  return 0;
  }

// =============================================================================
// - - - - - - - - - - - - - R A N D O M   F I L L   T M - - - - - - - - - - - -

uint8_t RandFillTM(TM *T, RAND *R){

  uint32_t x, y;

  for(x = 0 ; x < T->alphabet_size ; ++x)
    for(y = 0 ; y < T->number_of_states ; ++y){
      T->rules[x][y].new_write = GetRandNumber(R) % T->alphabet_size;
      T->rules[x][y].move      = T->moves[GetRandNumber(R) % MAXIMUM_MOVES];
      T->rules[x][y].new_state = GetRandNumber(R) % T->number_of_states;
      }

  T->tape->guard            = DEFAULT_TAPE_GUARD;
  T->tape->length           = T->maximum_time * 2 + 1;
  T->tape->current_position = T->maximum_time + 1;
  T->tape->minimum_position = T->tape->current_position - T->tape->guard;
  T->tape->maximum_position = T->tape->current_position + T->tape->guard;
  T->tape->time             = 0;

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
