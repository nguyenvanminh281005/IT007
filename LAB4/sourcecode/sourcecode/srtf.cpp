#include <stdio.h>
#include <stdlib.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct{
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

int getInformation (PCB Q, int iCriteria) {
    if (iCriteria == SORT_BY_ARRIVAL) return Q.iArrival;
    if (iCriteria == SORT_BY_PID) return Q.iPID;

    if (iCriteria == SORT_BY_BURST) return Q.iBurst;
    if (iCriteria == SORT_BY_START) return Q.iStart;
}
void inputProcess (int n, PCB P[]) {
    /* Nhap thong tin cua cac tien trinh */
    for (int i = 0;  i < n; i++) {
        printf("Input PID of %d-th process: ", i + 1);
        scanf("%d", &P[i].iPID);
        printf("Input Arrival time of %d process: ", P[i].iPID);
        scanf("%d", &P[i].iArrival);
        printf("Input Burst time %d process: ", P[i].iPID);
        scanf("%d", &P[i].iBurst);
    }
}

void printProcess (int n, PCB P[]) {
    /* In ra thong tin cua cac tien trinh */
    for (int i = 0; i < n; i++) {
        printf("===== P%d =====\n", P[i].iPID);
        printf("Start time: %d\n", P[i].iStart);
        printf("Finish time: %d\n", P[i].iFinish);
        printf("Waiting time: %d\n", P[i].iWaiting);
        printf("Response time: %d\n", P[i].iResponse);
        printf("Turnaround time: %d\n", P[i].iTaT);
        printf("\n");
/*
        printf("Arrival Time: %d\n", P[i].iArrival);
        printf("Burst Time: %d\n", P[i].iBurst);
*/
    }
}

void exportGanttChart (int n, PCB P[]) {
    /* Xuat bieu do Gantt */
    printf("Gantt Chart:\n");
    // Print top bar
    printf(" ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < P[i].iBurst; j++) printf("--");
        printf(" ");
    }
    printf("\n|");

    // Print process IDs
    for (int i = 0; i < n; i++) {
        int j;
        for (j = 0; j < P[i].iBurst - 1; j++) printf(" ");
        printf("P%d", P[i].iPID);
        for (j = 0; j < P[i].iBurst - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");

    // Print bottom bar
    for (int i = 0; i < n; i++) {
        int j;
        for (j = 0; j < P[i].iBurst; j++) printf("--");
        printf(" ");
    }
    printf("\n");

    // Print timeline
    printf("0");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < P[i].iBurst; j++) printf("  ");
        if (P[i].iFinish > 9) printf("\b");  // Adjust for double-digit numbers
        printf("%d", P[i].iFinish);
    }
    printf("\n");
}

void pushProcess (int *n, PCB P[], PCB Q) {
    /* them tien trinh Q vao cuoi danh sach tien trinh P*/
    P[(*n)++] = Q;
}

void removeProcess (int *n, int index, PCB P[]) {
    /* Xoa tien trinh index khoi danh sach tien trinh P*/
    for (int i = index; i + 1 < (*n); i++) P[i] = P[i + 1];
    (*n)--;
}

void swapProcess (PCB *P, PCB *Q) {
    PCB tmp;
    tmp = (*P);
    (*P) = (*Q);
    (*Q) =  tmp;
}

void swap (int *P, int *Q) {
    int tmp;
    tmp = (*P);
    (*P) = (*Q);
    (*Q) =  tmp;
}

void quickSort (PCB P[], int low, int high, int iCriteria) {
    if  (low >= high) return ;
    int tmp = (low + high) >> 1;
    int L = low, R = high;

    while (R - L + 1 >= 3)  {
        while (L < tmp && getInformation(P[L], iCriteria) <= getInformation(P[tmp], iCriteria)) L++;
        while (tmp < R && getInformation(P[tmp], iCriteria) < getInformation(P[R], iCriteria)) R--;

        if (L < tmp && tmp < R) {
            swapProcess(&P[L], &P[R]);
            L++;
            R--;
        }
        else {
            int pos = (R + L) >> 1;
            swapProcess(&P[pos], &P[tmp]);
            swap(&pos, &tmp);
        }
    }

    if (getInformation(P[L], iCriteria) > getInformation(P[R], iCriteria)) {
        if (L == tmp) tmp++;
        else tmp--;
        swapProcess(&P[L], &P[R]);
    }
    quickSort(P, low, tmp - 1, iCriteria);
    quickSort(P, tmp + 1, high, iCriteria);
}

