#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
int main(int argc, char *argv[]){
	struct timeval start, end;
    
	/* 
	 *
	 * Implement Here
	 *
	 *
	 */
    int i=0;
    long total=0;
    while(i < 100000){
        gettimeofday(&start,NULL);
        getpid();
        gettimeofday(&end,NULL);
        long seconds = (end.tv_sec - start.tv_sec);
        long micros = ((seconds * 1000000) + end.tv_usec)-(start.tv_usec);
        total+=micros;
        i++;
    }
    printf("Syscalls Performed: %d\n", i);
    printf("Total Elapsed Time: %ld ms\n", total);
    float average = (float) total / (100000.0);
    printf("Average Time Per Syscall: %f ms\n", average);
	return 0;

}
