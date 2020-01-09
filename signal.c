#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void            *threadfunc(void *parm)
{
    int        threadnum;
    int        *tnum;
    sigset_t   set;

    tnum = parm;
    threadnum = *tnum;
    int sigStat;
    printf("Thread %d executing\n", threadnum);
    sigemptyset(&set);
    if(sigaddset(&set, SIGUSR1) == -1) {
        perror("Sigaddset error");
        pthread_exit((void *)1);
    }
    printf("waiting for signal from main()\n");
    if(sigwait(&set,&sigStat) != SIGUSR1) {
        perror("Sigwait error");
        pthread_exit((void *)2);
    }
    printf("Signal received\n");

    pthread_exit(0);
}

int main() {
    int          status;
    int          threadparm = 1;
    pthread_t    threadid;
    int          thread_stat;

    status = pthread_create( &threadid,
                             NULL,
                             threadfunc,
                             (void *)&threadparm);
    if ( status <  0) {
        perror("pthread_create failed");
        exit(1);
    }

    sleep(5);

    printf("Sending signal to thread\n");
    status = pthread_kill( threadid, SIGUSR1);
    if ( status <  0)
        perror("pthread_kill failed");

    printf("Signal to thread sent...waiting for thread to terminate\n");
    status = pthread_join( threadid, (void *)&thread_stat);
    if ( status <  0)
        perror("pthread_join failed");
    printf("thread terminated\n");
    exit(0);
}