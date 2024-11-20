#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid_a, pid_b;

    // Tạo tiến trình A
    pid_a = fork();
    if (pid_a == 0) { // Tiến trình con A
        int x = 0;
        while (1) {
            x = x + 1;
            if (x == 20) {
                x = 0;
            }
            printf("Process A: x = %d\n", x);
            sleep(1); // Delay 1 giây
        }
    }

    // Tạo tiến trình B
    pid_b = fork();
    if (pid_b == 0) { // Tiến trình con B
        int x = 0;
        while (1) {
            x = x + 1;
            if (x == 20) {
                x = 0;
            }
            printf("Process B: x = %d\n", x);
            sleep(1); // Delay 1 giây
        }
    }

    // Đợi các tiến trình con kết thúc
    waitpid(pid_a, NULL, 0);
    waitpid(pid_b, NULL, 0);

    return 0;
}