#include <string.h>
#include "tm.h"
#include "colors.h"
#include "mem.h"
#include "misc.h"

// =============================================================================
// - - - - - - - - - - - C A L C U L A T E   A M P L I T U D E - - - - - - - - -

uint32_t GetAmplitude(TAPE *TP){
  return TP->max_position - TP->min_position + 1;
  }

// =============================================================================
// - - - - - - - - - - - - - - P R I N T   A C T I O N S - - - - - - - - - - - -

void PrintActions(TM *T){

  fprintf(stderr, "Actions        : ");
  for(uint32_t x = 0 ; x < MAXIMUM_MOVES ; ++x)
    if(x == MAXIMUM_MOVES-1 && !T->halt) 
      fprintf(stderr, " %c",  T->moves[x]);
    else 
      fprintf(stderr, " %c,", T->moves[x]);
  if(T->halt)
    fprintf(stderr, " %c", T->moves[3]);
  fprintf(stderr, "\n");

  return;
  }

// =============================================================================
// - - - - - - - - - - - - - P R I N T   A L P H A B E T - - - - - - - - - - - -

void PrintAlphabet(TM *T){

  fprintf(stderr, "Alphabet       : ");
  for(uint32_t x = 0 ; x < T->alphabet_size ; ++x)
    if(x == T->alphabet_size-1) 
      fprintf(stderr, " %s%c%s", colors[x % MAX_COLOR],
      T->alphabet->out_string[x], normal_color);
    else 
      fprintf(stderr, " %s%c%s,", colors[x % MAX_COLOR], 
      T->alphabet->out_string[x], normal_color);
  fprintf(stderr, "\n");

  return;
  }

// =============================================================================
// - - - - - - - - - - - - - - P R I N T   S T A T E S - - - - - - - - - - - - -

void PrintStates(TM *T){

  fprintf(stderr, "States         : ");
  for(uint32_t x = 0 ; x < T->number_of_states ; ++x)
    if(x == T->number_of_states-1) fprintf(stderr, " %u",  x);
    else                           fprintf(stderr, " %u,", x);
  fprintf(stderr, "\n");

  return;
  }

// =============================================================================
// - - - - - - - - - - - O U T P U T   T A P E   T O   F I L E - - - - - - - - -

void PrintTapeFile(TM *T, uint8_t *filename){

  FILE *OUT = Fopen(filename, "w");
  for(uint32_t x = T->tape->min_position ; x < T->tape->max_position ; ++x)
    fprintf(OUT, "%c", T->alphabet->out_string[T->tape->string[x]]);
  fprintf(OUT, "\n");
  fclose(OUT);

  return;
  }

// =============================================================================
// - - - - - - - - - P R I N T   T A P E   I N   W R I T T E R - - - - - - - - - 

void PrintTapeInWritter(TM *T, FILE *F){

  for(uint32_t x = T->tape->min_position ; x < T->tape->max_position ; ++x)
    fprintf(F, "%c", T->alphabet->out_string[T->tape->string[x]]);
  fprintf(F, "\t");

  return;
  }

// =============================================================================
// - - - - - - - - - P R I N T   R U L E S   I N   W R I T T E R - - - - - - - -

void PrintRulesInWritter(TM *T, FILE *F){

  for(uint32_t x = 0 ; x < T->alphabet_size ; ++x)
    for(uint32_t y = 0 ; y < T->number_of_states ; ++y){
      fprintf(F, "%u%c%u,",
      T->alphabet->string[T->rules[x][y].new_write],
      T->rules[x][y].move, T->rules[x][y].new_state);
      }

  fprintf(F, "\t");

  return;
  }

// =============================================================================
// - - - - - - - - - - - - L O A D   R U L E S   I N   T M - - - - - - - - - - -

