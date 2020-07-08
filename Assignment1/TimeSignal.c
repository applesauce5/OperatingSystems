#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

struct timeval start, end;
long total, average;
int count;

void handle_sigfpe(int signum){
	
	// Handler code goes here
    //printf("Caught using signal:  %d \n", signum);
    if(count < 100000){
        count++;
    }else {
        gettimeofday(&end,NULL);
        long seconds = (end.tv_sec - start.tv_sec);
        total=((seconds*1000000)+end.tv_usec) - (start.tv_usec);
        printf("Syscalls Performed: %d\n", count);
        printf("Total Elapsed Time: %ld ms \n", total);
        float average = (float) total / (100000.0);
        printf("Average Time Per Syscall: %f ms\n", average);
        exit(0);
    }
}

int main(int argc, char *argv[]){
	
	/* 
	 *
	 * Implement Here
	 *
	 *
	 */
    int x=5;
    int y=0;
    int z=0;
    signal(SIGFPE, handle_sigfpe); //Overrides the current default for signal handling??
    gettimeofday(&start,NULL);
    z=x/y;
    printf("Value of z: %d\n",z);
    return 0;
}
