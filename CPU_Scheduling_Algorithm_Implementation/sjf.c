#include <stdio.h>
#include <string.h>

typedef struct {
    char pid[3];
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
} Process;

void sjf(Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    int is_completed[n];
    for (int i = 0; i < n; i++) {
        is_completed[i] = 0;
    }

    while (completed != n) {
        int idx = -1;
        int min_burst_time = 10000; // A large number representing infinity

        // Find the process with the smallest burst time that has arrived and is not completed
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && !is_completed[i]) {
                if (processes[i].burst_time < min_burst_time) {
                    min_burst_time = processes[i].burst_time;
                    idx = i;
                }
                // If two processes have the same burst time, choose the one that arrived first
                if (processes[i].burst_time == min_burst_time) {
                    if (processes[i].arrival_time < processes[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            processes[idx].waiting_time = current_time - processes[idx].arrival_time;
            current_time += processes[idx].burst_time;
            processes[idx].turnaround_time = processes[idx].waiting_time + processes[idx].burst_time;
            is_completed[idx] = 1;
            completed++;
        } else {
            current_time++;
        }
    }
}

float calculateAverageWaitingTime(Process processes[], int n) {
    int total_waiting_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
    }
    return (float)total_waiting_time / n;
}

float calculateAverageTurnaroundTime(Process processes[], int n) {
    int total_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        total_turnaround_time += processes[i].turnaround_time;
    }
    return (float)total_turnaround_time / n;
}

int main() {
    Process processes[] = {
        {"P1", 0, 10, 0, 0},
        {"P2", 6, 8, 0, 0},
        {"P3", 7, 4, 0, 0},
        {"P4", 9, 5, 0, 0}
    };
    int n = sizeof(processes) / sizeof(processes[0]);

    sjf(processes, n);

    float average_waiting_time = calculateAverageWaitingTime(processes, n);
    float average_turnaround_time = calculateAverageTurnaroundTime(processes, n);

    printf("Process\tArrival\tBurst\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t%d\n", processes[i].pid, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }
    printf("Average Waiting Time: %.2f\n", average_waiting_time);
    printf("Average Turnaround Time: %.2f\n", average_turnaround_time);

    return 0;
}
