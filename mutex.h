#ifndef LAB1_MUTEX_H
#define LAB1_MUTEX_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "linked_list.h"
#include "global.h"

unsigned long mutexExecution(int total_operations,int member_ops,int insert_ops,int delete_ops,int threads);
void *thread_function(void* thread_id);
#endif //LAB1_MUTEX_H