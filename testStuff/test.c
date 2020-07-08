#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ucontext.h>
#include <string.h>
#include <sys/time.h>
static ucontext_t ctx[3];

static void f1 (void){
    puts("start f1\n");
    swapcontext(&ctx[1],&ctx[2]);
    puts("finish f1\n");
}

static void f2 (void){
    puts("start f2\n");
    //swapcontext(&ctx[2],&ctx[1]);
    puts("finish f2\n");
}
void *myThreadFun(void *vargp){
    sleep(1);
    printf("In function Thread:    %d\n",pthread_self());
    
    printf("Printing GeeksQuiz from Thread \n");
    pthread_exit(NULL);
    return NULL;
}
void timer_handler(int sum){
    static int count=0;
    printf("time expired %d times\n",++count);
}

typedef uint my_pthread_t;

typedef enum threadStatus{
    RUNNABLE,
    SLEEP,
    FINISHED
} status_t;

typedef struct threadControlBlock{
    my_pthread_t tid;
    status_t status;
    ucontext_t context;
    struct threadControlBlock* next;
} my_pthread_tcb;

int main(void){ 
    /**
    pthread_t thread_id;
    printf("Before Thread\n");
    pthread_create(&thread_id, NULL, myThreadFun,NULL);
    printf("Main Thread?    %d\n", pthread_self());
    pthread_join(thread_id,NULL);
    printf("After Thread\n");
    **/
    
   // char st1[8192];
   // char st2[8192];
/**
    getcontext(&ctx[1]);
    ctx[1].uc_stack.ss_sp = st1;
    ctx[1].uc_stack.ss_size = sizeof(st1);
    ctx[1].uc_link = &ctx[0];
    makecontext(&ctx[1], f1, 0);
   **/
    /**
    getcontext(&ctx[1]);

    getcontext(&ctx[2]);
    ctx[2].uc_stack.ss_sp = st2;
    ctx[2].uc_stack.ss_size = sizeof(st2);
    ctx[2].uc_link = &ctx[1];
    makecontext(&ctx[2], f2, 0);

    swapcontext(&ctx[1], &ctx[2]);
    printf("Derp\n");
    **/
    /**
    ucontext_t mainCon;
    getcontext(&mainCon);

    my_pthread_tcb* newTCB = (my_pthread_tcb*) malloc(sizeof(my_pthread_tcb));
    memset(newTCB, 0 , sizeof(my_pthread_tcb));
    getcontext(&(newTCB->context));
    newTCB->context.uc_stack.ss_sp = malloc(1024*64);
    newTCB->context.uc_stack.ss_size = 1024*64;
    newTCB->context.uc_link = &mainCon;
  
    makecontext(&(newTCB->context),f2,0);

    swapcontext(&mainCon,&(newTCB->context));
**/
/**
    struct sigaction sa;
    struct itimerval timer;
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGVTALRM, &sa, NULL);
    
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 250000;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 250000;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);
    
    while(1);
    **/
    int i, rc;
    for(i = 0; i<=3; i++){
        if((rc = fork())==0){ // child process
            printf("Value of rc in the child process %d \n",rc);
            printf("Value of i in the child process........ %d\n",i);
        }
    }

    printf("All children created...............%d\n",rc);
    printf("Dope==============================\n");
    return 0;
}
