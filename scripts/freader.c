#include <stdio.h>
#include <stdlib.h>

#include <math.h>

int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}
int main()
{
    char ch, file_name[25];
    FILE *fp;

    printf("Enter name of a file you wish to see\n");
    gets(file_name);

    fp = fopen(file_name, "r"); // read mode

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    int mean = 0;
    char buffer[10];
    int min = 99999999;
    int max = -1;

    while (fgets(buffer, 10, fp) != NULL)
    {
        int number = atoi(buffer);
        mean += number;
        count++;
        if (number >= max)
            max = number;
        if (number <= min && number != 0)
            min = number;
    }

    printf("sum %d count %d\n", mean, count);

    float res = (float)mean / (float)count;
    printf("mean is: %f\n", res);
    printf("max is: %d\n", max);
    printf("min is: %d\n", min);

    rewind(fp);
    float var = 0;
    int arr[count];
    int i = 0;
    while (fgets(buffer, 10, fp) != NULL)
    {
        float number = atoi(buffer);
        var += (number - res) * (number - res);
        arr[i] = number;
        i++;
    }
    var /= count;
    var = pow(var, 0.5);
    printf("var is: %f\n", var);

    qsort(arr, count, sizeof(int), cmpfunc);

    printf("median is: %d\n", arr[count / 2]);
    fclose(fp);
    return 0;
}