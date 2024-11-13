#include <stdio.h>
#include <stdlib.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct {
    int pid;
    int arrival, burst;
    int start, finish, waiting, response, tat;
} PCB;

int get_information(PCB q, int criteria) {
    switch (criteria) {
        case SORT_BY_ARRIVAL: return q.arrival;
        case SORT_BY_PID: return q.pid;
        case SORT_BY_BURST: return q.burst;
        case SORT_BY_START: return q.start;
    }
    return -1;
}

void input_process(int n, PCB p[]) {
    for (int i = 0; i < n; i++) {
        printf("Input PID of %d-th process: ", i + 1);
        scanf("%d", &p[i].pid);
        printf("Input Arrival time of %d process: ", p[i].pid);
        scanf("%d", &p[i].arrival);
        printf("Input Burst time %d process: ", p[i].pid);
        scanf("%d", &p[i].burst);
    }
}

void print_process(int n, PCB p[]) {
    for (int i = 0; i < n; i++) {
        printf("===== P%d =====\n", p[i].pid);
        printf("Start time: %d\n", p[i].start);
        printf("Finish time: %d\n", p[i].finish);
        printf("Waiting time: %d\n", p[i].waiting);
        printf("Response time: %d\n", p[i].response);
        printf("Turnaround time: %d\n\n", p[i].tat);
    }
}

void export_gantt_chart(int n, PCB p[]) {
    printf("Gantt Chart:\n ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p[i].burst; j++) printf("--");
        printf(" ");
    }
    printf("\n|");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p[i].burst - 1; j++) printf(" ");
        printf("P%d", p[i].pid);
        for (int j = 0; j < p[i].burst - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p[i].burst; j++) printf("--");
        printf(" ");
    }
    printf("\n0");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p[i].burst; j++) printf("  ");
        if (p[i].finish > 9) printf("\b");
        printf("%d", p[i].finish);
    }
    printf("\n");
}

void push_process(int *n, PCB p[], PCB q) {
    p[(*n)++] = q;
}

void remove_process(int *n, int index, PCB p[]) {
    for (int i = index; i + 1 < *n; i++) p[i] = p[i + 1];
    (*n)--;
}

void swap_process(PCB *p, PCB *q) {
    PCB tmp = *p;
    *p = *q;
    *q = tmp;
}

void quick_sort(PCB p[], int low, int high, int criteria) {
    if (low >= high) return;
    int pivot = (low + high) / 2, left = low, right = high;

    while (right - left + 1 >= 3) {
        while (left < pivot && get_information(p[left], criteria) <= get_information(p[pivot], criteria)) left++;
        while (pivot < right && get_information(p[pivot], criteria) < get_information(p[right], criteria)) right--;

        if (left < pivot && pivot < right) {
            swap_process(&p[left], &p[right]);
            left++;
            right--;
        } else {
            int pos = (left + right) / 2;
            swap_process(&p[pos], &p[pivot]);
            swap_process(&p[pos], &p[pivot]);
        }
    }
    if (get_information(p[left], criteria) > get_information(p[right], criteria)) {
        if (left == pivot) pivot++;
        else pivot--;
        swap_process(&p[left], &p[right]);
    }
    quick_sort(p, low, pivot - 1, criteria);
    quick_sort(p, pivot + 1, high, criteria);
}

void calculate_avg_times(int n, PCB p[], int type) {
    const char *labels[] = {"Waiting time", "Turnaround time", "Response time"};
    int sum = 0;
    printf("\nProcess\t\t");
    for (int i = 0; i < n; i++) printf("\tP%d", p[i].pid);
    printf("\n%s:\t", labels[type]);
    for (int i = 0; i < n; i++) {
        int time = type == 0 ? p[i].waiting : type == 1 ? p[i].tat : p[i].response;
        printf("\t%d", time);
        sum += time;
    }
    printf("\nAvg %s: %.2f\n", labels[type], sum / (float)n);
}

int main() {
    PCB input[100], ready_queue[100], terminated[100];
    int num_process, remaining, ready = 0, terminated_count = 0;

    printf("Please input number of Process: ");
    scanf("%d", &num_process);

    remaining = num_process;
    input_process(num_process, input);
    quick_sort(input, 0, num_process - 1, SORT_BY_ARRIVAL);
    print_process(num_process, input);

    push_process(&ready, ready_queue, input[0]);
    remove_process(&remaining, 0, input);

    ready_queue[0].start = ready_queue[0].arrival;
    ready_queue[0].finish = ready_queue[0].start + ready_queue[0].burst;
    ready_queue[0].response = ready_queue[0].start - ready_queue[0].arrival;
    ready_queue[0].waiting = ready_queue[0].response;
    ready_queue[0].tat = ready_queue[0].finish - ready_queue[0].arrival;

    while (terminated_count < num_process) {
        while (remaining > 0 && input[0].arrival <= ready_queue[0].finish) {
            push_process(&ready, ready_queue, input[0]);
            remove_process(&remaining, 0, input);
        }
        if (ready > 0) {
            push_process(&terminated_count, terminated, ready_queue[0]);
            remove_process(&ready, 0, ready_queue);
            ready_queue[0].start = terminated[terminated_count - 1].finish;
            ready_queue[0].finish = ready_queue[0].start + ready_queue[0].burst;
            ready_queue[0].response = ready_queue[0].start - ready_queue[0].arrival;
            ready_queue[0].waiting = ready_queue[0].response;
            ready_queue[0].tat = ready_queue[0].finish - ready_queue[0].arrival;
        }
    }

    printf("\n===== FCFS Scheduling =====\n");
    export_gantt_chart(terminated_count, terminated);
    quick_sort(terminated, 0, terminated_count - 1, SORT_BY_PID);

    print_process(terminated_count, terminated);
    calculate_avg_times(terminated_count, terminated, 2);  // ART
    calculate_avg_times(terminated_count, terminated, 0);  // AWT
    calculate_avg_times(terminated_count, terminated, 1);  // ATaT
    printf("\n");

    return 0;
}
