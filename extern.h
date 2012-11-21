#ifndef EXTERN_H
#define EXTERN_H

#include "common.h"
#include <setjmp.h>

extern struct JOB *jobs_list;
extern struct JOB *cur_job;
extern jmp_buf sjbuf;


#endif