void LoadTMRules(TM *T, uint8_t *name){

  uint32_t n, x = 0, y = 0, idx = 0, new_write, new_state;
  uint8_t c, rule[20], move;

  FILE *F = Fopen(name, "r");
 
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
// - - - - - - - - - - - - L O A D   R A N D O M   T A P E - - - - - - - - - - -

void LoadTMRandTape(TM *T, RAND *R){
  
  for(uint32_t x = 0 ; x < T->tape->length ; ++x){
    T->tape->string[x] = GetRandNumber(R) % T->alphabet_size;
    }

  return;
  }

// =============================================================================
// - - - - - - - - - - - - L O A D   F I L E   I N   T A P E - - - - - - - - - -

void LoadTMTape(TM *T, uint8_t *name){

  uint32_t length = 0, x;
  int c, in = 1;

  FILE *F = Fopen(name, "r");
  
  while((c = fgetc(F)) != EOF && c != '\n' && c != ' ')
    ++length;

  rewind(F);

  if(length >= T->tape->length){
    fprintf(stderr, "Error: tape is longer than %u\n", 
    T->tape->max_position);
    exit(1);
    }

  while((c = fgetc(F)) != EOF && c != '\n' && c != ' '){
    in = 0;
    for(x = 0 ; x < T->alphabet->size ; ++x){
      if(T->alphabet->out_string[x] == c){
	in = 1;
	break;
        }
      }
    if(in == 1){
        
      if(T->tape->current_position > T->tape->max_position)
        T->tape->max_position = T->tape->current_position;

      if(T->tape->current_position < T->tape->min_position)
        T->tape->min_position = T->tape->current_position;
      
      ++T->tape->max_position;
      ++T->tape->current_position;
     
      T->tape->string[T->tape->current_position] = T->alphabet->rev_map[c];
      }
    else{ 
      fprintf(stderr, "Warning: input tape symbol outside the alphabet!\n");
      fprintf(stderr, "Note: this outside symbol will be ignored!\n");
      }
    }

  fclose(F);

  return;
  }

// =============================================================================
// - - - - - - - - - - - - - - - P R I N T   T A P E - - - - - - - - - - - - - -

void PrintTape(TM *T){

  for(uint32_t x = T->tape->min_position ; x < T->tape->max_position ; ++x)
    fprintf(stderr, "%c", T->alphabet->out_string[T->tape->string[x]]);
  fprintf(stderr, "\n");

  return;
  }

// =============================================================================
// - - - - - - P R I N T   T A P E   F O R   P R E S E N T A T I O N - - - - - -

void PrintTapePres(TM *T, double delay){

  usleep(delay);

  fprintf(stderr, "Time [%lu] | Tape: ", T->tape->time);
  for(uint32_t x = T->tape->min_position ; x < T->tape->max_position ; ++x)
    fprintf(stderr, "%s%c%s", colors[T->tape->string[x] % MAX_COLOR], 
    T->alphabet->out_string[T->tape->string[x]], normal_color);
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
    fprintf(stderr, " %s%c%s\t", colors[x % MAX_COLOR], 
    T->alphabet->out_string[x], normal_color);
    for(y = 0 ; y < T->number_of_states ; ++y){
      fprintf(stderr, " %s%c%s %c %-2u  ", 
      colors[T->rules[x][y].new_write % MAX_COLOR],
      T->alphabet->out_string[T->rules[x][y].new_write], normal_color,
      T->rules[x][y].move, T->rules[x][y].new_state);
      }
    fprintf(stderr, "\n");
    }

  return;
  }

// =============================================================================
// - - - - - - - - - - - - - - - C R E A T E   T M - - - - - - - - - - - - - - -

TM *CreateTM(uint8_t halt, uint8_t *alphabet, uint32_t alphabet_size, uint32_t 
  number_of_states, uint32_t max_time, uint32_t max_amplitude, uint32_t 
  min_amplitude, double max_distribution, uint8_t mode, uint32_t initial_state, 
  int32_t initial_position){
  
  uint32_t x;

  TM *T = (TM *) Calloc(1, sizeof(TM));

  T->halt              = halt;
  T->alphabet_size     = alphabet_size;
  T->number_of_states  = number_of_states;
  T->max_time          = max_time;
  T->max_amplitude     = max_amplitude;
  T->min_amplitude     = min_amplitude;
  T->max_distribution  = max_distribution;
  T->initial_state     = initial_state;
  T->current_state     = T->initial_state;
  T->initial_position  = initial_position;
  T->mode              = mode;
  T->moves             = (uint8_t *) Calloc(MAXIMUM_MOVES + 2, sizeof(uint8_t));
  T->moves[0]          = '<';
  T->moves[1]          = '>';
  T->moves[2]          = '=';
  T->moves[3]          = '.';

  T->rules = (RULE **) Calloc(T->alphabet_size, sizeof(RULE *));
  for(x = 0 ; x < T->alphabet_size ; ++x)
    T->rules[x] = (RULE *) Calloc(T->number_of_states, sizeof(RULE));

  T->tape = (TAPE *) Calloc(1, sizeof(TAPE));

  assert(T->max_time > T->tape->guard);

  T->tape->time             = 0;
  T->tape->guard            = DEFAULT_TAPE_GUARD;
  T->tape->length           = T->max_time * 2 + 1;
  T->tape->current_position = T->max_time + 1;
  T->tape->min_position     = T->tape->current_position - T->tape->guard;
  T->tape->max_position     = T->tape->current_position + T->tape->guard;

  T->tape->string           = (uint8_t *) Calloc((T->tape->length + 
		              2 * T->tape->guard), sizeof(uint8_t));

  T->alphabet = (ALPHABET *) Calloc(1, sizeof(ALPHABET));

  T->alphabet->string     = (uint8_t *) Calloc(256, sizeof(uint8_t));
  T->alphabet->out_string = (uint8_t *) Calloc(256, sizeof(uint8_t));
  T->alphabet->rev_map    = (uint8_t *) Calloc(256, sizeof(uint8_t));
  
  T->alphabet->size = T->alphabet_size;
  
  for(x = 0 ; x < T->alphabet_size ; ++x)
    T->alphabet->string[x] = (uint8_t) x;
  
  if(!strcmp(alphabet, "-")){
    for(x = 0 ; x < T->alphabet_size ; ++x){
      T->alphabet->out_string[x] = (uint8_t) x + 'A';
      T->alphabet->rev_map[T->alphabet->out_string[x]] = x;
      }
    }
  else{
    if(strlen(alphabet) < T->alphabet_size){
      fprintf(stderr, "Error: alphabet is smaller than the alphabet size!\n");
      exit(1);
      }
    for(x = 0 ; x < T->alphabet_size ; ++x){
      T->alphabet->out_string[x] = (uint8_t) alphabet[x];
      T->alphabet->rev_map[T->alphabet->out_string[x]] = x;
      }
    }
  
  return T;
  }

