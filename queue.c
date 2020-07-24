queue *queueInit(int queueSize);
void queueDelete(queue *q);
void queueAdd(queue *q, QueueEntry in);
void *queuePop(queue *q);
long int calculateTimeDiff(struct timeval start, struct timeval end);
queue *queueInit(int queueSize)
{
    queue *q;

    q = (queue *)malloc(sizeof(queue));
    if (q == NULL)
    {
        fprintf(stderr, "queueInit:  queue alloc failed.\n");
        exit(1);
    }

    q->buf = malloc(queueSize * sizeof(QueueEntry));

    if (q->buf == NULL)
    {
        fprintf(stderr, "queueInit:  q buffer alloc failed.\n");
        exit(1);
    }

    q->queueSize = queueSize;
    q->empty = 1;
    q->full = 0;
    q->head = 0;
    q->tail = 0;
    q->mut = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(q->mut, NULL);
    q->notFull = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->notFull, NULL);
    q->queueNotEmptyOrTimerFinished = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->queueNotEmptyOrTimerFinished, NULL);
    q->onlineTimers = 0;

    return (q);
}

void queueDelete(queue *q)
{
    pthread_mutex_destroy(q->mut);
    free(q->mut);
    pthread_cond_destroy(q->notFull);
    free(q->notFull);
    pthread_cond_destroy(q->queueNotEmptyOrTimerFinished);
    free(q->queueNotEmptyOrTimerFinished);
    free(q->buf);
    free(q);
}

void queueAdd(queue *q, QueueEntry in)
{
    //get time when producer SUCCESSFULLY added work to queue
    q->buf[q->tail] = in;
    gettimeofday(&q->buf[q->tail].producer_added_work_to_queuue_timestamp, NULL);
    long int time_interval_to_add_to_queue = calculateTimeDiff(q->buf[q->tail].producer_about_to_add_work_to_queue_timestamp, q->buf[q->tail].producer_added_work_to_queuue_timestamp);
    printf("producer:%ld\n", time_interval_to_add_to_queue);

    q->tail++;
    if (q->tail == q->queueSize)
        q->tail = 0;
    if (q->tail == q->head)
        q->full = 1;
    q->empty = 0;

    return;
}

void *queuePop(queue *q)
{
    if (q->head == q->tail && !q->full)
    {
        q->empty = 1;
        return NULL;
    }
    else
    {
        QueueEntry *out = &q->buf[q->head];

        q->head++;
        if (q->head == q->queueSize)
            q->head = 0;
        if (q->head == q->tail)
            q->empty = 1;

        q->full = 0;

        return (void *)out;
    }
}

long int calculateTimeDiff(struct timeval start, struct timeval end)
{
    long int start_time_to_us = (start.tv_sec * 1000000) + start.tv_usec;
    long int end_time_to_us = (end.tv_sec * 1000000) + end.tv_usec;

    return end_time_to_us - start_time_to_us;
}