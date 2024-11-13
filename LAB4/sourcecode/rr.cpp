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
} pcb;

int get_info(pcb p, int criteria) {
    switch (criteria) {
        case SORT_BY_ARRIVAL: return p.arrival;
        case SORT_BY_PID: return p.pid;
        case SORT_BY_BURST: return p.burst;
        case SORT_BY_START: return p.start;
        default: return -1;
    }
}

void input_processes(int n, pcb processes[]) {
    for (int i = 0; i < n; i++) {
        printf("Input PID of %d-th process: ", i + 1);
        scanf("%d", &processes[i].pid);
        printf("Input Arrival time of process %d: ", processes[i].pid);
        scanf("%d", &processes[i].arrival);
        printf("Input Burst time of process %d: ", processes[i].pid);
        scanf("%d", &processes[i].burst);
    }
}

void print_processes(int n, pcb processes[]) {
    for (int i = 0; i < n; i++) {
        printf("===== P%d =====\n", processes[i].pid);
        printf("Start time: %d\n", processes[i].start);
        printf("Finish time: %d\n", processes[i].finish);
        printf("Waiting time: %d\n", processes[i].waiting);
        printf("Response time: %d\n", processes[i].response);
        printf("Turnaround time: %d\n", processes[i].tat);
        printf("\n");
    }
}

void export_gantt_chart(int n, pcb processes[]) {
    printf("Gantt Chart:\n ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burst; j++) printf("--");
        printf(" ");
    }
    printf("\n|");

    for (int i = 0; i < n; i++) {
        int j;
        for (j = 0; j < processes[i].burst - 1; j++) printf(" ");
        printf("P%d", processes[i].pid);
        for (j = 0; j < processes[i].burst - 1; j++) printf(" ");
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

void push_process(int *n, pcb processes[], pcb new_process) {
    processes[(*n)++] = new_process;
}

void remove_process(int *n, int index, pcb processes[]) {
    for (int i = index; i + 1 < (*n); i++) processes[i] = processes[i + 1];
    (*n)--;
}

void swap_process(pcb *p1, pcb *p2) {
    pcb temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

void quick_sort(pcb processes[], int low, int high, int criteria) {
    if (low >= high) return;
    int mid = (low + high) / 2, left = low, right = high;
    pcb pivot = processes[mid];

    while (left <= right) {
        while (get_info(processes[left], criteria) < get_info(pivot, criteria)) left++;
        while (get_info(processes[right], criteria) > get_info(pivot, criteria)) right--;
        if (left <= right) {
            swap_process(&processes[left], &processes[right]);
            left++;
            right--;
        }
    }

    quick_sort(processes, low, right, criteria);
    quick_sort(processes, left, high, criteria);
}

void calculate_avg_waiting_time(int n, pcb processes[]) {
    printf("\nAverage Waiting Time: ");
    int total_waiting_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting;
    }
    printf("%.2f\n", (float) total_waiting_time / n);
}

void calculate_avg_turnaround_time(int n, pcb processes[]) {
    printf("\nAverage Turnaround Time: ");
    int total_tat = 0;
    for (int i = 0; i < n; i++) {
        total_tat += processes[i].tat;
    }
    printf("%.2f\n", (float) total_tat / n);
}

void calculate_avg_response_time(int n, pcb processes[]) {
    printf("\nAverage Response Time: ");
    int total_response = 0;
    for (int i = 0; i < n; i++) {
        total_response += processes[i].response;
    }
    printf("%.2f\n", (float) total_response / n);
}

int main() {
    pcb input[100], ready_queue[100], terminated[100], gantt[100];
    int quantum_time, num_processes, remaining, ready = 0, terminated_count = 0, gantt_count = 0;
    int start_time[100] = {-1}, burst_time[100];

    printf("Please input Quantum time for RR: ");
    scanf("%d", &quantum_time);
    printf("Please input number of Processes: ");
    scanf("%d", &num_processes);

    remaining = num_processes;
    input_processes(num_processes, input);

    for (int i = 0; i < num_processes; i++) burst_time[input[i].pid] = input[i].burst;

    quick_sort(input, 0, num_processes - 1, SORT_BY_ARRIVAL);
    push_process(&ready, ready_queue, input[0]);
    remove_process(&remaining, 0, input);

    ready_queue[0].start = ready_queue[0].arrival;
    ready_queue[0].response = ready_queue[0].start - ready_queue[0].arrival;
    ready_queue[0].finish = ready_queue[0].start + ready_queue[0].burst;
    ready_queue[0].waiting = ready_queue[0].response;
    ready_queue[0].tat = ready_queue[0].finish - ready_queue[0].arrival;

    int current_time = 0;
    while (terminated_count < num_processes) {
        while (remaining > 0 && input[0].arrival <= current_time + quantum_time) {
            push_process(&ready, ready_queue, input[0]);
            remove_process(&remaining, 0, input);
        }

        if (ready > 0) {
            if (start_time[ready_queue[0].pid] == -1) start_time[ready_queue[0].pid] = current_time;
            int time_slice = (ready_queue[0].burst < quantum_time) ? ready_queue[0].burst : quantum_time;

            push_process(&gantt_count, gantt, ready_queue[0]);
            gantt[gantt_count - 1].burst = time_slice;
            gantt[gantt_count - 1].finish = current_time + time_slice;

            if (time_slice == ready_queue[0].burst) {
                ready_queue[0].start = start_time[ready_queue[0].pid];
                ready_queue[0].finish = current_time + time_slice;
                ready_queue[0].response = ready_queue[0].start - ready_queue[0].arrival;
                ready_queue[0].tat = ready_queue[0].finish - ready_queue[0].arrival;
                ready_queue[0].waiting = ready_queue[0].tat - burst_time[ready_queue[0].pid];
                push_process(&terminated_count, terminated, ready_queue[0]);
            } else {
                ready_queue[0].burst -= time_slice;
                push_process(&ready, ready_queue, ready_queue[0]);
            }
            remove_process(&ready, 0, ready_queue);
            current_time += time_slice;
        }
    }

    printf("\n===== RR Scheduling with Quantum time = %d =====\n", quantum_time);
    export_gantt_chart(gantt_count, gantt);
    quick_sort(terminated, 0, terminated_count - 1, SORT_BY_PID);
    print_processes(terminated_count, terminated);
    calculate_avg_response_time(terminated_count, terminated);
    calculate_avg_waiting_time(terminated_count, terminated);
    calculate_avg_turnaround_time(terminated_count, terminated);

    return 0;
}
