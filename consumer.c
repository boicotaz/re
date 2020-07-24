
void *consumer(void *q)
{
    printf("consume init \n");
    queue *fifo = (queue *)q;

    while (fifo->onlineTimers != 0)
    {
        pthread_mutex_lock(fifo->mut);
        while (fifo->empty && fifo->onlineTimers)
        {
            pthread_cond_wait(fifo->queueNotEmptyOrTimerFinished, fifo->mut);
        }

        QueueEntry *returnedEntry = (QueueEntry *)queuePop(fifo);

        if (returnedEntry != NULL)
        {
            QueueEntry entry = returnedEntry[0];
            pthread_mutex_unlock(fifo->mut);
            pthread_cond_signal(fifo->notFull);
            //get time when consumer BEGINS to execute the work
            gettimeofday(&entry.consumer_execute_TimerFcn_timestamp, NULL);
            printf("consumer:%ld\n", calculateTimeDiff(entry.producer_added_work_to_queuue_timestamp, entry.consumer_execute_TimerFcn_timestamp));
            entry.work(entry.arg);
        }
    }

    pthread_mutex_unlock(fifo->mut);
}