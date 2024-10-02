#include "linked_list.h"
#include "global.h"
#include "mutex.h"
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

#define MAX 65535

// Shared data

pthread_mutex_t mutex;  
list_node_s *head = NULL;
void *thread_function(void * rank);
int total_operations_count = 0;
int member_operations_count = 0;
int insert_operations_count = 0;
int delete_operations_count = 0;

int tot_operations;
int mem_fraction, insert_fraction, delete_fraction;

unsigned long mutexExecution(int total_operations,int member_frac,int insert_frac,int delete_frac,int thread_count){
    
    tot_operations = total_operations;
    mem_fraction = member_frac;
    insert_fraction = insert_frac;
    delete_fraction = delete_frac;

    int n=1000;

    //inserting n random numbers
    for(int nodes=0;nodes<n;nodes++){
        Insert(rand()%MAX,&head);
    }
    //creating threads
    pthread_t *thread_handles=malloc(thread_count*sizeof(pthread_t));
    //initializing mutex
    pthread_mutex_init(&mutex,NULL);
  
    struct timeval start;
    struct timeval end;

    // Start time
    gettimeofday(&start, NULL);
    //creating threads
    for(int thread=0;thread<thread_count;thread++){
        pthread_create(&thread_handles[thread],NULL,thread_function,(void*)total_operations);
    }
    //joining threads
    for(int thread=0;thread<thread_count;thread++){
        pthread_join(thread_handles[thread],NULL);
    }


    Destroy(head);
    pthread_mutex_destroy(&mutex); 
    free(thread_handles);

    gettimeofday(&end, NULL);

    unsigned long elapsed_time = time_diff(&start, &end);
    return elapsed_time;
}
void *thread_function(void* rank){
while(total_operations_count<tot_operations){
    int rand_value = rand() % MAX;
    int op = rand() % 3;
    pthread_mutex_lock(&mutex);
    if(op==0 && insert_operations_count<insert_fraction){
        // pthread_mutex_lock(&mutex);
        Insert(rand_value,&head);
        insert_operations_count++;
        total_operations_count++;
        // pthread_mutex_unlock(&mutex);
    }
    else if(op==1 && member_operations_count<mem_fraction){
        // pthread_mutex_lock(&mutex);
        Member(rand_value,head);
        member_operations_count++;
        total_operations_count++;
        // pthread_mutex_unlock(&mutex);
    }
    else if(op==2 && delete_operations_count<delete_fraction){
        // pthread_mutex_lock(&mutex);
        Delete(rand_value,&head);
        delete_operations_count++;
        total_operations_count++;
        // pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_unlock(&mutex);
}
return NULL;
}