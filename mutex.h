#ifndef LAB1_MUTEX_H
#define LAB1_MUTEX_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "linked_list.h"
#include "global.h"


unsigned long mutexExecution(int total_operations,int member_frac,int insert_frac,int delete_frac,int thread_count);
void *thread_function(void *rank);
#endif //LAB1_MUTEX_H