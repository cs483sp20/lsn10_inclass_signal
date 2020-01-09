#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void sigfunc() {
    return;
}

void* threadfunc(void *parm) {

    int        threadnum;
    int        *tnum;
    sigset_t   set;
    struct sigaction sigAct;

    sigAct.sa_handler = &sigfunc;
    tnum = parm;
    threadnum = *tnum;
    int sigStat;

    sigaction(SIGUSR1,&sigAct,NULL);
    sigemptyset(&(sigAct.sa_mask));
    sigAct.sa_flags = 0;

    printf("Thread %d executing\n", threadnum);
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    printf("waiting for signal from main()\n");
    sigwait(&set,&sigStat);
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

    sleep(1);

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