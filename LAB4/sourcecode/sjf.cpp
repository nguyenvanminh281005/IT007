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

int get_information(PCB process, int criteria) {
    if (criteria == SORT_BY_ARRIVAL) return process.arrival;
    if (criteria == SORT_BY_PID) return process.pid;
    if (criteria == SORT_BY_BURST) return process.burst;
    if (criteria == SORT_BY_START) return process.start;
    return 0;
}

void input_process(int n, PCB processes[]) {
    for (int i = 0; i < n; i++) {
        printf("Input PID of %d-th process: ", i + 1);
        scanf("%d", &processes[i].pid);
        printf("Input Arrival time of process %d: ", processes[i].pid);
        scanf("%d", &processes[i].arrival);
        printf("Input Burst time of process %d: ", processes[i].pid);
        scanf("%d", &processes[i].burst);
    }
}

void print_process(int n, PCB processes[]) {
    for (int i = 0; i < n; i++) {
        printf("===== P%d =====\n", processes[i].pid);
        printf("Arrival Time: %d\n", processes[i].arrival);
        printf("Burst Time: %d\n", processes[i].burst);
        printf("Start Time: %d\n", processes[i].start);
        printf("Finish Time: %d\n", processes[i].finish);
        printf("Waiting Time: %d\n", processes[i].waiting);
        printf("Response Time: %d\n", processes[i].response);
        printf("Turnaround Time: %d\n\n", processes[i].tat);
    }
}

void export_gantt_chart(int n, PCB processes[]) {
    printf("Gantt Chart:\n ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) printf("--");
        printf(" ");
    }
    printf("\n|");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst - 1; j++) printf(" ");
        printf("P%d", processes[i].pid);
        for (int j = 0; j < processes[i].burst - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) printf("--");
        printf(" ");
    }
    printf("\n0");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) printf("  ");
        if (processes[i].finish > 9) printf("\b");
        printf("%d", processes[i].finish);
    }
    printf("\n");
}

void push_process(int *n, PCB processes[], PCB process) {
    processes[(*n)++] = process;
}

void remove_process(int *n, int index, PCB processes[]) {
    for (int i = index; i + 1 < *n; i++) processes[i] = processes[i + 1];
    (*n)--;
}

void quick_sort(PCB processes[], int low, int high, int criteria) {
    if (low >= high) return;
    int pivot = get_information(processes[(low + high) / 2], criteria);
    int left = low, right = high;

    while (left <= right) {
        while (get_information(processes[left], criteria) < pivot) left++;
        while (get_information(processes[right], criteria) > pivot) right--;
        if (left <= right) {
            PCB temp = processes[left];
            processes[left] = processes[right];
            processes[right] = temp;
            left++;
            right--;
        }
    }

    quick_sort(processes, low, right, criteria);
    quick_sort(processes, left, high, criteria);
}

void calculate_avg_times(int n, PCB processes[]) {
    int total_waiting = 0, total_tat = 0, total_response = 0;

    printf("\nProcess\t\t");
    for (int i = 0; i < n; i++) printf("\tP%d", processes[i].pid);
    printf("\nWaiting Time:\t");
    for (int i = 0; i < n; i++) {
        printf("\t%d", processes[i].waiting);
        total_waiting += processes[i].waiting;
    }

    printf("\nTurnaround Time:");
    for (int i = 0; i < n; i++) {
        printf("\t%d", processes[i].tat);
        total_tat += processes[i].tat;
    }

    printf("\nResponse Time:  ");
    for (int i = 0; i < n; i++) {
        printf("\t%d", processes[i].response);
        total_response += processes[i].response;
    }

    printf("\n\nAWT: %.2f\nATaT: %.2f\nART: %.2f\n",
           (float)total_waiting / n,
           (float)total_tat / n,
           (float)total_response / n);
}

int main() {
    PCB input[100], ready_queue[100], terminated[100];
    int num_processes;

    printf("Please input the number of processes: ");
    scanf("%d", &num_processes);

    int remain = num_processes, ready = 0, terminated_count = 0;
    input_process(num_processes, input);
    quick_sort(input, 0, num_processes - 1, SORT_BY_ARRIVAL);

    push_process(&ready, ready_queue, input[0]);
    remove_process(&remain, 0, input);

    ready_queue[0].start = ready_queue[0].arrival;
    ready_queue[0].finish = ready_queue[0].start + ready_queue[0].burst;
    ready_queue[0].response = ready_queue[0].start - ready_queue[0].arrival;
    ready_queue[0].waiting = ready_queue[0].response;
    ready_queue[0].tat = ready_queue[0].finish - ready_queue[0].arrival;

    while (terminated_count < num_processes) {
        while (remain > 0 && input[0].arrival <= ready_queue[0].finish) {
            push_process(&ready, ready_queue, input[0]);
            remove_process(&remain, 0, input);
        }

        if (ready > 0) {
            print_process(1, ready_queue);
            push_process(&terminated_count, terminated, ready_queue[0]);
            remove_process(&ready, 0, ready_queue);

            quick_sort(ready_queue, 0, ready - 1, SORT_BY_BURST);

            ready_queue[0].start = terminated[terminated_count - 1].finish;
            ready_queue[0].finish = ready_queue[0].start + ready_queue[0].burst;
            ready_queue[0].response = ready_queue[0].start - ready_queue[0].arrival;
            ready_queue[0].waiting = ready_queue[0].response;
            ready_queue[0].tat = ready_queue[0].finish - ready_queue[0].arrival;
        }
    }

    printf("\n===== SJF Scheduling =====\n");
    export_gantt_chart(terminated_count, terminated);
    quick_sort(terminated, 0, terminated_count - 1, SORT_BY_PID);
    print_process(terminated_count, terminated);
    calculate_avg_times(terminated_count, terminated);
    return 0;
}
