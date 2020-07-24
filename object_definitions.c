#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>
#include <math.h>

typedef struct
{
    int userId;
} CallbackArgs;

typedef struct
{
    void (*work)();
    void *arg;
    struct timeval producer_about_to_add_work_to_queue_timestamp;
    struct timeval producer_added_work_to_queuue_timestamp;
    struct timeval consumer_execute_TimerFcn_timestamp;

} QueueEntry;

typedef struct
{
    QueueEntry *buf;
    long head, tail;
    int full, empty, queueSize;
    pthread_mutex_t *mut;
    pthread_cond_t *notFull;
    /*
    This signal is send from producers when either of 2 things happen:
    1. Queue is not empty 
    2. Timer has finished
    It is up to the consumers to grasp the right meaning of the signal
    */
    pthread_cond_t *queueNotEmptyOrTimerFinished;
    int onlineTimers;
} queue;

struct Timer
{
    char timer_id;
    unsigned int Period;
    unsigned int TasksToExecute;
    unsigned int StartDelay;
    queue *fifo;
    void (*StopFcn)();
    void (*ErrorFcn)();
    void (*SetInterval)(void (*timerFcn)(), struct Timer *t, CallbackArgs *ca);
};

typedef struct
{
    struct Timer *timer;
    QueueEntry *queueEntry;
} ProducerData;