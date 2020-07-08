#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char **arv){
    int child = fork();
    int x = 5;
    if(child==0){
        x+=5;
        printf("Child == 0......%d\n",x);
        printf("Process is......%d\n",child);
    }else{
        child=fork();
        x+=10;
        if(child){
            x+=5;
            printf("If Child......%d\n",x);
            printf("Process IF is.....%d\n",child);
        }
    }
}
