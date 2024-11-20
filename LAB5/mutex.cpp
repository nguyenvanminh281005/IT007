#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

pthread_mutex_t mutex;
int x = 0;
bool stop = false;

void* process(void* arg) {
    while (!stop) {
        pthread_mutex_lock(&mutex);
        x = (x + 1) % 20;
        printf("%d\n", x);
        pthread_mutex_unlock(&mutex);
        usleep(100000);
    }
    return NULL;
}

int main() {
    pthread_t threadA, threadB;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&threadA, NULL, process, NULL);
    pthread_create(&threadB, NULL, process, NULL);

    sleep(5);

    stop = true;
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}
