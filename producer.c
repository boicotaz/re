
void *producer(void *pd)
{
    ProducerData *producerData = (ProducerData *)pd;
    struct Timer *timer = producerData->timer;
    queue *fifo = timer->fifo;
    QueueEntry *queueEntry = producerData->queueEntry;
    struct timeval preceding_execution_timestamp, succeeding_execution_timestamp;
    bool isProducerFirstTimeExecution = true;
    long int drift = 0, usleep_period;

    usleep(timer->StartDelay);
    while (timer->TasksToExecute > 0)
    {
        //get time when producer BEGINS to add work to queue
        gettimeofday(&queueEntry->producer_about_to_add_work_to_queue_timestamp, NULL);

        //drift calculation_start
        if (isProducerFirstTimeExecution == true)
        {
            gettimeofday(&preceding_execution_timestamp, NULL);
            isProducerFirstTimeExecution = false;
        }
        else
        {
            gettimeofday(&succeeding_execution_timestamp, NULL);
            drift = calculateTimeDiff(preceding_execution_timestamp, succeeding_execution_timestamp) - timer->Period;
            preceding_execution_timestamp = succeeding_execution_timestamp;
        }
        printf("%c:drift:%ld\n", timer->timer_id, drift);
        //drift calculation_end

        // while queue Full sleep
        pthread_mutex_lock(fifo->mut);
        while (fifo->full)
        {
            timer->ErrorFcn(pthread_self());
            pthread_cond_wait(fifo->notFull, fifo->mut);
        }

        // if queue notFull add work to queue
        queueAdd(fifo, *queueEntry);
        pthread_mutex_unlock(fifo->mut);
        pthread_cond_signal(fifo->queueNotEmptyOrTimerFinished); //inform consumer that queue is not empty

        // Fall asleep until Period passed.
        timer->TasksToExecute--;
        usleep_period = timer->Period;
        usleep(usleep_period);
    }

    // producer clean up
    pthread_mutex_lock(fifo->mut);
    timer->StopFcn(timer); // free timer object
    pthread_mutex_unlock(fifo->mut);
    pthread_cond_broadcast(fifo->queueNotEmptyOrTimerFinished); // inform consumers that one timer has finished
}
