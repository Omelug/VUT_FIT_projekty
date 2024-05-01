#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

sem_t *semafor;
int main(){
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    semafor = mmap(NULL, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    sem_init(semafor,1,0);
    
    int value;
    for (int i=0;i<10;i++) {
        int res=fork();
        if (res<0) {return 1;}
        if (res!=0) {
            sem_getvalue(semafor, &value);
            printf("%d\n",value);
            sem_wait(semafor);
            sem_post(semafor);
            printf("EXIT");
            exit(0);
        }
    }
    //sem_post(semafor);
    //while (wait(NULL))

    printf("\nKONEc\n");
    usleep(500);
    sem_post(semafor);
    usleep(5000);
};
