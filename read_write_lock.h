#ifndef LAB1_RWLOCK_H
#define LAB1_RWLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "linked_list.h"
#include "global.h"

void *threadFuncReadWrite(void* rank);

struct rw_lock_data {
    pthread_rwlock_t rwlock;
    list_node_s *head;
    int *thread_id;

    int thread_count;

    int member_frac;
    int insert_frac;
    int delete_frac;
};
typedef struct rw_lock_data rw_lock_data;

unsigned long readWriteExecution(int total_operations, int member_frac, int insert_frac, int delete_frac, int thread_count);

#endif //LAB1_RWLOCK_H
