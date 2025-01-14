#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 100
#define MAX_NUM_QUEUES 10

typedef struct
{
    int id;
    int priority;
    int time_left;
    int running;
} Process;

typedef struct
{
    Process *processes[MAX_PROCESSES];
    int head;
    int tail;
} Queue;

typedef struct
{
    Queue queues[MAX_NUM_QUEUES];
    int num_queues;
} Scheduler;

void scheduler_init(Scheduler *s)
{
    s->num_queues = 0;
}

int scheduler_add_queue(Scheduler *s)
{
    if (s->num_queues >= MAX_NUM_QUEUES)
    {
        return -1;
    }
    Queue *q = &(s->queues[s->num_queues]);
    q->head = 0;
    q->tail = 0;
    s->num_queues++;
    return s->num_queues - 1;
}

int enqueue(Queue *q, Process *proc)
{
    if ((q->tail + 1) % MAX_PROCESSES == q->head)
    {
        return -1; // queue is full
    }
    q->processes[q->tail] = proc;
    q->tail = (q->tail + 1) % MAX_PROCESSES;
    return 0;
}

void *dequeue(Queue *q)
{
    if (q->head == q->tail)
    {
        return NULL; // queue is empty
    }
    Process *proc = q->processes[q->head];
    q->head = (q->head + 1) % MAX_PROCESSES;
    return proc;
}

int scheduler_enqueue(Scheduler *s, int queue_idx, Process *proc)
{
    if (queue_idx < 0 || queue_idx >= s->num_queues)
    {
        return -1; // invalid queue index
    }
    return enqueue(&(s->queues[queue_idx]), proc);
}

void *scheduler_dequeue(Scheduler *s)
{
    int i;
    for (i = 0; i < s->num_queues; i++)
    {
        Queue *q = &(s->queues[i]);
        void *proc = dequeue(q);
        if (proc != NULL)
        {
            return proc;
        }
    }
    return NULL; // all queues are empty
}

void create_process(Process *p, int i)
{
    p->id = i;
    p->priority = i;
    p->time_left = ((rand() % 10) + 1) * 5;
    p->running = 0;

}

int main()
{

    Scheduler s;
    scheduler_init(&s);
    srand(time(NULL));

    int q1 = scheduler_add_queue(&s);
    int q2 = scheduler_add_queue(&s);

    Process proc0;
    Process proc1;
    Process proc2;
    Process proc3;

    create_process(&proc0, 0);
    create_process(&proc1, 1);
    create_process(&proc2, 2);
    create_process(&proc3, 3);

    scheduler_enqueue(&s, q1, &proc0);
    scheduler_enqueue(&s, q1, &proc1);
    scheduler_enqueue(&s, q2, &proc2);
    scheduler_enqueue(&s, q2, &proc3);

    Process *p;
    while ((p = scheduler_dequeue(&s)) != NULL)
    {
        printf("Process ID: %d - Priority: %d\n", p->id, p->priority);
    }
    printf("\n");
    return 0;
}
