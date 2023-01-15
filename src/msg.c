#include "msg.h"
#include <stdio.h>
#include <stdlib.h>

void PrintMenu(void){
  fprintf(stderr,
  "                                                                     \n"  
  "     ███████  ███████  ███████  ███████  ██   ███                    \n"
  "     ██       ██   ██  ██   ██  ██   ██  ██  ███                     \n"
  "     ███████  ███████  ███████  ███████  ██████                      \n"
  "          ██  ██       ██   ██  ██ ███   ██  ███                     \n"
  "     ███████  ██       ██   ██  ██  ███  ██   ███                    \n"
  "                                                                     \n"  
  "NAME                                                                 \n"
  "     SPARK, v%u-r%u.                                                 \n"
  "                                                                     \n"
  "SYNOPSIS                                                             \n"
  "     ./SPARK [OPTION]...                                             \n"
  "                                                                     \n"
  "DESCRIPTION                                                          \n"
  "     This program schools, simulates, and searches for exact or      \n"
  "     approximate Turing machines (TM) w/ specific characteristics.   \n"
  "                                                                     \n"
  "ARGUMENTS                                                            \n"
  "     -h,        --help,                 give this help,              \n"
  "     -a,        --about,                display extra info,          \n"
  "     -x,        --version,              display version number,      \n"
  "     -v,        --verbose,              verbose mode (more info),    \n"
  "     -f,        --force,                force output overwrite,      \n"
  "     -t,        --threads,              number of threads (df:%u),   \n"
  "                                                                     \n"
  "     -sc,       --skip-complexity,      skip complexity process,     \n"
  "     -ht,       --hide-tape,            hide tape output,            \n"
  "     -sa,       --show-all-tape,        show all tape iterations,    \n"
  "     -hr,       --hide-rules,           hide rules in top file,      \n"
  "                                                                     \n"
  "     -rt,       --random-tape,          random tape (input size),    \n"
  "                                                                     \n"
  "     -is <INT>, --initial-state <INT>,  initial state to start,      \n"
  "     -as <INT>, --alphabet-size <INT>,  alphabet cardinality,        \n"
  "     -sn <INT>, --states-number <INT>,  number of states,            \n"
  "     -ms <INT>, --max-time <INT>,       maximum time before halt,    \n"
  "     -ma <INT>, --max-amplitude <INT>,  maximum tape amplitude,      \n"
  "     -ia <INT>, --min-amplitude <INT>,  minimum tape amplitude,      \n"
  "                                                                     \n"
  "     -al <STR>, --alphabet <STR>,       alphabet to use (String),    \n"
  "                                                                     \n"
  "     -rs <INT>, --seed <INT>,           seed in random generation,   \n"
  "     -dl <INT>, --delay <INT>,          visualization delay (ms),    \n"
  "     -tp <INT>, --top <INT>,            high complexity top tapes,   \n"
  "     -co <INT>, --ctx <INT>,            complexity context order,    \n"
  "     -th <DBL>, --threshold <DBL>,      threshold: NC(3) | NRC(4),   \n"
  "                                                                     \n"
  "     -md <INT>, --mode <INT>,           running SPARK modes:         \n"
  "                                          1 - School (simple),       \n"
  "                                          2 - School (advanced),     \n"
  "                                          3 - NC-Complexity top,     \n"
  "                                          4 - NRC search,            \n"
  "                                          5 - X search,              \n"
  "                                          6 - Impossible,            \n"
  "                                                                     \n"
  "     -ot <FILE>, --output-tape <FILE>,  output TM tape to file,      \n"
  "     -ox <FILE>, --output-top  <FILE>,  output complexity top,       \n"
  "     -ir <FILE>, --input-rules <FILE>,  load input rules for TM,     \n"
  "                                                                     \n"
  "     -i <FILE>, --input <FILE>,         input sequence filename.     \n"
  "                                                                     \n"
  "EXAMPLES                                                             \n"
  "     ./SPARK --alphabet-size 5 --states-number 5 --input seq.txt     \n"
  "     ./SPARK --top 50 --mode 3 --states-number 9 --input seq.txt     \n"
  "                                                                     \n"
  "COPYRIGHT                                                            \n"
  "     Copyright 2017-2023, D. Pratas, IEETA, University of Aveiro.    \n"
  "     License GPLv3, GNU GPLv3 <http://gnu.org/licenses/gpl.html>.    \n"
  "                                                                     \n",
  VERSION, RELEASE, DEFT);
  }

void PrintAbout(void){
  fprintf(stderr,
  "                                                                       \n"
  "                          ===================                          \n"
  "                          |    SPARK %u.%u    |                        \n"
  "                          ===================                          \n"
  "                                                                       \n"
  "                  A tool for simulation or search for                  \n"
  "                  exact / approximate Turing machines                  \n"
  "                                                                       \n"
  "                    Copyright 2017-2023, D. Pratas,                    \n"
  "                   IEETA/DETI, University of Aveiro.                   \n"
  "                                                                       \n"
  "                 This is a Free software, under GPLv3.                 \n"
  "                                                                       \n"
  "You may redistribute copies of it under the terms of the GNU - General \n"
  "Public License v3 <http://www.gnu.org/licenses/gpl.html>. There is NOT \n"
  "ANY WARRANTY, to the extent permitted by law.\n\n", VERSION, RELEASE);
  }

void PrintVersion(void){
  fprintf(stderr, "%u.%u\n", VERSION, RELEASE);
  return;
  }


