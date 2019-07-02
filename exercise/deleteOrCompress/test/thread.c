#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct
{
    char name[16];
    char msg[64];
}thread_args;


static void *printHello(void *args)
{
    thread_args *thr_args = (thread_args*)args;
    printf ("Hello World! I am: %s!\n", thr_args->name);
    printf ("%s!\n", thr_args->msg);

    pthread_exit(NULL);
}

int main (void)
{
    pthread_t threadID;
    int ret;
    long i = 0;
    thread_args thr;

    memset(&thr, 0x0, sizeof(thread_args));
    strncpy(thr.name, "vimentor", sizeof(thr.name));
	
    ret = pthread_create(&threadID, NULL, printHello, (&thr));
    if(ret)
    {
        printf("pthread_create() error number is %d\n", ret);
        return -1; 
    }
    pthread_exit(NULL);
}
