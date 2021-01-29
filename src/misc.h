#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

#include "defs.h"

uint32_t    FLog2          (uint64_t);
uint64_t    NBytesInFile   (uint8_t *);
FILE        *Fopen         (const char *, const char *);

#endif

