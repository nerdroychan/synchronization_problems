#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int buffer = 0;
int CAPACITY = 20;

int nc = 5;
int np = 5;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void* consumer(int* id) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (buffer == 0) {
            pthread_cond_wait(&not_empty, &lock);
        }
        buffer--;
        printf("Consumer %d take 1 element, now has %d\n", *id, buffer);
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);
    }
}

void* producer(int* id) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (buffer == CAPACITY) {
            pthread_cond_wait(&not_full, &lock);
        }
        buffer++;
        printf("Producer %d give 1 element, now has %d\n", *id, buffer);
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);
    }
}

int main() {
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