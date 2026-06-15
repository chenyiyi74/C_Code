#include <stdio.h>
#include <stdlib.h>

#define MAX 10

typedef struct {
    char name;
    int arrive;
    int runtime;
    int remain;
    int finish;
    int turnaround;
    float weighted_turnaround;
} Process;

typedef struct Queue {
    int data[MAX];
    int front, rear;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = 0;
}

int isEmpty(Queue *q) {
    return q->front == q->rear;
}

void enQueue(Queue *q, int x) {
    q->data[q->rear++] = x;
}

int deQueue(Queue *q) {
    return q->data[q->front++];
}

void RR(Process p[], int n, int qtime) {
    Queue queue;
    initQueue(&queue);
    int i, time = 0, count = 0;
    int remaining[MAX];
    
    for (i = 0; i < n; i++) {
        remaining[i] = p[i].runtime;
        enQueue(&queue, i);
    }
    
    while (!isEmpty(&queue)) {
        int idx = deQueue(&queue);
        if (remaining[idx] > qtime) {
            time += qtime;
            remaining[idx] -= qtime;
            enQueue(&queue, idx);
        } else {
            time += remaining[idx];
            remaining[idx] = 0;
            p[idx].finish = time;
            p[idx].turnaround = p[idx].finish - p[idx].arrive;
            p[idx].weighted_turnaround = (float)p[idx].turnaround / p[idx].runtime;
        }
    }
}

int main() {
    Process p[] = {
        {'A', 0, 20}, {'B', 0, 10}, {'C', 0, 15}, {'D', 0, 5}
    };
    int n = 4, qtime = 5, i;
    float avg_turn = 0, avg_weighted = 0;
    
    RR(p, n, qtime);
    
    printf("时间片=%dms时：\n", qtime);
    for (i = 0; i < n; i++) {
        printf("%c: 周转时间=%d, 带权周转时间=%.2f\n", p[i].name, p[i].turnaround, p[i].weighted_turnaround);
        avg_turn += p[i].turnaround;
        avg_weighted += p[i].weighted_turnaround;
    }
    printf("平均周转时间=%.2f, 平均带权周转时间=%.3f\n", avg_turn/n, avg_weighted/n);
    
    return 0;
}