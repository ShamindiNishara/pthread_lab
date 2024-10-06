#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "linked_list.h"
#include "read_write_lock.h"

unsigned long readWriteExecution(int total_operations, int member_frac, int insert_frac, int delete_frac, int thread_count){

    rw_lock_data thread_data;

    thread_data.head = NULL;

    thread_data.thread_count = thread_count;

    // Fractions of each operation
    thread_data.member_frac = member_frac;
    thread_data.insert_frac = insert_frac;
    thread_data.delete_frac = delete_frac;

    int n = 1000; // Number of nodes in linked list
    int nodes = 0;

    // Randomly insert linked list nodes
    while (nodes < n){
        Insert(rand()%MAX, &thread_data.head);
        nodes++;
    }

    // Initialize the read write lock
    pthread_rwlock_init(&thread_data.rwlock, NULL);

    // Allocate memory for threads
    pthread_t *thread_handles = malloc(thread_count * sizeof(pthread_t));
    thread_data.thread_id = (int *)malloc(sizeof(int) * thread_count);

    struct timeval start;
    struct timeval end;

    // Start time
    gettimeofday(&start, NULL);

    // Generate threads
    int i = 0;
    for (long thread=0; thread < thread_count; thread++){
        thread_data.thread_id[i] = i;
        pthread_create(&thread_handles[thread], NULL, threadFuncReadWrite, (void*) &thread_data);
    }

    i = 0;
    for (long thread=0; thread < thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
        i++;
    }

    free(thread_handles);

    // End time
    gettimeofday(&end, NULL);

    pthread_rwlock_destroy(&thread_data.rwlock);

    unsigned long time = time_diff(&start, &end);

    Destroy(thread_data.head);
    printf("RW lock run complete\n");
    return time;
}

void *threadFuncReadWrite(void *args){

    rw_lock_data* thread_data = args;

    int thread_count = thread_data->thread_count;

    int m_member = thread_data->member_frac;
    int m_insert = thread_data->insert_frac;
    int m_delete = thread_data->delete_frac;

    int local_m;
    int local_m_insert = 0;
    int local_m_delete = 0;
    int local_m_member = 0;

    int id = *(int *)thread_data->thread_id;

    // Generate local no of insert operations without loss
    if (m_insert % thread_count == 0 || m_insert % thread_count <= id) {
        local_m_insert = m_member / thread_count;
    }
    else if (m_insert % thread_count > id) {
        local_m_insert = m_insert / thread_count + 1;
    }

    // Generate local no of member operations without loss
    if (m_member % thread_count == 0 || m_member % thread_count <= id) {
        local_m_member = m_member / thread_count;
    }
    else if (m_member % thread_count > id) {
        local_m_member = m_member / thread_count + 1;
    }

    // Generate local no of delete operations without loss
    if (m_delete % thread_count == 0 || m_delete % thread_count <= id) {
        local_m_delete = m_delete / thread_count;
    }
    else if (m_delete % thread_count > id) {
        local_m_delete = m_delete / thread_count + 1;
    }

    local_m = local_m_insert + local_m_delete + local_m_member;

    int count_tot = 0;
    int count_member = 0;
    int count_insert = 0;
    int count_delete = 0;

    int finished_member = 0;
    int finished_insert = 0;
    int delete_finished = 0;

    int i = 0;
    while (count_tot < local_m){

        // Generate random number
        int rand_value = rand() % MAX;

        // Generate random operation number
        int op = rand() % 3;


        if (op==0 && finished_member == 0){
            if (count_member < local_m_member){
                /* Critical section */
                pthread_rwlock_rdlock(&thread_data->rwlock);
                Member(rand_value, thread_data->head);
                pthread_rwlock_unlock(&thread_data->rwlock);
                count_member++;
            }else
                finished_member = 1;
        }
        else if(op==1 && finished_insert == 0){
            if (count_insert < local_m_insert){
                /* Critical section */
                pthread_rwlock_wrlock(&thread_data->rwlock);
                Insert(rand_value, &thread_data->head);
                pthread_rwlock_unlock(&thread_data->rwlock);
                count_insert++;
            }else
                finished_insert = 1;
        }else if(op==2 && delete_finished == 0){
            if (count_delete < local_m_delete){
                /* Critical section */
                pthread_rwlock_wrlock(&thread_data->rwlock);
                Delete(rand_value, &thread_data->head);
                pthread_rwlock_unlock(&thread_data->rwlock);
                count_delete++;
            }else
                delete_finished = 1;
        }
        count_tot = count_insert + count_member + count_delete;
        i++;
    }
    return NULL;
}