void calculateAWT (int n, PCB P[]) {
    printf("\nProcess\t\t");
    for (int i = 0; i < n; i++) {
        printf("\tP%d", P[i].iPID);
    }
    printf("\n");

    // Waiting Time Row
    printf("Waiting time:\t");
    for (int i = 0; i < n; i++) {
        printf("\t%d", P[i].iWaiting);
    }
    printf("\n");

    int t = 0;
    for (int i = 0; i < n; i++) t = t + P[i].iWaiting;

    printf("\nAWT: %.2f\n", 1.0 * t / n);
}

void calculateATaT (int n, PCB P[]) {
    printf("\nProcess\t\t");
    for (int i = 0; i < n; i++) {
        printf("\tP%d", P[i].iPID);
    }
    printf("\n");

//  Turnaround Time Row
    printf("Turnaround Time:");
    for (int i = 0; i < n; i++) {
        printf("\t%d", P[i].iTaT);
    }
    printf("\n");


    int t = 0;
    for (int i = 0; i < n; i++) t = t + P[i].iTaT;
    printf("\nATaT: %.2f\n", 1.0 * t / n);
}

void calculateART (int n, PCB P[]) {
    printf("\nProcess\t\t");
    for (int i = 0; i < n; i++) {
        printf("\tP%d", P[i].iPID);
    }
    printf("\n");

//  Response Time Row
    printf("Response Time:  ");
    for (int i = 0; i < n; i++) {
        printf("\t%d", P[i].iResponse);
    }
    printf("\n");


    int t = 0;
    for (int i = 0; i < n; i++) t = t + P[i].iResponse;
    printf("\nART: %.2f\n", 1.0 * t / n);
}

int main() {
    int iStart[100];
    int iBurst[100];
    for (int i = 0; i < 100; i++) iStart[i] = -1;

    PCB Input[100];
    PCB ReadyQueue[100];
    PCB TerminatedArray[100];
    PCB Gantt[200];

    int iNumberOfProcess;
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);

    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0, iGantt = 0;

    inputProcess(iNumberOfProcess, Input);

    for (int i = 0; i < iNumberOfProcess; i++)
        iBurst[Input[i].iPID]  = Input[i].iBurst;

    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);

    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);

    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;

    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

    int curTime = 0;
    while (iTerminated < iNumberOfProcess) {
        while (iRemain > 0) {
            if (Input[0].iArrival <= ReadyQueue[0].iFinish) {
                // push gantt
                if (iStart[ReadyQueue[0].iPID] == -1) iStart[ReadyQueue[0].iPID] = curTime;

                pushProcess(&iGantt, Gantt, ReadyQueue[0]);
                Gantt[iGantt - 1].iBurst = Input[0].iArrival - curTime;

                curTime = Input[0].iArrival;
                Gantt[iGantt - 1].iFinish = curTime;

                // fixed time in ready queue
                ReadyQueue[0].iBurst = ReadyQueue[0].iBurst - Gantt[iGantt - 1].iBurst;

                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);

                quickSort(ReadyQueue, 0, iReady - 1, SORT_BY_BURST);
                ReadyQueue[0].iFinish = curTime + ReadyQueue[0].iBurst;
            }
            else break;
        }

        if (iReady > 0) {
            ReadyQueue[0].iStart = iStart[ReadyQueue[0].iPID];
            ReadyQueue[0].iFinish = ReadyQueue[0].iFinish;  // was calculated
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - iBurst[ReadyQueue[0].iPID];

            curTime = ReadyQueue[0].iFinish;

            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            pushProcess(&iGantt, Gantt, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);
            // calculate start and finish for the next candidate
            if (iStart[ReadyQueue[0].iPID] == -1) iStart[ReadyQueue[0].iPID] = curTime;
            ReadyQueue[0].iFinish = curTime + ReadyQueue[0].iBurst;
        }
    }
    // Merge the same process in Gantt
    int pos = 0;
    for (int i = 1; i < iGantt; i++) {
        if (Gantt[pos].iPID == Gantt[i].iPID) {
            Gantt[pos].iBurst = Gantt[pos].iBurst + Gantt[i].iBurst;
            Gantt[pos].iFinish = Gantt[i].iFinish;
        }
        else {
            pos++;
            Gantt[pos] = Gantt[i];
        }
    }
    iGantt = ++pos;

    printf("\n===== SRTF Scheduling =====\n");
    exportGanttChart(iGantt, Gantt);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);

    printProcess(iTerminated, TerminatedArray);

    calculateART(iTerminated, TerminatedArray);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);
    printf("\n");
    return 0;
}
