#ifndef LAB1_RWLOCK_H
#define LAB1_RWLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "linked_list.h"
#include "global.h"

void *Thread_Operation(void *thread_id);

unsigned long readWriteExecution(int total_operations, int member_ops, int insert_ops, int delete_ops, int threads);

#endif //LAB1_RWLOCK_H
