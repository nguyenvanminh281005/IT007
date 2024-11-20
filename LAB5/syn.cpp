#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_ITEMS 945

int arr[MAX_ITEMS] = {0};
int count = 0;
sem_t sem;

void* producer(void* arg) {
    srand(23520945);
    int r;

    while (true) {
        sem_wait(&sem);

        if (count < MAX_ITEMS) {
            arr[count] = rand();
            count++;
            printf("Producer: Count is %d\n", count);
        }

        sem_post(&sem);
    }
}

void* consumer(void* arg) {
    while (true) {
        sem_wait(&sem);

        if (count == 0) {
            printf("Consumer: There's nothing to consume\n");
        } else {
            count--;
            printf("Consumer: Count is %d\n", count);
        }

        sem_post(&sem);
    }
}

int main() {
    pthread_t producer_thread, consumer_thread;

    sem_init(&sem, 0, 1);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    for (int i = 0; i < 60; i++) {
        usleep(500000);
    }

    return 0;
}
