#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid_a, pid_b;

    // Tạo tiến trình con A
    pid_a = fork();
    if (pid_a == 0) { // Tiến trình con A
        int x = 0;
        while (1) {
            printf("Process A: x = %d\n", x);
            x = (x + 1) % 20; // Tăng x và quay lại 0 khi đạt 20
            sleep(1); // Delay 1 giây
        }
    } else if (pid_a > 0) { // Tiến trình cha
        // Tạo tiến trình con B
        pid_b = fork();
        if (pid_b == 0) { // Tiến trình con B
            int x = 0;
            while (1) {
                printf("Process B: x = %d\n", x);
                x = (x + 1) % 20; // Tăng x và quay lại 0 khi đạt 20
                sleep(1); // Delay 1 giây
            }
        } else if (pid_b > 0) { // Tiến trình cha
            // Chờ cả hai tiến trình con kết thúc
            printf("Parent process waiting for child processes to finish...\n");
            waitpid(pid_a, NULL, 0); // Chờ tiến trình A
            waitpid(pid_b, NULL, 0); // Chờ tiến trình B
            printf("All child processes have finished. Exiting parent process.\n");
        } else {
            perror("Failed to fork process B");
            return 1;
        }
    } else {
        perror("Failed to fork process A");
        return 1;
    }

    return 0;
}
