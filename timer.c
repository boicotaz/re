
void SetInterval(void (*timerFcn)(), struct Timer *t, CallbackArgs *ca)
{

    // malloc data for producer function
    ProducerData *producerData;
    producerData = (ProducerData *)malloc(sizeof(ProducerData));
    if (producerData == NULL)
    {
        fprintf(stderr, "SetInterval:  ProducerData alloc failed.\n");
        exit(1);
    }

    //malloc data for queue entry
    QueueEntry *queueEntry;
    queueEntry = (QueueEntry *)malloc(sizeof(queueEntry));
    if (queueEntry == NULL)
    {
        fprintf(stderr, "SetInterval:  queueEntry alloc failed.\n");
        exit(1);
    }

    //Each queue entry has the working function and the args for that working function
    queueEntry->arg = (void *)ca;
    queueEntry->work = timerFcn;

    //producer data consists of the queue Entry
    //and the timer obj that exposes the Period, errorFcn...
    //that producer will utilize

    struct Timer *timer = t;
    producerData->queueEntry = queueEntry;
    producerData->timer = t;

    pthread_t producerThread;
    pthread_create(&producerThread, NULL, producer, (void *)producerData);

    printf("setInterval: interval ended\n");
}

struct Timer *StartFcn(unsigned int Period, unsigned int TasksToExecute, unsigned int StartDelay, void (*StopFcn)(), void (*ErrorFcn)(), void (*SetInterval)(), queue *fifo, char timer_id)
{
    struct Timer *t;

    t = (struct Timer *)malloc(sizeof(struct Timer));
    if (t == NULL)
    {
        fprintf(stderr, "main: Timer Init failed.\n");
        exit(1);
    }
    fifo->onlineTimers++;
    t->fifo = fifo;
    t->Period = Period;
    t->TasksToExecute = TasksToExecute;
    t->StartDelay = StartDelay;
    t->StopFcn = StopFcn;
    t->ErrorFcn = ErrorFcn;
    t->SetInterval = SetInterval;
    t->timer_id = timer_id;

    return (t);
}

void ErrorFcn(long int thread_id)
{
    printf("thread_%ld: queue FULL.\n", thread_id);
}

void StopFcn(void *t)
{

    struct Timer *timer = (struct Timer *)t;
    timer->fifo->onlineTimers--;
    free(timer);
}
