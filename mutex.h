#ifndef LAB1_MUTEX_H
#define LAB1_MUTEX_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "linked_list.h"
#include "global.h"

unsigned long mutexExecution(int total_operations,int member_ops,int insert_ops,int delete_ops,int thread_count);
void *thread_function(void* args);
struct mutex_data{
    pthread_mutex_t mutex;
    list_node_s *head;

    int tot_operations;
    int mem_operations;
    int insert_operations;
    int delete_operations;

    int mem_count;
    int insert_count;
    int delete_count;
    int total_count;
    };
typedef struct mutex_data mutex_data;
#endif //LAB1_MUTEX_H