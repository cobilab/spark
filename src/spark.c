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
#include "buffer.h"
#include "context.h"
#include "pmodels.h"
#include "array.h"
#include "hash.h"
#include "tm.h"

PARAMETERS *P;

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - I N I T I A L I Z E   L C G - - - - - - - - - - - -

void InitializeLCG(THREADS *T){

 T->value = T->seed;

 return;
 }


////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - C O M P U T E   L C G - - - - - - - - - - - - - -

void ComputeLCF(THREADS *T){

 T->value = T->a * T->value + T->b % T->prime;

 return;
 }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - C H E C K   I N I T I A L   S T A T E - - - - - - - - - -

void CheckInitialState(void){

  if(P->initial_state >= P->number_of_states && P->initial_state != RDST){
    fprintf(stderr, "Error: the initial state must be less than the number"
    " of states (< %u)\n", P->number_of_states);
    exit(1);
    }

 return;
 }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - P R I N T   I N F O R M A T I O N - - - - - - - - - - -

void PrintInformation(void){
 
  if(P->mode == 3){	
    fprintf(stderr, " Threads (T)    :  %u\n", P->threads);
    fprintf(stderr, " (T) machines   :  %u\n", P->thread_machines);
    fprintf(stderr, " Context (FCM)  :  %u\n", P->ctx);
    }

  fprintf(stderr, " Using seed     :  %u\n", P->seed);
  fprintf(stderr, " Max steps      :  %u\n", P->max_steps);
  fprintf(stderr, " Max amplitude  :  %u\n", P->max_amplitude);
  fprintf(stderr, " Min amplitude  :  %u\n", P->min_amplitude);
  fprintf(stderr, " Init state     :  %u\n", P->initial_state);

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - I M P O S S I B L E - - - - - - - - - - - - - -

void Impossible(void){

  THREADS *T;

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - S E A R C H - - - - - - - - - - - - - - - -

void Search(void){

  THREADS *T;

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - G E T   T A P E   C O M P L E X I T Y - - - - - - - - - -

double GetTapeComplexity(TM *TM){

  double   bits = 0;
  uint32_t x, n, k;
  uint8_t  sym;
  CBUF     *symBuf = CreateCBuffer(BUFFER_SIZE, BGUARD);
  PModel   *PM = CreatePModel(P->alphabet_size);
  CModel   *CM = CreateCModel(P->ctx, 1, 0, 0, 0, P->alphabet_size, 0.9, 0.9);

  for(x = TM->tape->minimum_position ; x < TM->tape->maximum_position ; ++x){
    symBuf->buf[symBuf->idx] = sym = TM->alphabet->string[TM->tape->string[x]];
    GetPModelIdx(&symBuf->buf[symBuf->idx-1], CM);
    ComputePModel(CM, PM, CM->pModelIdx, CM->alphaDen);
    bits += PModelSymbolNats(PM, sym) / M_LN2;
    UpdateCModelCounter(CM, sym, CM->pModelIdx);
    UpdateCBuffer(symBuf);
    }

  RemoveCModel(CM);
  RemovePModel(PM);
  RemoveCBuffer(symBuf);

  return bits / GetAmplitude(TM->tape);
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - C O M P L E X I T Y   T H R E A D S - - - - - - - - - - -

void ComplexityTMs(THREADS T){

  uint32_t x, step, machine;
  char out[4096];
  sprintf(out, "%s-%u.inf", P->output_top, T.id+1);
  FILE *Writter = fopen(out, "w");

  TM *TM = CreateTM(P->alphabet_size, P->number_of_states, P->max_steps, 
           P->max_amplitude, P->min_amplitude, P->mode, P->initial_state);

  for(machine = 0 ; machine < P->thread_machines ; ++machine){
    
    RandFillTM(TM);
    for(step = 0 ; step < P->max_steps ; ++step){
      UpdateTM(TM);
      }
    
    double amplitude = GetAmplitude(TM->tape);
    if(TM->minimum_amplitude < amplitude && TM->maximum_amplitude > amplitude){
    
      if(!P->complexity) 
        fprintf(Writter, "%.3lf\t%u\t%u\t%u\t", GetTapeComplexity(TM), P->seed, 
        P->number_of_states, P->alphabet_size);
      else
        fprintf(Writter, "%u\t%u\t%u\t", P->seed, P->number_of_states, 
	P->alphabet_size);

      PrintTapeInWritter(TM, Writter);    
      }
    }

  RemoveTM(TM);

  fclose(Writter);
  
  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - C   T H R E A D I N G - - - - - - - - - - - - -

void *ComplexityThread(void *Thr){
  THREADS *T = (THREADS *) Thr;
  ComplexityTMs(T[0]);
  pthread_exit(NULL);
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - C O M P L E X I T Y   T O P - - - - - - - - - - - -

void ComplexityTop(void){
  
  uint32_t x;
  pthread_t t[P->threads];
  THREADS *T = (THREADS *) Calloc(P->threads, sizeof(THREADS));

  srand(P->seed);
  
  for(x = 0 ; x < P->threads ; ++x){
    T[x].id = x;
    InitializeLCG(T);
    }

  CheckInitialState();

  if(P->initial_state == RDST) P->initial_state = rand() % P->number_of_states;

  PrintInformation();

  for(x = 0 ; x < P->threads ; ++x)
    pthread_create(&(t[x+1]), NULL, ComplexityThread, (void *) &(T[x]));
  for(x = 0 ; x < P->threads ; ++x) // DO NOT JOIN FORS!
    pthread_join(t[x+1], NULL);

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - S C H O O L   A D V A N C E D - - - - - - - - - - - -

void SchoolAdvanced(void){


  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - S C H O O L   S I M P L E - - - - - - - - - - - -

void SchoolSimple(void){

  uint32_t x;
  TM *TM;

  srand(P->seed);

  CheckInitialState();

  if(P->initial_state == RDST) P->initial_state = rand() % P->number_of_states;

  PrintInformation();

  TM = CreateTM(P->alphabet_size, P->number_of_states, P->max_steps,
                P->max_amplitude, P->min_amplitude, P->mode, P->initial_state);

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

  PrintTapeFile(TM, P->output_tm);

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

  P = (PARAMETERS *) Malloc(1 * sizeof(PARAMETERS));
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
  P->complexity       = ArgState (0,           p, argc, "-sc", "--skip-complexity");
 
  P->threads          = ArgNumber (DEFT,  p, argc, "-t",  "--threads", 0, 5000);
  P->initial_state    = ArgNumber (RDST,  p, argc, "-is", "--initial-state", 0, 255);
  P->alphabet_size    = ArgNumber (4,     p, argc, "-as", "--alphabet-size", 2, 254);
  P->number_of_states = ArgNumber (7,     p, argc, "-sn", "--states-number", 1, 99);
  P->max_steps        = ArgNumber (10000, p, argc, "-ms", "--max-steps", 1, 2999999999);
  P->thread_machines  = ArgNumber (10000, p, argc, "-tm", "--machines", 1, 2999999999);
  P->min_amplitude    = ArgNumber (50,    p, argc, "-ia", "--min-amplitude", 1, 2999999999);
  P->max_amplitude    = ArgNumber (20000, p, argc, "-ma", "--max-amplitude", 1, 2999999999);
  P->mode             = ArgNumber (1,     p, argc, "-md", "--mode",  1, 5);
  P->top              = ArgNumber (5,     p, argc, "-tp", "--top",   0, 100000);
  P->seed             = ArgNumber (0,     p, argc, "-rs", "--seed",  0, 2999999999);
  P->delay            = ArgNumber (50000, p, argc, "-dl", "--delay", 0, 2999999999);
  P->ctx              = ArgNumber (2,     p, argc, "-co", "--context", 0, 32);

  P->output_tm        = ArgString ("tm.txt",  p, argc, "-ot", "--output-tm");
  P->output_top       = ArgString ("top.txt", p, argc, "-ox", "--output-top");

  fprintf(stderr, "\n");

  if(!P->seed) P->seed = (uint32_t) time(NULL);

  switch(P->mode){
    case 1: SchoolSimple   ();  break;
    case 2: SchoolAdvanced ();  break;
    case 3: ComplexityTop  ();  break;
    case 4: Search         ();  break;
    case 5: Impossible     ();  break;
    default: fprintf(stderr, "Error: unknown mode!\n"); exit(1); break;
    }

  stop = clock();
  fprintf(stderr, "\n");
  fprintf(stdout, " Spent %g sec.\n", ((double)(stop-start)) / CLOCKS_PER_SEC);
  fprintf(stderr, "\n");

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
