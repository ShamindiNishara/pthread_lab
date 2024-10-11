#include "linked_list.h"
#include "global.h"
#include "mutex.h"
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

unsigned long mutexExecution(int total_operations,int member_ops,int insert_ops,int delete_ops,int thread_count){
    mutex_data thread_data;
    thread_data.head=NULL;

    thread_data.tot_operations=total_operations;
    thread_data.mem_operations=member_ops;
    thread_data.insert_operations=insert_ops;
    thread_data.delete_operations=delete_ops;
   
    thread_data.mem_count=0;
    thread_data.insert_count=0;
    thread_data.delete_count=0;
    thread_data.total_count=0;

    int n=1000;
 
    //inserting n random numbers
     int nodes = 0;
    while (nodes < n) {
        Insert(rand() % 65535, &thread_data.head);
        nodes++;
    }

    pthread_t *thread_handles=malloc(thread_count*sizeof(pthread_t));
    //initializing mutex
    pthread_mutex_init(&thread_data.mutex,NULL);
  
    struct timeval start;
    struct timeval end;

    // Start time
    gettimeofday(&start, NULL);
    //creating threads
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, thread_function, (void*) &thread_data);
    }

    // Join threads
    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
    }
    free(thread_handles);

    gettimeofday(&end, NULL);

    pthread_mutex_destroy(&thread_data.mutex);
    Destroy(thread_data.head);

    unsigned long time = time_diff(&start, &end);
    return time;
}
void *thread_function(void* args){
    mutex_data* thread_data=args;
    

    while (thread_data->total_count < thread_data->tot_operations) {

        int random_value = rand() % MAX;
        int operation = rand() % 3;

        // Member operation
        if (operation == 0 && thread_data->mem_count < thread_data->mem_operations) {
            if(thread_data->total_count < thread_data->tot_operations){
                pthread_mutex_lock(&thread_data->mutex);
                Member(random_value, thread_data->head);
                thread_data->mem_count++;
                thread_data->total_count++;
                pthread_mutex_unlock(&thread_data->mutex);
            }

        }
            // Insert Operation
        else if (operation == 1 && thread_data->insert_count < thread_data->insert_operations) {
               if(thread_data->total_count < thread_data->tot_operations){
                pthread_mutex_lock(&thread_data->mutex);
                Insert(random_value, &thread_data->head);
                thread_data->insert_count++;
                thread_data->total_count++;
                pthread_mutex_unlock(&thread_data->mutex); 
            }}

            // Delete Operation
        else if (operation == 2 && thread_data->delete_count < thread_data->delete_operations) {
               if(thread_data->total_count < thread_data->tot_operations){
                pthread_mutex_lock(&thread_data->mutex);
                Delete(random_value, &thread_data->head);
                thread_data->delete_count++;
                thread_data->total_count++;
                pthread_mutex_unlock(&thread_data->mutex);
            }   
        }
       
    }
    return NULL;
}