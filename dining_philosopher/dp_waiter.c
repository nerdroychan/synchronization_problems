#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NP 5

pthread_mutex_t forks[NP];
pthread_mutex_t waiter = PTHREAD_MUTEX_INITIALIZER;

void* philosopher(int* id) {
    int left = *id;
    int right = (*id+1)%NP;
    while (1) {
        printf("Philosopher %d started thinking\n", *id); fflush(stdout);
        pthread_mutex_lock(&waiter);
        int a = pthread_mutex_trylock(&forks[left]);
        int b = pthread_mutex_trylock(&forks[right]);
        if (a == 0 && b == 0) {
            printf("Philosopher %d starts to eat\n", *id); fflush(stdout);
        }
        pthread_mutex_unlock(&forks[right]);
        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&waiter);
    }
}

int main() {
    for (int i=0; i<NP; i++) {
        pthread_mutex_t _fork = PTHREAD_MUTEX_INITIALIZER;
        forks[i] = _fork;
    }

    int p_ids[NP];
    pthread_t p_threads[NP];
    for (int i=0; i<NP; i++) {
        p_ids[i] = i;
        pthread_create(&p_threads[i], NULL, (void*)philosopher, &p_ids[i]);
    }

    while (1);
    return 0;
}