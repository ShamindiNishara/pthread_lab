#include "linked_list.h"
#include "global.h"
#include "serial.h"
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#define MAX 65535

unsigned long serialExecution(int total_operations, int mem_frac, int insert_frac,int delete_frac){
    int member_operations_count=0;
    int insert_operations_count=0;
    int delete_operations_count=0;
    int total_operations_count=0;
     
    int n = 1000;
    list_node_s *head = NULL;
    //inserting n random numbers
    for (int nodes = 0; nodes < n; nodes++) {
    Insert(rand() % MAX, &head);
    }
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);

    while(total_operations_count<total_operations){
        int rand_value = rand() % MAX;
        int op=rand() % 3;
        if (op == 0 & insert_operations_count<insert_frac){
            Insert(rand_value, &head);
            insert_operations_count++;
            total_operations_count++;
        }
        else if (op==1 && member_operations_count<mem_frac){
            Member(rand_value, head);
            member_operations_count++;
            total_operations_count++;
        }
        else if (op==2 && delete_operations_count<delete_frac){
            Delete(rand_value, &head);
            delete_operations_count++;
            total_operations_count++;
        }
    }
 
    Destroy(head);
  
    gettimeofday(&end, NULL);

    unsigned long elapsed_time = time_diff(&start, &end);

    // Return the elapsed time
    return elapsed_time;
    printf("Serial Execution Done\n");


}