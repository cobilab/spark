#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>

#include "defs.h"
#include "mem.h"
#include "msg.h"
#include "args.h"
#include "tm.h"

/*
////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - F   T H R E A D I N G - - - - - - - - - - - - - - -

void *CompressThread(void *Thr){
  Threads *T = (Threads *) Thr;
  CompressTarget(T[0]);
  pthread_exit(NULL);
  }
*/


////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - C H E C K   I N I T I A L   S T A T E - - - - - - - - - -

void CheckInitialState(PARAMETERS *P){

  if(P->initial_state >= P->number_of_states && P->initial_state != RDST){
    fprintf(stderr, "Error: the initial state must be less than the number"
    " of states (< %u)\n", P->number_of_states);
    exit(1);
    }

 return;
 }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - P R I N T   I N F O R M A T I O N - - - - - - - - - - -

void PrintInformation(PARAMETERS *P){
  
  fprintf(stderr, " Using seed     :  %u\n", P->seed);
  fprintf(stderr, " Max steps      :  %u\n", P->max_steps);
  fprintf(stderr, " Max amplitude  :  %u\n", P->max_amplitude);
  fprintf(stderr, " Init state     :  %u\n", P->initial_state);

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - I M P O S S I B L E - - - - - - - - - - - - - -

void Impossible(PARAMETERS *P){

  THREADS *T;

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - S E A R C H - - - - - - - - - - - - - - - -

void Search(PARAMETERS *P){

  THREADS *T;

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - C O M P L E X I T Y   T O P - - - - - - - - - - - -

void ComplexityTop(PARAMETERS *P){

  THREADS *T;


  uint32_t x;
  TM *TM;

  if(!P->seed)
    P->seed = (uint32_t) time(NULL);
  srand(P->seed);

  CheckInitialState(P);

  if(P->initial_state == RDST) P->initial_state = rand() % P->number_of_states;

  PrintInformation(P);

  TM = CreateTM(P->alphabet_size, P->number_of_states, P->max_steps,
                P->max_amplitude, P->mode, P->initial_state);

  


  for(machine = 0 ; machine < P->threads ; ++machine){
    

    RandFillTM(TM);

    for(step = 0 ; step < P->max_steps ; ++step){
      UpdateTM(TM);
      }
    
    //Evaluate(TM);
    }


  uint8_t *filename = "out.txt";
  PrintTapeFile(TM, filename);

  RemoveTM(TM);
  
  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - S C H O O L   A D V A N C E D - - - - - - - - - - - -

void SchoolAdvanced(PARAMETERS *P){


  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - S C H O O L   S I M P L E - - - - - - - - - - - -

void SchoolSimple(PARAMETERS *P){

  uint32_t x;
  TM *TM;

  if(!P->seed) 
    P->seed = (uint32_t) time(NULL);
  srand(P->seed);

  CheckInitialState(P);

  if(P->initial_state == RDST) P->initial_state = rand() % P->number_of_states;

  PrintInformation(P);

  TM = CreateTM(P->alphabet_size, P->number_of_states, P->max_steps,
                P->max_amplitude, P->mode, P->initial_state);

  RandFillTM(TM);

  PrintAlphabet(TM);
  PrintStates(TM);
  PrintActions(TM);
  PrintTM(TM);

  fprintf(stderr, "\n");
  for(x = 0 ; x < P->max_steps ; ++x){
    UpdateTM(TM);
    if(!P->hide_tape){
      if(P->show_tape) PrintTape(TM);
      else             PrintTapePres(TM, P->delay);
      }
    }
  
  fprintf(stderr, "\n");

  uint8_t *filename = "out.txt";
  PrintTapeFile(TM, filename);

  RemoveTM(TM);

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - M A I N - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int32_t main(int argc, char *argv[]){
  
  char    **p = *&argv;
  clock_t stop = 0, start = clock();

  PARAMETERS *P = (PARAMETERS *) Malloc(1 * sizeof(PARAMETERS));
  if((P->help = ArgState(DEF_HELP, p, argc, "-h", "--help")) == 1 || argc < 2){
    PrintMenu();
    return EXIT_SUCCESS;
    }

  if(ArgState(DEF_VERSION, p, argc, "-a", "--about")){
    PrintVersion();
    return EXIT_SUCCESS;
    }

  P->verbose          = ArgState (DEF_VERBOSE, p, argc, "-v",  "--verbose");
  P->force            = ArgState (DEF_FORCE,   p, argc, "-f",  "--force");
  P->show_tape        = ArgState (0,           p, argc, "-sa", "--show-all-tape");
  P->hide_tape        = ArgState (0,           p, argc, "-ht", "--hide-tape");
 
  P->initial_state    = ArgNumber (RDST,  p, argc, "-is", "--initial-state", 0, 255);
  P->alphabet_size    = ArgNumber (4,     p, argc, "-as", "--alphabet-size", 2, 254);
  P->number_of_states = ArgNumber (7,     p, argc, "-sn", "--states-number", 1, 99);
  P->max_steps        = ArgNumber (10000, p, argc, "-ms", "--max-steps", 1, 2999999999);
  P->max_amplitude    = ArgNumber (20000, p, argc, "-ma", "--max-amplitude", 1, 2999999999);
  P->mode             = ArgNumber (1,     p, argc, "-md", "--mode",  1, 5);
  P->top              = ArgNumber (5,     p, argc, "-tp", "--top",   0, 100000);
  P->seed             = ArgNumber (0,     p, argc, "-rs", "--seed",  0, 2999999999);
  P->delay            = ArgNumber (50000, p, argc, "-dl", "--delay", 0, 2999999999);

  fprintf(stderr, "\n");

  switch(P->mode){
    case 1: SchoolSimple   (P);  break;
    case 2: SchoolAdvanced (P);  break;
    case 3: ComplexityTop  (P);  break;
    case 4: Search         (P);  break;
    case 5: Impossible     (P);  break;
    default: fprintf(stderr, "Error: unknown mode!\n"); exit(1); break;
    }

  stop = clock();
  fprintf(stderr, "\n");
  fprintf(stdout, " Spent %g sec.\n", ((double)(stop-start)) / CLOCKS_PER_SEC);
  fprintf(stderr, "\n");

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
