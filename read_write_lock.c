#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "linked_list.h"
#include "read_write_lock.h"
#include "global.h"


unsigned long readWriteExecution(int total_operations, int member_ops, int insert_ops, int delete_ops, int thread_count){
    rwlock_data thread_data;
    thread_data.head=NULL;
    thread_data.thread_count=thread_count;
    thread_data.member_operations=member_ops;
    thread_data.insert_operations=insert_ops;
    thread_data.delete_operations=delete_ops;

    //inserting n random numbers
    int n=1000;
    int nodes=0;
    while (nodes < n){
        Insert(rand()%MAX, &thread_data.head);
        nodes++;
    }
    //creating threads
    pthread_t *thread_handles=malloc(thread_count*sizeof(pthread_t));
    // Initialize the read write lock
    pthread_rwlock_init(&thread_data.rwlock, NULL);
    thread_data.thread_id = (int *)malloc(sizeof(int) * thread_count);
    struct timeval start;
    struct timeval end;

    // Start time
    gettimeofday(&start, NULL);

    // Thread Creation
    int i =0;
    for (long thread = 0; thread < thread_count; thread++) {
        thread_data.thread_id[i]=i;
        pthread_create(&thread_handles[thread], NULL, thread_function_rw, (void*) &thread_data);
    }

    // Join threads
    
    for (long thread= 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    free(thread_handles);

    // End time
    gettimeofday(&end, NULL);

    pthread_rwlock_destroy(&thread_data.rwlock);

    unsigned long time = time_diff(&start, &end);

    Destroy(thread_data.head);
    return time;
}

void *thread_function_rw(void *args) {
    rwlock_data* thread_data=args;
    int thread_count = thread_data->thread_count;
    int insert_operations=thread_data->insert_operations;
    int mem_operations=thread_data->member_operations;
    int delete_operations=thread_data->delete_operations;

    int local_m = 0;
    int local_insert_operations = 0;
    int local_delete_operations = 0;
    int local_mem_operations = 0;

    int id = *(int *)thread_data->thread_id;

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

    int member_finished =0;
    int insert_finished=0;
    int delete_finished=0;

    while (count_tot < local_m) {

        int random_value = rand() % MAX;
        int operation = rand() % 3;

        // Member operation
        if (operation == 0 && member_finished == 0 ) {
            if(count_member < local_mem_operations){
                pthread_rwlock_rdlock(&thread_data->rwlock);
                Member(random_value, thread_data->head);
                pthread_rwlock_unlock(&thread_data->rwlock);
                count_member++;
            }else{
                member_finished=1;
            }

        }
            // Insert Operation
        else if (operation == 1 && insert_finished == 0 ) {
            if(count_insert < local_insert_operations){
                pthread_rwlock_wrlock(&thread_data->rwlock);
                Insert(random_value, &thread_data->head);
                pthread_rwlock_unlock(&thread_data->rwlock);
                count_insert++;   
            }else{
                insert_finished=1;
            }
 
        }

            // Delete Operation
        else if (operation == 2 && delete_finished ==0 ) {
            if(count_delete < local_delete_operations){
                pthread_rwlock_wrlock(&thread_data->rwlock);
                Delete(random_value, &thread_data->head);
                pthread_rwlock_unlock(&thread_data->rwlock);
                count_delete++;
            }else{
                 delete_finished=1;
            }
           
        }
        count_tot = count_insert + count_member + count_delete;
    }
    return NULL;
}
