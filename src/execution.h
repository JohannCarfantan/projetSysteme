#ifndef _EXECUTION_H_
#define _EXECUTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "divers.h"
#include "parse.h"


void execution_ligne_cmd(parse_info *info);
t_bool execution_cmd(parse_info *info, int debut, int nb_arg);
void execution_cmdPipe();

#endif
