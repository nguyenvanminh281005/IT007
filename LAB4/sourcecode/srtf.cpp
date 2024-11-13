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

int get_info(pcb process, int criteria) {
    if (criteria == SORT_BY_ARRIVAL) return process.arrival;
    if (criteria == SORT_BY_PID) return process.pid;
    if (criteria == SORT_BY_BURST) return process.burst;
    if (criteria == SORT_BY_START) return process.start;
    return -1;
}

void input_process(int n, pcb processes[]) {
    for (int i = 0; i < n; i++) {
        printf("Input PID of %d-th process: ", i + 1);
        scanf("%d", &processes[i].pid);
        printf("Input Arrival time of %d process: ", processes[i].pid);
        scanf("%d", &processes[i].arrival);
        printf("Input Burst time %d process: ", processes[i].pid);
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
        printf("Turnaround time: %d\n\n", processes[i].tat);
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

void swap_processes(pcb *a, pcb *b) {
    pcb temp = *a;
    *a = *b;
    *b = temp;
}

void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void quicksort(pcb processes[], int low, int high, int criteria) {
    if (low >= high) return;
    int mid = (low + high) / 2;
    int left = low, right = high;
    
    while (right - left + 1 >= 3)  {
        while (left < mid && get_info(processes[left], criteria) <= get_info(processes[mid], criteria)) left++;
        while (mid < right && get_info(processes[mid], criteria) < get_info(processes[right], criteria)) right--;

        if (left < mid && mid < right) {
            swap_processes(&processes[left], &processes[right]);
            left++;
            right--;
        } else {
            int pos = (right + left) / 2;
            swap_processes(&processes[pos], &processes[mid]);
            swap_int(&pos, &mid);
        }
    }
    if (get_info(processes[left], criteria) > get_info(processes[right], criteria)) {
        if (left == mid) mid++;
        else mid--;
        swap_processes(&processes[left], &processes[right]);
    }
    quicksort(processes, low, mid - 1, criteria);
    quicksort(processes, mid + 1, high, criteria);
}

void calculate_averages(int n, pcb processes[], char *type) {
    printf("\nProcess\t\t");
    for (int i = 0; i < n; i++) printf("\tP%d", processes[i].pid);
    printf("\n");

    int total = 0;
    printf("%s:\t", type);
    for (int i = 0; i < n; i++) {
        int value = (type == "Waiting time") ? processes[i].waiting : (type == "Turnaround Time") ? processes[i].tat : processes[i].response;
        total += value;
        printf("\t%d", value);
    }
    printf("\nAverage %s: %.2f\n", type, total / (float)n);
}

int main() {
    int start_times[100];
    int burst_times[100];
    for (int i = 0; i < 100; i++) start_times[i] = -1;

    pcb input[100], ready_queue[100], terminated[100], gantt[200];

    int num_processes;
    printf("Please input number of processes: ");
    scanf("%d", &num_processes);

    int remaining = num_processes, ready = 0, terminated_count = 0, gantt_count = 0;

    input_process(num_processes, input);
    for (int i = 0; i < num_processes; i++)
        burst_times[input[i].pid] = input[i].burst;

    quicksort(input, 0, num_processes - 1, SORT_BY_ARRIVAL);
    push_process(&ready, ready_queue, input[0]);
    remove_process(&remaining, 0, input);

    ready_queue[0].start = ready_queue[0].arrival;
    ready_queue[0].response = ready_queue[0].start - ready_queue[0].arrival;
    ready_queue[0].finish = ready_queue[0].start + ready_queue[0].burst;
    ready_queue[0].waiting = ready_queue[0].response;
    ready_queue[0].tat = ready_queue[0].finish - ready_queue[0].arrival;

    int cur_time = 0;
    while (terminated_count < num_processes) {
        while (remaining > 0) {
            if (input[0].arrival <= ready_queue[0].finish) {
                if (start_times[ready_queue[0].pid] == -1) start_times[ready_queue[0].pid] = cur_time;
                push_process(&gantt_count, gantt, ready_queue[0]);
                gantt[gantt_count - 1].burst = input[0].arrival - cur_time;
                cur_time = input[0].arrival;
                gantt[gantt_count - 1].finish = cur_time;
                ready_queue[0].burst -= gantt[gantt_count - 1].burst;
                push_process(&ready, ready_queue, input[0]);
                remove_process(&remaining, 0, input);
                quicksort(ready_queue, 0, ready - 1, SORT_BY_BURST);
                ready_queue[0].finish = cur_time + ready_queue[0].burst;
            } else break;
        }
        if (ready > 0) {
            ready_queue[0].start = start_times[ready_queue[0].pid];
            cur_time = ready_queue[0].finish;
            push_process(&terminated_count, terminated, ready_queue[0]);
            push_process(&gantt_count, gantt, ready_queue[0]);
            remove_process(&ready, 0, ready_queue);
            if (start_times[ready_queue[0].pid] == -1) start_times[ready_queue[0].pid] = cur_time;
            ready_queue[0].finish = cur_time + ready_queue[0].burst;
        }
    }

    int pos = 0;
    for (int i = 1; i < gantt_count; i++) {
        if (gantt[pos].pid == gantt[i].pid) {
            gantt[pos].burst += gantt[i].burst;
            gantt[pos].finish = gantt[i].finish;
        } else {
            pos++;
            gantt[pos] = gantt[i];
        }
    }
    gantt_count = ++pos;

    printf("\n===== SRTF Scheduling =====\n");
    export_gantt_chart(gantt_count, gantt);
    quicksort(terminated, 0, num_processes - 1, SORT_BY_PID);
    print_processes(num_processes, terminated);
    calculate_averages(num_processes, terminated, "Waiting time");
    calculate_averages(num_processes, terminated, "Turnaround Time");
    calculate_averages(num_processes, terminated, "Response Time");
    return 0;
}
