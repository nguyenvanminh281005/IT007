#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

int products = 0;
int sells = 0;

sem_t sem;
pthread_mutex_t mutex;

void* producer(void* arg) {
    while (true) {
        sem_wait(&sem); // Wait until consumer posts a semaphore
        pthread_mutex_lock(&mutex); // Ensure mutual exclusion when accessing shared data
        if (sells < products) {
            sells++;
        }
        pthread_mutex_unlock(&mutex); // Unlock mutex after modification
    }
}

void* consumer(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex); // Lock mutex to modify shared resources
        if (products < sells + 945) { // Ensure products are updated based on MSSV
            products++;
        }
        pthread_mutex_unlock(&mutex); // Unlock mutex after modification
        sem_post(&sem); // Signal the producer that it can consume
    }
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize synchronization primitives
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem, 0, 0); // Initialize semaphore with value 0 to synchronize threads

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Simulate and print product and sell values
    for (int i = 0; i < 5; i++) {
        printf("Products: %d, Sells: %d\n", products, sells);
        usleep(500000); // Sleep for 0.5 seconds (usleep uses microseconds)
    }

    // Clean up
    sem_destroy(&sem);
    pthread_mutex_destroy(&mutex);

    return 0;
}
