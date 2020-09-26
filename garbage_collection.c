#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

/*
This program provides a possible solution for first readers writers problem using mutex and semaphore.
I have used 10 readers and 5 producers to demonstrate the solution. You can always play with these values.
*/

sem_t wrt;
pthread_mutex_t mutex;
int cnt = 1;
int numreader = 0;

void *garbage_sender(void *wno)
{   
    sem_wait(&wrt);
    cnt = cnt*2;
    printf("garbage_sender %d getting filled cnt to %d\n",(*((int *)wno)),cnt);
    sem_post(&wrt);

}
void *garbage_reciever(void *rno)
{   
    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader++;
    if(numreader == 1) {
        sem_wait(&wrt); // If this id the first reader, then it will block the writer
    }
    pthread_mutex_unlock(&mutex);
    // Reading Section
    printf("garbage_reciever: %d \n",*((int *)rno));

    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) {
        sem_post(&wrt); // If this is the last reader, it will wake up the writer.
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{   

    pthread_t garbage_send[4],garbage_recieve[4];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    int a[4] = {1,2,3,4}; //Just used for numbering the producer and consumer

    for(int i = 0; i < 4; i++) {
        pthread_create(&garbage_send[i], NULL, (void *)garbage_sender, (void *)&a[i]);
    }
    for(int i = 0; i < 4; i++) {
        pthread_create(&garbage_recieve[i], NULL, (void *)garbage_reciever, (void *)&a[i]);
    }

    for(int i = 0; i < 4; i++) {
        pthread_join(garbage_send[i], NULL);
    }
    for(int i = 0; i < 4; i++) {
        pthread_join(garbage_recieve[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
    
}
