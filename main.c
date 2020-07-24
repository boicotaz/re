// #include <pthread.h>
#include "object_definitions.c"
#include "queue.c"
#include "callback.c"
#include "producer.c"
#include "timer.c"
#include "consumer.c"
void start(struct Timer *timerObj, queue *q);
void startat(struct Timer *timerObj, queue *q, int y, int m, int d, int h, int min, int sec);

int main(void)
{
    // Init Queue
    queue *q = queueInit(1);

    struct Timer *timerObj;

    //Init consumer threads
    pthread_t consumerThread;
    pthread_create(&consumerThread, NULL, consumer, (void *)q);

    start(timerObj, q);

    pthread_join(consumerThread, NULL);

    //free queue resources
    queueDelete(q);
}

void start(struct Timer *timerObj, queue *q)
{
    // Define our custom callbackArgs
    CallbackArgs callbackArgs;
    callbackArgs.userId = 1;

    timerObj = StartFcn(10000, 40, 1000000, StopFcn, ErrorFcn, SetInterval, q, 'a');
    timerObj->SetInterval(callback, timerObj, &callbackArgs);
}

void startat(struct Timer *timerObj, queue *q, int y, int m, int d, int h, int min, int sec)
{
    // Define our custom callbackArgs
    CallbackArgs callbackArgs;
    callbackArgs.userId = 1;

    //Convert date to milliseconds since the epoch
    unsigned long int startAtAsMilliSecondsSinceEpoch = ((y - 1970) * pow(3.154, 10)) + ((m - 1) * pow(2.628, 9)) + ((d - 1) * pow(8.64, 7)) + (h * pow(3.6, 6)) + min * 60000 + sec * 1000;

    // convert current date to milliseconds since the epoch
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long long currentDateAsMillisecondsSinceEpoch =
        (unsigned long long)(tv.tv_sec) * 1000 +
        (unsigned long long)(tv.tv_usec) / 1000;

    // Calculate Start Delay
    unsigned long int StartDelay = startAtAsMilliSecondsSinceEpoch - currentDateAsMillisecondsSinceEpoch;
    //transform to us
    StartDelay *= 1000;

    timerObj = StartFcn(10000, 40, StartDelay, StopFcn, ErrorFcn, SetInterval, q, 'a');
    timerObj->SetInterval(callback, timerObj, &callbackArgs);
}