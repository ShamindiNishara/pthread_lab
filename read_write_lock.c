#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "linked_list.h"
#include "read_write_lock.h"
#include "global.h"

list_node_s *head = NULL;
pthread_rwlock_t rwlock;
void *Thread_Operation(void *thread_id);
int tot_operations;
int mem_operations, insert_operations, delete_operations;
int thread_count;

unsigned long readWriteExecution(int total_operations, int member_ops, int insert_ops, int delete_ops, int threads){
    tot_operations = total_operations;
    mem_operations = member_ops;
    insert_operations = insert_ops;
    delete_operations = delete_ops;
    thread_count = threads;
    // operationss of each operation
    int n=1000;
    int *thread_id;
    thread_id = (int *)malloc(sizeof(int) * thread_count);

    //inserting n random numbers
    int i = 0;
    while (i < n) {
        if (Insert(rand() % MAX, &head) == 1)
            i++;
    }
    //creating threads
    pthread_t *thread_handles=malloc(thread_count*sizeof(pthread_t));
    // Initialize the read write lock
    pthread_rwlock_init(&rwlock, NULL);

    struct timeval start;
    struct timeval end;

    // Start time
    gettimeofday(&start, NULL);

    // Thread Creation
    i = 0;
    while (i < thread_count) {
        thread_id[i] = i;
        pthread_create(&thread_handles[i], NULL, (void *) Thread_Operation, (void *) &thread_id[i]);
        i++;
    }

    // Thread Join
    i = 0;
    while (i < thread_count) {
        pthread_join(thread_handles[i], NULL);
        i++;
    }

    free(thread_handles);

    // End time
    gettimeofday(&end, NULL);

    pthread_rwlock_destroy(&rwlock);

    unsigned long time = time_diff(&start, &end);

    Destroy(head);
    return time;
}

void *Thread_Operation(void *thread_id) {

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
                pthread_rwlock_rdlock(&rwlock);
                Member(random_value, head);
                pthread_rwlock_unlock(&rwlock);
                count_member++;
        }
            // Insert Operation
        else if (operation == 1 && count_insert < local_insert_operations) {
                pthread_rwlock_wrlock(&rwlock);
                Insert(random_value, &head);
                pthread_rwlock_unlock(&rwlock);
                count_insert++;    
        }

            // Delete Operation
        else if (operation == 2 && count_delete < local_delete_operations) {
                pthread_rwlock_wrlock(&rwlock);
                Delete(random_value, &head);
                pthread_rwlock_unlock(&rwlock);
                count_delete++;
        }
        count_tot = count_insert + count_member + count_delete;
    }
    return NULL;
}
