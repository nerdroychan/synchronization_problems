#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Reader and writer count
#define NR 5
#define NW 3

#define sleep(int) {}

// The reader counter
int readers = 0;

// Mutex for the reader counter
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Mutex for accessing the target
pthread_mutex_t alock = PTHREAD_MUTEX_INITIALIZER;

/*
  Reader thread function
*/
void* reader(int* id) {
    while (1) {
        // Exclusively access the reader counter
        pthread_mutex_lock(&lock);
        readers++;
        // If this thread is the first reader in the queue,
        // it will wait for the access lock.
        if (readers == 1) {
            pthread_mutex_lock(&alock);
        }
        pthread_mutex_unlock(&lock);

        // Start to read, now this thread is either:
        // - The first reader and has gain the access lock
        // - The reader after the first reader
        printf("Reader %d is reading\n", *id); fflush(stdout);
        sleep(3);

        // Exclusively access the reader counter
        pthread_mutex_lock(&lock);
        readers--;
        // If no more readers, release the access lock
        if (readers == 0) {
            pthread_mutex_unlock(&alock);
        }
        pthread_mutex_unlock(&lock);

        // Have a rest
        sleep(6);
    }
}

/*
  Writer thread function
*/
void* writer(int* id) {
    while (1) {
        // Waiting for the access lock
        pthread_mutex_lock(&alock);
        // Start to write, not this thread is the only writer
        // and no other threads are reading
        printf("Writer %d is writing\n", *id); fflush(stdout);
        sleep(6);
        // Release the access lock
        pthread_mutex_unlock(&alock);

        // Have a rest
        sleep(8);
    }
}

int main() {
    int r_ids[NR];
    int w_ids[NW];

    pthread_t r_threads[NR];
    for (int i=0; i<NR; i++) {
        r_ids[i] = i;
        pthread_create(&r_threads[i], NULL, (void*)reader, &r_ids[i]);
    }

    pthread_t w_threads[NW];
    for (int i=0; i<NW; i++) {
        w_ids[i] = i;
        pthread_create(&w_threads[i], NULL, (void*)writer, &w_ids[i]);
    }

    while (1);
    return 0;
}