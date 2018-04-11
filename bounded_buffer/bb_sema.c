#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int buffer = 0;
int CAPACITY = 20;

int nc = 5;
int np = 5;

sem_t* buffer_element;
sem_t* buffer_remain;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* consumer(int* id) {
    while (1) {
        sem_wait(buffer_element);
        pthread_mutex_lock(&lock);
        buffer--;
        printf("Consumer %d take 1 element, now has %d\n", *id, buffer);
        pthread_mutex_unlock(&lock);
        sem_post(buffer_remain);
    }
}

void* producer(int* id) {
    while (1) {
        sem_wait(buffer_remain);
        pthread_mutex_lock(&lock);
        buffer++;
        printf("Producer %d give 1 element, now has %d\n", *id, buffer);
        pthread_mutex_unlock(&lock);
        sem_post(buffer_element);
    }
}

int main() {
    sem_unlink("buffer_element");
    sem_unlink("buffer_remain");

    buffer_element = sem_open("buffer_element", O_CREAT, S_IRUSR | S_IWUSR, 0);
    buffer_remain = sem_open("buffer_remain", O_CREAT, S_IRUSR | S_IWUSR, CAPACITY);
    
    int c_ids[nc];
    int p_ids[np];

    pthread_t c_threads[nc];
    for (int i=0; i<nc; i++) {
        c_ids[i] = i;
        pthread_create(&c_threads[i], NULL, (void*)consumer, &c_ids[i]);
    }

    pthread_t p_threads[np];
    for (int i=0; i<np; i++) {
        p_ids[i] = i;
        pthread_create(&c_threads[i], NULL, (void*)producer, &p_ids[i]);
    }

    while (1);
    return 0;
}