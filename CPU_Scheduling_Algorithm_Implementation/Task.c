#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char pid[3];
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
} Process;

// First come First Serve
void fcfs(Process processes[], int n) {
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        current_time += processes[i].burst_time;
    }
}

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

void rr(Process processes[], int n, int quantum) {
    int current_time = 0;
    int *burst_remaining = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        burst_remaining[i] = processes[i].burst_time;
    }
    while (1) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (burst_remaining[i] > 0) {
                done = 0;
                if (burst_remaining[i] > quantum) {
                    current_time += quantum;
                    burst_remaining[i] -= quantum;
                } else {
                    current_time += burst_remaining[i];
                    processes[i].waiting_time = current_time - processes[i].burst_time - processes[i].arrival_time;
                    burst_remaining[i] = 0;
                }
            }
        }
        if (done) {
            break;
        }
    }
    free(burst_remaining);
}

float calculate_avg_waiting_time(Process processes[], int n) {
    int total_waiting_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
    }
    return (float)total_waiting_time / n;
}

void print_processes(Process processes[], int n) {
    printf("PID\tArrival\tBurst\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\n", processes[i].pid, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time);
    }
}

int main() {
    Process processes[] = {
        {"P1", 0, 10, 0},
        {"P2", 6, 8, 0},
        {"P3", 7, 4, 0},
        {"P4", 9, 5, 0}
    };
    int n = sizeof(processes) / sizeof(processes[0]);
    Process processes_fcfs[n], processes_sjf[n], processes_rr[n];
    memcpy(processes_fcfs, processes, n * sizeof(Process));
    memcpy(processes_sjf, processes, n * sizeof(Process));
    memcpy(processes_rr, processes, n * sizeof(Process));

    // FCFS
    fcfs(processes_fcfs, n);
    float avg_waiting_time_fcfs = calculate_avg_waiting_time(processes_fcfs, n);
    printf("FCFS Average Waiting Time: %.2f\n", avg_waiting_time_fcfs);
    print_processes(processes_fcfs, n);

    // SJF
    sjf(processes_sjf, n);
    float avg_waiting_time_sjf = calculate_avg_waiting_time(processes_sjf, n);
    printf("SJF Average Waiting Time: %.2f\n", avg_waiting_time_sjf);
    print_processes(processes_sjf, n);

    // RR
    int quantum = 4;
    rr(processes_rr, n, quantum);
    float avg_waiting_time_rr = calculate_avg_waiting_time(processes_rr, n);
    printf("RR Average Waiting Time: %.2f\n", avg_waiting_time_rr);
    print_processes(processes_rr, n);

    // Analysis
    printf("\nAnalysis of CPU Scheduling Algorithms:\n");
    printf("\n1. First-Come, First-Served (FCFS):\n");
    printf("   Average Waiting Time: %.2f\n", avg_waiting_time_fcfs);
    printf("   - Simple to implement.\n");
    printf("   - May lead to long waiting times, especially if the first process has a long burst time.\n");
    
    printf("\n2. Shortest Job First (SJF):\n");
    printf("   Average Waiting Time: %.2f\n", avg_waiting_time_sjf);
    printf("   - More efficient in terms of average waiting time.\n");
    printf("   - Requires knowledge of burst time in advance, which is not always possible.\n");
    printf("   - Can lead to starvation of longer processes if many short processes keep arriving.\n");
    
    printf("\n3. Round Robin (RR):\n");
    printf("   Average Waiting Time: %.2f\n", avg_waiting_time_rr);
    printf("   - Fair allocation of CPU time.\n");
    printf("   - Suitable for time-sharing systems.\n");
    printf("   - The choice of quantum impacts performance; too small a quantum leads to high context-switching overhead, too large a quantum makes it similar to FCFS.\n");

    return 0;
}
