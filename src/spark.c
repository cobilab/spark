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
#include "misc.h"
#include "rand.h"
#include "buffer.h"
#include "context.h"
#include "pmodels.h"
#include "fastmem.h"
#include "array.h"
#include "hash.h"
#include "tm.h"

PARAMETERS *P;

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - C A L C U L A T E   B I T S - - - - - - - - - - - -

double CalculateBits(void){

 double rule = log2(P->alphabet_size) + log2(P->number_of_states) + log2(3);
 double bits = ((double) P->alphabet_size * P->number_of_states * rule);

 return bits;
 }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - C A L C U L A T E   I N P U T   B I T S - - - - - - - - - -

double CalculateInputBits(void){

 return ((double) log2(P->alphabet_size) * NBytesInFile(P->input_sequence));
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

  fprintf(stderr, "Mode           :  %u\n", P->mode);
  switch(P->mode){
    case 1: 
    fprintf(stderr, "Using seed     :  %u\n", P->seed);
    fprintf(stderr, "Init state     :  %u\n", P->initial_state);
    fprintf(stderr, "Max time       :  %u\n", P->max_time);
    fprintf(stderr, "Max amplitude  :  %u\n", P->max_amplitude);
    fprintf(stderr, "Min amplitude  :  %u\n", P->min_amplitude);
    fprintf(stderr, "Visual delay   :  %u\n", P->delay);
    break;

    case 2:
    fprintf(stderr, "Max time       :  %u\n", P->max_time);
    fprintf(stderr, "Max amplitude  :  %u\n", P->max_amplitude);
    fprintf(stderr, "Min amplitude  :  %u\n", P->min_amplitude);
    break;

    case 3:
    fprintf(stderr, "Threads (T)    :  %u\n", P->threads);
    fprintf(stderr, "Machines by T  :  %u\n", P->thread_machines);
    fprintf(stderr, "Context (FCM)  :  %u\n", P->ctx);
    fprintf(stderr, "Max time       :  %u\n", P->max_time);
    fprintf(stderr, "Max amplitude  :  %u\n", P->max_amplitude);
    fprintf(stderr, "Min amplitude  :  %u\n", P->min_amplitude);
    fprintf(stderr, "NC threshold   :  %.3lf\n", P->threshold);
    fprintf(stderr, "Bits in rules  :  %.3lf\n", CalculateBits());
    break;

    case 4:
    fprintf(stderr, "Threads (T)    :  %u\n", P->threads);
    fprintf(stderr, "Machines by T  :  %u\n", P->thread_machines);
    fprintf(stderr, "Context (FCM)  :  %u\n", P->ctx);
    fprintf(stderr, "Max time       :  %u\n", P->max_time);
    fprintf(stderr, "Max amplitude  :  %u\n", P->max_amplitude);
    fprintf(stderr, "Min amplitude  :  %u\n", P->min_amplitude);
    fprintf(stderr, "NRC Threshold  :  %.3lf\n", P->threshold);
    fprintf(stderr, "Bits in rules  :  %.3lf\n", CalculateBits());
    fprintf(stderr, "Bits in input  :  %.3lf\n", CalculateInputBits());
    break;

    case 5:
    fprintf(stderr, "Threads (T)    :  %u\n", P->threads);
    fprintf(stderr, "Machines by T  :  %u\n", P->thread_machines);
    fprintf(stderr, "Context (FCM)  :  %u\n", P->ctx);
    fprintf(stderr, "Max time       :  %u\n", P->max_time);
    fprintf(stderr, "Max amplitude  :  %u\n", P->max_amplitude);
    fprintf(stderr, "Min amplitude  :  %u\n", P->min_amplitude);
    fprintf(stderr, "Threshold      :  %.3lf\n", P->threshold);
    fprintf(stderr, "Bits in rules  :  %.3lf\n", CalculateBits());
    fprintf(stderr, "Bits in input  :  %.3lf\n", CalculateInputBits());
    break;

    case 6:
    fprintf(stderr, "Threads (T)    :  %u\n", P->threads);
    fprintf(stderr, "Machines by T  :  %u\n", P->thread_machines);
    fprintf(stderr, "Context (FCM)  :  %u\n", P->ctx);
    fprintf(stderr, "Max time       :  %u\n", P->max_time);
    fprintf(stderr, "Max amplitude  :  %u\n", P->max_amplitude);
    fprintf(stderr, "Min amplitude  :  %u\n", P->min_amplitude);
    break;
    }

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - C H E C K   A L P H A B E T   S Y M - - - - - - - - - - -

void CheckInputAlphabet(ALPHABET *AL, uint8_t *input){

  int x = 0, c, found = 0;
  FILE *F = Fopen(input, "r");

  while((c = fgetc(F)) != EOF){
 
    found = 0;
    for(x = 0 ; x < AL->size ; ++x)
      if(c == AL->out_string[x]){
	found = 1;
	break;
        }

    if(found == 0){
      fprintf(stderr, "Error: input char not in alphabet (ascii: %d)!\n", c);
      exit(1);
      }
    }

  fclose(F);

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - M E M O R I Z E   T A P E - - - - - - - - - - - -

void MemorizeTape(TM *TM, CModel *CM){

  uint32_t x;
  uint8_t sym;
  CBUF *symBuf = CreateCBuffer(BUFFER_SIZE, BGUARD);

  for(x = TM->tape->minimum_position ; x < TM->tape->maximum_position ; ++x){
    symBuf->buf[symBuf->idx] = sym = TM->alphabet->string[TM->tape->string[x]];
    GetPModelIdx(&symBuf->buf[symBuf->idx-1], CM);
    UpdateCModelCounter(CM, sym, CM->pModelIdx);
    UpdateCBuffer(symBuf);
    }

  RemoveCBuffer(symBuf);

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - L O A D  F A S T  M E M - - - - - - - - - - - - - -
//
void LoadFastMem(FAST_MEM *FM, CModel *CM, ALPHABET *AL){
  
  int      c;
  uint8_t  sym;
  uint32_t idx = 0;
  CBUF     *symBuf = CreateCBuffer(BUFFER_SIZE, BGUARD);

  if(!strcmp(P->input_sequence, "-")){
    fprintf(stderr, "Error: missing input sequence to compare!\n");
    exit(1);
    }

  FILE *F = Fopen(P->input_sequence, "r");

  ResetCModelIdx(CM);
  while((c = fgetc(F)) != EOF){
    symBuf->buf[symBuf->idx] = sym = AL->rev_map[c];
    GetPModelIdx(&symBuf->buf[symBuf->idx-1], CM);
    FM->idx[idx]   = CM->pModelIdx;
    FM->sym[idx] = sym;
    UpdateCBuffer(symBuf);
    if(idx++ == FM->length){
      fprintf(stderr, "Error: error loading fast indexes\n");
      exit(1);
      }
    }

  RemoveCBuffer(symBuf);
  fclose(F); 

  ResetCModel(CM);

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - X   R E L A T I V E   E V A L U A T I O N - - - - - - - - -

double XRelativeEvaluation(FAST_MEM *FM, CModel *CM, ALPHABET *AL){

  uint32_t idx = 0;
  double   bits = 0;

  while(idx < FM->length){
    ComputePModel(CM, FM->pmodel, FM->idx[idx], CM->alphaDen);
    bits += PModelSymbolNats(FM->pmodel, FM->sym[idx]) / M_LN2;
    ++idx;
    }

  return (bits / FM->normalization);
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - R E L A T I V E   E V A L U A T I O N - - - - - - - - -

double RelativeEvaluation(CModel *CM, uint8_t *input_name, ALPHABET *AL){

  int      c;
  uint8_t  sym;
  uint32_t sequence_size = 0;
  double   bits = 0;
  CBUF     *symBuf = CreateCBuffer(BUFFER_SIZE, BGUARD);
  PModel   *PM = CreatePModel(P->alphabet_size);

  if(!strcmp(P->input_sequence, "-")){
    fprintf(stderr, "Error: missing input sequence to compare!\n");
    exit(1);
    }

  FILE *F = Fopen(P->input_sequence, "r");

  ResetCModelIdx(CM);
  while((c = fgetc(F)) != EOF){ 
    symBuf->buf[symBuf->idx] = sym = AL->rev_map[c];
    GetPModelIdx(&symBuf->buf[symBuf->idx-1], CM);
    ComputePModel(CM, PM, CM->pModelIdx, CM->alphaDen);
    bits += PModelSymbolNats(PM, sym) / M_LN2;
    UpdateCBuffer(symBuf);
    ++sequence_size;
    }

  RemovePModel(PM);
  RemoveCBuffer(symBuf);
  fclose(F);

  return (bits / ((double) sequence_size * log2(AL->size)));
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - I M P O S S I B L E - - - - - - - - - - - - - -

void Impossible(void){

  THREADS *T;

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - X S E A R C H   T H R E A D S - - - - - - - - - - -

void XSearchTMs(THREADS T){

  double amplitude;
  uint32_t x, time, machine, initial_state = 0;
  char out[4096];
  sprintf(out, "%s-%u.inf", P->output_top, T.id+1);
  FILE *Writter = Fopen(out, "w");

  RAND *R = CreateRand(P->rand_type);
  if(P->initial_state == RDST)
    P->initial_state = GetRandNumber(R) % P->number_of_states;

  CModel *CM = CreateCModel(P->ctx, 1, 0, 0, 0, P->alphabet_size, 0.9, 0.9);

  TM *TM = CreateTM(P->alphabet, P->alphabet_size, P->number_of_states,
  P->max_time, P->max_amplitude, P->min_amplitude, P->mode, P->initial_state);

  CheckInputAlphabet(TM->alphabet, P->input_sequence);

  FAST_MEM *FM = CreateFastMem(NBytesInFile(P->input_sequence), 
		 TM->alphabet_size);

  LoadFastMem(FM, CM, TM->alphabet);

  fprintf(Writter, "NC\tSeed\tTime\tStates\tAlphabet\tInitState\tRules\tTape\n");
  for(machine = 0 ; machine < P->thread_machines ; ++machine){

    initial_state = TM->current_state;
    RandFillTM(TM, R);

    for(time = 0 ; time < P->max_time ; ++time){
      UpdateTM(TM);
      }

    amplitude = GetAmplitude(TM->tape);
    if(TM->minimum_amplitude < amplitude && TM->maximum_amplitude > amplitude){

      MemorizeTape(TM, CM);
      double nrc = XRelativeEvaluation(FM, CM, TM->alphabet);
      ResetCModel(CM);

      if(nrc < P->threshold){

	if(P->verbose) fprintf(stderr, "Found TM NRC   :  %3lf\n", nrc);

        fprintf(Writter, "%.3lf\t%u\t%u\t%u\t%u\t%u\t", nrc,
        P->seed, TM->maximum_time, TM->number_of_states,
        TM->alphabet_size, initial_state);

        if(!P->hide_rules)
          PrintRulesInWritter(TM, Writter);
        PrintTapeInWritter(TM, Writter);
        }
      }
    }

  RemoveCModel(CM);
  RemoveTM(TM);
  RemoveRand(R);
  fclose(Writter);

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - S E A R C H   T H R E A D S - - - - - - - - - - -

void SearchTMs(THREADS T){

  double amplitude;
  uint32_t x, time, machine, initial_state = 0;
  char out[4096];
  sprintf(out, "%s-%u.inf", P->output_top, T.id+1);
  FILE *Writter = Fopen(out, "w");

  RAND *R = CreateRand(P->rand_type);
  if(P->initial_state == RDST)
    P->initial_state = GetRandNumber(R) % P->number_of_states;

  CModel *CM = CreateCModel(P->ctx, 1, 0, 0, 0, P->alphabet_size, 0.9, 0.9);
  
  TM *TM = CreateTM(P->alphabet, P->alphabet_size, P->number_of_states,
  P->max_time, P->max_amplitude, P->min_amplitude, P->mode, P->initial_state);

  CheckInputAlphabet(TM->alphabet, P->input_sequence);

  fprintf(Writter, "NC\tSeed\tTime\tStates\tAlphabet\tInitState\tRules\tTape\n");
  for(machine = 0 ; machine < P->thread_machines ; ++machine){

    initial_state = TM->current_state;
    RandFillTM(TM, R);

    for(time = 0 ; time < P->max_time ; ++time){
      UpdateTM(TM);
      }

    amplitude = GetAmplitude(TM->tape);
    if(TM->minimum_amplitude < amplitude && TM->maximum_amplitude > amplitude){

      MemorizeTape(TM, CM);
      double nrc = RelativeEvaluation(CM, P->input_sequence, TM->alphabet);
      ResetCModel(CM);

      if(nrc < P->threshold){
	
	if(P->verbose) fprintf(stderr, "Found TM NRC   :  %3lf\n", nrc);

        fprintf(Writter, "%.3lf\t%u\t%u\t%u\t%u\t%u\t", nrc,
        P->seed, TM->maximum_time, TM->number_of_states,
        TM->alphabet_size, initial_state);

        if(!P->hide_rules)
          PrintRulesInWritter(TM, Writter);
        PrintTapeInWritter(TM, Writter);
        }
      }
    }

  RemoveCModel(CM);
  RemoveTM(TM);
  RemoveRand(R);
  fclose(Writter);

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - X S   T H R E A D I N G - - - - - - - - - - - - -

void *XSearchThread(void *Thr){
  THREADS *T = (THREADS *) Thr;
  XSearchTMs(T[0]);
  pthread_exit(NULL);
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - S   T H R E A D I N G - - - - - - - - - - - - -

void *SearchThread(void *Thr){
  THREADS *T = (THREADS *) Thr;
  SearchTMs(T[0]);
  pthread_exit(NULL);
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - X   S E A R C H - - - - - - - - - - - - - -

void XSearch(void){

  uint32_t x;
  pthread_t t[P->threads];
  THREADS *T = (THREADS *) Calloc(P->threads, sizeof(THREADS));

  for(x = 0 ; x < P->threads ; ++x)
    T[x].id = x;

  CheckInitialState();

  PrintInformation();

  for(x = 0 ; x < P->threads ; ++x)
    pthread_create(&(t[x+1]), NULL, XSearchThread, (void *) &(T[x]));
  for(x = 0 ; x < P->threads ; ++x) // DO NOT JOIN FORS!
    pthread_join(t[x+1], NULL);

  return;
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - S E A R C H - - - - - - - - - - - - - - - -

void Search(void){

  uint32_t x;
  pthread_t t[P->threads];
  THREADS *T = (THREADS *) Calloc(P->threads, sizeof(THREADS));

  for(x = 0 ; x < P->threads ; ++x)
    T[x].id = x;

  CheckInitialState();

  PrintInformation();

  for(x = 0 ; x < P->threads ; ++x)
    pthread_create(&(t[x+1]), NULL, SearchThread, (void *) &(T[x]));
  for(x = 0 ; x < P->threads ; ++x) // DO NOT JOIN FORS!
    pthread_join(t[x+1], NULL);

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

  return bits / (GetAmplitude(TM->tape) * log2(P->alphabet_size)); // CHANGE TO FLOG_2?
  }

////////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - C O M P L E X I T Y   T H R E A D S - - - - - - - - - - -

void ComplexityTMs(THREADS T){

  double complexity;
  uint32_t x, time, machine, initial_state = 0, amplitude;
  char out[4096];
  sprintf(out, "%s-%u.inf", P->output_top, T.id+1);
  FILE *Writter = Fopen(out, "w");

  RAND *R = CreateRand(P->rand_type);

  if(P->initial_state == RDST)
    P->initial_state = GetRandNumber(R) % P->number_of_states;

  TM *TM = CreateTM(P->alphabet, P->alphabet_size, P->number_of_states, 
  P->max_time, P->max_amplitude, P->min_amplitude, P->mode, P->initial_state);

  fprintf(Writter, "NC\tSeed\tsize\tTime\tStates\tAlphabet"
		   "\tInitState\tRules\tTape\n");

  for(machine = 0 ; machine < P->thread_machines ; ++machine){
    
    initial_state = TM->current_state;
    RandFillTM(TM, R);
    
    for(time = 0 ; time < P->max_time ; ++time){
      UpdateTM(TM);
      }
    
    amplitude = GetAmplitude(TM->tape);
    if(TM->minimum_amplitude < amplitude && TM->maximum_amplitude > amplitude){

      complexity = GetTapeComplexity(TM);

      if(complexity > P->threshold){      
    
        fprintf(Writter, "%.3lf\t%u\t%u\t%u\t%u\t%u\t%u\t", complexity, 
	P->seed, amplitude, TM->maximum_time, TM->number_of_states,
       	TM->alphabet_size, initial_state);

        if(!P->hide_rules) 
	  PrintRulesInWritter(TM, Writter);
        PrintTapeInWritter(TM, Writter);
        }
      }
    }

  RemoveTM(TM);
  RemoveRand(R);
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

  for(x = 0 ; x < P->threads ; ++x)
    T[x].id = x;

  CheckInitialState();
  
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
// - - - - - - - - - - - - - S C H O O L   S I M P L E - - - - - - - - - - - - -

void SchoolSimple(void){

  uint32_t x;

  CheckInitialState();

  RAND *R = CreateRand(P->rand_type);

  if(P->initial_state == RDST) 
    P->initial_state = GetRandNumber(R) % P->number_of_states;

  PrintInformation();

  TM *TM = CreateTM(P->alphabet, P->alphabet_size, P->number_of_states, 
  P->max_time, P->max_amplitude, P->min_amplitude, P->mode, P->initial_state);

  if(strcmp(P->input_rules, "-"))
    LoadTMRules(TM, P->input_rules);
  else 
    RandFillTM(TM, R);

  PrintAlphabet (TM);
  PrintStates   (TM);
  PrintActions  (TM);
  PrintTM       (TM);

  fprintf(stderr, "\n");
  
  for(x = 0 ; x < TM->maximum_time ; ++x){
    UpdateTM(TM);

    if(!P->hide_tape){
      if(P->show_tape) PrintTape(TM);
      else             PrintTapePres(TM, P->delay);
      }
    }
  
  fprintf(stderr, "\n");

  if(strcmp(P->output_tape, "-")) PrintTapeFile(TM, P->output_tape);

  RemoveTM(TM);
  RemoveRand(R);

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
    PrintAbout();
    return EXIT_SUCCESS;
    }

  if(ArgState(DEF_VERSION, p, argc, "-x", "--version")){
    PrintVersion();
    return EXIT_SUCCESS;
    }

  P->verbose          = ArgState (DEF_VERBOSE, p, argc, "-v",  "--verbose");
  P->force            = ArgState (DEF_FORCE,   p, argc, "-f",  "--force");
  P->show_tape        = ArgState (0,           p, argc, "-sa", "--show-all-tape");
  P->hide_tape        = ArgState (0,           p, argc, "-ht", "--hide-tape");
  P->complexity       = ArgState (0,           p, argc, "-sc", "--skip-complexity");
  P->hide_rules       = ArgState (0,           p, argc, "-hr", "--hide-rules");
  P->hide_color       = ArgState (0,           p, argc, "-hc", "--hide-color");
  P->random_tape      = ArgState (0,           p, argc, "-rt", "--random-tape");
 
  P->threads          = ArgNumber (DEFT,  p, argc, "-t",  "--threads", 0, 5000);
  P->initial_state    = ArgNumber (RDST,  p, argc, "-is", "--initial-state", 0, 255);
  P->alphabet_size    = ArgNumber (4,     p, argc, "-as", "--alphabet-size", 2, 254);
  P->number_of_states = ArgNumber (7,     p, argc, "-sn", "--states-number", 1, 99);
  P->max_time         = ArgNumber (10000, p, argc, "-ms", "--max-time", 1, NMAX);
  P->thread_machines  = ArgNumber (10000, p, argc, "-tm", "--machines", 1, NMAX);

  P->min_amplitude    = ArgNumber (50,    p, argc, "-ia", "--min-amplitude", 1, NMAX);
  P->max_amplitude    = ArgNumber (20000, p, argc, "-ma", "--max-amplitude", 1, NMAX);
  P->mode             = ArgNumber (1,     p, argc, "-md", "--mode",  1, 6);
  P->top              = ArgNumber (5,     p, argc, "-tp", "--top",   0, 100000);
  P->seed             = ArgNumber (0,     p, argc, "-rs", "--seed",  0, NMAX);
  P->delay            = ArgNumber (50000, p, argc, "-dl", "--delay", 0, NMAX);
  P->ctx              = ArgNumber (2,     p, argc, "-co", "--context", 0, 32);
  P->rand_type        = ArgNumber (0,     p, argc, "-rt", "--rand-type", 0, 1);
  P->threshold        = ArgDouble (0,     p, argc, "-th", "--threshold");

  P->alphabet         = ArgString ("-",       p, argc, "-al", "--alphabet");
  
  P->output_tape      = ArgString ("-",       p, argc, "-ot", "--output-tape");
  P->output_top       = ArgString ("top.txt", p, argc, "-ox", "--output-top");
  P->input_rules      = ArgString ("-",       p, argc, "-ir", "--input-rules");
  P->input_sequence   = ArgString ("-",       p, argc, "-i",  "--input");

  fprintf(stderr, "\n");

  if(!P->seed) 
    P->seed = (uint32_t) time(NULL);
  srand(P->seed);

  switch(P->mode){
    case 1: SchoolSimple   ();  break;
    case 2: SchoolAdvanced ();  break;
    case 3: ComplexityTop  ();  break;
    case 4: Search         ();  break;
    case 5: XSearch        ();  break;
    case 6: Impossible     ();  break;
    default: fprintf(stderr, "Error: unknown mode!\n"); exit(1); break;
    }

  fprintf(stderr, "\n");

  stop = clock();
  fprintf(stdout, " Spent %g sec.\n\n", ((double)(stop-start))/CLOCKS_PER_SEC);

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