// =============================================================================
// - - - - - - - - - - - - - - - U P D A T E   T M - - - - - - - - - - - - - - -

uint8_t UpdateTM(TM *T){

  if(T->tape->time > T->max_time || T->max_amplitude < GetAmplitude(T->tape))
    return 1;
  else if(T->tape->current_position == T->tape->length || 
	  T->tape->current_position == 0)
    return 2;

  RULE *R = &T->rules[T->tape->string[T->tape->current_position]]
	    [T->current_state];

  T->tape->string[T->tape->current_position] = R->new_write;
 
  switch(R->move){
    case '<': T->tape->current_position--; break;
    case '>': T->tape->current_position++; break;
    case '=': break;
    case '.': fprintf(stdout, "\nThis machine has halted (.)\n"); exit(0);
    default: fprintf(stderr, "Error: TM moves restricted to {<, >, =, .}\n"); 
    exit(1);
    }

  T->current_state = R->new_state;

  if(T->tape->current_position > T->tape->max_position)
    T->tape->max_position = T->tape->current_position;

  if(T->tape->current_position < T->tape->min_position)
    T->tape->min_position = T->tape->current_position;

  T->tape->time++;

  return 0;
  }

// =============================================================================
// - - - - - - - - - - - - - - - R E S E T   T A P E - - - - - - - - - - - - - -

void ResetTape(TM *T){
  
  T->tape->guard            = DEFAULT_TAPE_GUARD;
  T->tape->length           = T->max_time * 2 + 1;
  T->tape->current_position = T->max_time + 1;
  T->tape->min_position     = T->tape->current_position - T->tape->guard;
  T->tape->max_position     = T->tape->current_position + T->tape->guard;
  T->tape->time             = 0;

  memset((void *) T->tape->string, 0, (T->tape->length +
                  2 * T->tape->guard) * sizeof(char));

  return;
  }

// =============================================================================
// - - - - - - - - - - - - - R A N D O M   F I L L   T M - - - - - - - - - - - -

uint8_t RandFillTM(TM *T, RAND *R){

  for(uint32_t x = 0 ; x < T->alphabet_size ; ++x)
    for(uint32_t y = 0 ; y < T->number_of_states ; ++y){
      T->rules[x][y].new_write = GetRandNumber(R) % T->alphabet_size;
      T->rules[x][y].move      = T->moves[GetRandNumber(R) % MAXIMUM_MOVES];
      T->rules[x][y].new_state = GetRandNumber(R) % T->number_of_states;
      }
 
  if(T->halt){
    T->rules[GetRandNumber(R) % T->alphabet_size]
	    [GetRandNumber(R) % T->number_of_states].move = '.';
    }

  ResetTape(T);   

  return 0;
  }


// =============================================================================
// - - - - - - - - - - - - R E S E T   D I S T R I B U T I O N - - - - - - - - -
void ResetDist(TM *T){
  
  memset(T->alphabet->counts, 0, 256 * sizeof(uint32_t));

  return;
  }

// =============================================================================
// - - - - - - - - - - E V A L U A T E   D I S T R I B U T I O N - - - - - - - -

int8_t EvalTapeDist(uint32_t amp, TM *T){

  assert(amp > 0);

  ResetDist(T);
  for(uint32_t x = T->tape->min_position ; x < T->tape->max_position ; ++x)
    T->alphabet->counts[T->tape->string[x]]++;

  uint32_t max = T->alphabet->counts[0];
  for(uint32_t i = 1 ; i < T->alphabet_size ; ++i)
    if(T->alphabet->counts[i] > max)
      max = T->alphabet->counts[i];

  if((double) max / amp > T->max_distribution)
    return 0; // CODE '0' TO STOP MACHINE
  
  return 1; // CODE '1' TO CONTINUE
  }

// =============================================================================
// - - - - - - - - - - - - - - - - R E M O V E   T M - - - - - - - - - - - - - -

void RemoveTM(TM *T){

  for(uint32_t x = 0 ; x < T->alphabet_size ; ++x)
    Free(T->rules[x]);
  Free(T->rules);
  Free(T->tape->string);
  Free(T->tape);
  Free(T->alphabet->string);
  Free(T->alphabet->out_string);
  Free(T->alphabet->rev_map);
  Free(T->alphabet);
  Free(T->moves);
  Free(T);

  return;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
