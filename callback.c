
void callback(void *ud)
{
    CallbackArgs *userData = (CallbackArgs *)ud;
    int id = userData->userId;
    printf("cb1\n");
    usleep(10000);
}

void callback2(void *ud)
{
    CallbackArgs *userData = (CallbackArgs *)ud;
    int id = userData->userId;

    printf("cb2\n");
    usleep(10000);
}

void callback3(void *ud)
{
    printf("cb3\n");
    usleep(10000);
}
