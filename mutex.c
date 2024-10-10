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
void *thread_function(void * rank);
// int total_operations_count = 0;
// int member_operations_count = 0;
// int insert_operations_count = 0;
// int delete_operations_count = 0;
list_node_s *head_m = NULL;
extern int tot_operations;
extern int mem_operations, insert_operations, delete_operations,thread_count;

unsigned long mutexExecution(int total_operations,int member_ops,int insert_ops,int delete_ops,int threads){
    tot_operations = total_operations;
    mem_operations = member_ops;
    insert_operations = insert_ops;
    delete_operations = delete_ops;
    thread_count=threads;

    int n=1000;
    int *thread_id;
    thread_id = (int *)malloc(sizeof(int) * thread_count);
    //inserting n random numbers
     int i = 0;
    while (i < n) {
        if (Insert(rand() % 65535, &head_m) == 1)
            i++;
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
    i = 0;
    while (i < thread_count) {
        thread_id[i] = i;
        pthread_create(&thread_handles[i], NULL, (void *) thread_function, (void *) &thread_id[i]);
        i++;
    }

    // Thread Join
    i = 0;
    while (i < thread_count) {
        pthread_join(thread_handles[i], NULL);
        i++;
    }
    free(thread_handles);

    gettimeofday(&end, NULL);

    pthread_mutex_destroy(&mutex);
    Destroy(head_m);

    unsigned long time = time_diff(&start, &end);
    return time;
}
void *thread_function(void* thread_id){
     int local_m = 0;
    int local_insert_operations = 0;
    int local_delete_operations = 0;
    int local_mem_operations = 0;

    int id = *(int *)thread_id;
    // Generate local no of insertion operations without loss
    if (insert_operations % thread_count == 0 || insert_operations % thread_count <= id) {
        local_insert_operations = insert_operations / thread_count;
    }
    else if (insert_operations % thread_count > id) {
        local_insert_operations = insert_operations / thread_count + 1;
    }

    // Generate local no of member operations without loss
    if (mem_operations % thread_count == 0 || mem_operations % thread_count <= id) {
        local_mem_operations = mem_operations / thread_count;
    }
    else if (mem_operations % thread_count > id) {
        local_mem_operations = mem_operations / thread_count + 1;
    }

    // Generate local no of deletion operations without loss
    if (delete_operations % thread_count == 0 || delete_operations % thread_count <= id) {
        local_delete_operations = delete_operations / thread_count;
    }
    else if (delete_operations % thread_count > id) {
        local_delete_operations = delete_operations / thread_count + 1;
    }

    local_m = local_insert_operations + local_delete_operations + local_mem_operations;

    int count_tot = 0;
    int count_member = 0;
    int count_insert = 0;
    int count_delete = 0;

    while (count_tot < local_m) {

        int random_value = rand() % MAX;
        int operation = rand() % 3;

        // Member operation
        if (operation == 0 && count_member < local_mem_operations) {
                pthread_mutex_lock(&mutex);
                Member(random_value, head_m);
                count_member++;
                pthread_mutex_unlock(&mutex);
        }
            // Insert Operation
        else if (operation == 1 && count_insert < local_insert_operations) {
                pthread_mutex_lock(&mutex);
                Insert(random_value, &head_m);
                count_insert++;    
                pthread_mutex_unlock(&mutex);
            
        }

            // Delete Operation
        else if (operation == 2 && count_delete < local_delete_operations) {
                pthread_mutex_lock(&mutex);
                Delete(random_value, &head_m);
                count_delete++;
                pthread_mutex_unlock(&mutex);
              
        }
        count_tot = count_insert + count_member + count_delete;
    }
    return NULL;
}