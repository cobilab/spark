#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <assert.h>
#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <unistd.h>

typedef uint64_t ULL;
typedef uint64_t U64;
typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t  U8;
typedef int64_t  I64;
typedef int32_t  I32;
typedef int16_t  I16;
typedef int8_t   I8;

typedef struct{
  U8        help;
  U8        verbose;
  U8        force;
  U8        show_tape;
  U8        hide_tape;
  U8        show_action;
  U8        hide_rules;
  int32_t   initial_state;
  uint32_t  top;
  uint32_t  threads;
  double    threshold;
  uint32_t  alphabet_size;
  uint32_t  number_of_states;
  uint32_t  max_time;
  uint32_t  thread_machines;
  uint32_t  max_amplitude;
  uint32_t  min_amplitude;
  uint8_t   mode;
  uint8_t   complexity;
  uint32_t  seed;
  uint32_t  delay;
  uint32_t  ctx;
  uint32_t  lcf_value;
  uint32_t  lcf_a;
  uint32_t  lcf_b;
  uint32_t  lcf_prime;
  char      *alphabet;
  char      *output_top;
  char      *output_tape;
  char      *input_rules;
  char      *input_sequence;
  }
PARAMETERS;

typedef struct{
  uint32_t  id;
  }
THREADS;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define RELEASE                1
#define VERSION                1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define DEF_VERSION            0
#define DEF_THRESHOLD          0.5
#define DEF_HELP               0
#define DEF_VERBOSE            0
#define DEF_FORCE              0
#define RDST                   255
#define DEFT                   4

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

