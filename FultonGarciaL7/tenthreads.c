/*  Name: Fulton Garcia
*   PantherID: 5464347
*   This program increments a shared value between 10 threads
*   I affirm that I wrote this program myself without any help form any other people or sources from the internet.
*/
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid[10];
long int shared_var = 10;
pthread_mutex_t mutex_lock;

void* increment_shared_var(void *arg){
//aquiring mutex lock
pthread_mutex_lock(&mutex_lock);

unsigned long i = 0;

//getting the thread ID
int thread_id = (int)pthread_self();

//incrementing shared variable with thread ID
for(i = 0; i < 6; i++){
    shared_var = shared_var + thread_id;
}


for(i = 0; i < (0xFFFFFFFF); i++);

//printing thread ID
printf("\n Thread %d has finished incrementing\n", thread_id);

//releasing mutex lock
pthread_mutex_unlock(&mutex_lock);

return NULL;
}

int main(void){
int i = 0;
int val;

//initiating mutex lock
if (pthread_mutex_init(&mutex_lock, NULL) != 0){
    printf("\n mutex initiation has failed\n");
    return 1;
}

//spawning threads
for(i = 0; i < 10; i++){
    val = pthread_create(&(tid[i]), NULL, &increment_shared_var, NULL);
    if (val != 0){
        printf("\nThread can't be created :[%s]", strerror(val));
    }
}

pthread_join(tid[0], NULL);
pthread_join(tid[1], NULL);
pthread_join(tid[2], NULL);
pthread_join(tid[3], NULL);
pthread_join(tid[4], NULL);
pthread_join(tid[5], NULL);
pthread_join(tid[6], NULL);
pthread_join(tid[7], NULL);
pthread_join(tid[8], NULL);
pthread_join(tid[9], NULL);
pthread_mutex_destroy(&mutex_lock);

//printing final shared variable value
printf("\n Shared variable value %li \n", shared_var);

return 0;
}