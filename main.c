#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "serial.h"
#include "mutex.h"
#include "read_write_lock.h"

int main(){
int m=10000;
int mem_frac=m*0.99;
int insert_frac=m*0.005;
int delete_frac=m*0.005;
srand(time(0));
 unsigned long time_taken_for_serial = serialExecution(m, mem_frac, insert_frac, delete_frac);  
printf("Time taken for serial execution: %lu microseconds\n", time_taken_for_serial);
unsigned long time_taken_for_mutex = mutexExecution(m, mem_frac, insert_frac, delete_frac, 8);
printf("Time taken for mutex execution: %lu microseconds\n", time_taken_for_mutex);
unsigned long time_taken_for_rw_lock = readWriteExecution(m, mem_frac, insert_frac, delete_frac, 8);
printf("Time taken for read write lock execution: %lu microseconds\n", time_taken_for_rw_lock);
return 0;
}