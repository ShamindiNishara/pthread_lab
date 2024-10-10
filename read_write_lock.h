#ifndef LAB1_RWLOCK_H
#define LAB1_RWLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "linked_list.h"
#include "global.h"
void *thread_function_rw(void *args);
unsigned long readWriteExecution(int total_operations, int member_ops, int insert_ops, int delete_ops, int thread_count);
struct rwlock_data{
    pthread_rwlock_t rwlock;
    list_node_s *head;
    int *thread_id;

    int thread_count;
    int member_operations;
    int insert_operations;
    int delete_operations;
};
typedef struct rwlock_data rwlock_data;

#endif //LAB1_RWLOCK_H
