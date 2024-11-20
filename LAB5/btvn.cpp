#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// Khai báo biến chung
int x1, x2, x3, x4, x5, x6;
int w, v, y, z, ans;

// Khai báo semaphore
sem_t sem_b, sem_c_d, sem_e, sem_g;

void* calculate_a(void* arg) {
    w = x1 * x2;
    printf("Step (a): w = %d * %d = %d\n", x1, x2, w);
    sem_post(&sem_b); // Mở khóa (b)
    return NULL;
}

void* calculate_b(void* arg) {
    sem_wait(&sem_b); // Chờ (a) hoàn thành
    v = x3 * x4;
    printf("Step (b): v = %d * %d = %d\n", x3, x4, v);
    sem_post(&sem_c_d); // Mở khóa (c) và (d)
    sem_post(&sem_c_d);
    return NULL;
}

void* calculate_c(void* arg) {
    sem_wait(&sem_c_d); // Chờ (b) hoàn thành
    y = v * x5;
    printf("Step (c): y = %d * %d = %d\n", v, x5, y);
    sem_post(&sem_e); // Báo hiệu (e) có thể thực hiện
    return NULL;
}

void* calculate_d(void* arg) {
    sem_wait(&sem_c_d); // Chờ (b) hoàn thành
    z = v * x6;
    printf("Step (d): z = %d * %d = %d\n", v, x6, z);
    sem_post(&sem_g); // Báo hiệu (g) có thể thực hiện (z đã tính xong)
    return NULL;
}

void* calculate_e(void* arg) {
    sem_wait(&sem_e); // Chờ (a) và (c) hoàn thành
    y = w * y;
    printf("Step (e): y = %d * %d = %d\n", w, y, y);
    sem_post(&sem_g); // Báo hiệu (g) có thể thực hiện
    return NULL;
}

void* calculate_f(void* arg) {
    sem_wait(&sem_g); // Chờ (e) và (d) hoàn thành
    ans = y + z;
    printf("Step (f): z = %d * %d = %d\n", w, z, z);
    printf("Step (g): ans = %d + %d = %d\n", y, z, ans);
    return NULL;
}

int main() {
    // Khởi tạo các giá trị
    x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5, x6 = 6;

    // Khởi tạo semaphore
    sem_init(&sem_b, 0, 0);
    sem_init(&sem_c_d, 0, 0);
    sem_init(&sem_e, 0, 0);
    sem_init(&sem_g, 0, 0);

    // Khởi tạo thread
    pthread_t t_a, t_b, t_c, t_d, t_e, t_f;

    pthread_create(&t_a, NULL, calculate_a, NULL);
    pthread_create(&t_b, NULL, calculate_b, NULL);
    pthread_create(&t_c, NULL, calculate_c, NULL);
    pthread_create(&t_d, NULL, calculate_d, NULL);
    pthread_create(&t_e, NULL, calculate_e, NULL);
    pthread_create(&t_f, NULL, calculate_f, NULL);

    // Chờ các thread hoàn thành
    pthread_join(t_a, NULL);
    pthread_join(t_b, NULL);
    pthread_join(t_c, NULL);
    pthread_join(t_d, NULL);
    pthread_join(t_e, NULL);
    pthread_join(t_f, NULL);

    // Hủy semaphore
    sem_destroy(&sem_b);
    sem_destroy(&sem_c_d);
    sem_destroy(&sem_e);
    sem_destroy(&sem_g);

    return 0;
}
