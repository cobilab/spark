#ifndef ARGS_H_INCLUDED
#define ARGS_H_INCLUDED

#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint32_t    ArgNumber       (uint32_t , char *[], uint32_t, char *, char *, 
		             uint32_t, uint32_t);
int32_t     ArgNNumber      (int32_t , char *[], uint32_t, char *, char *, 
		             int32_t, int32_t);
double      ArgDouble       (double, char *[], uint32_t, char *, char *);
uint8_t     ArgState        (uint8_t  , char *[], uint32_t, char *, char *);
char        *ArgString      (char    *, char *[], uint32_t, char *, char *);
void        PrintArgs       (PARAMETERS *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
