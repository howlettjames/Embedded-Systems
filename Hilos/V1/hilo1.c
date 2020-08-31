#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int num1 = 20, num2 = 10;

void * suma(void *arg);
void * resta(void *arg);
void * mult(void *arg);
void * divi(void *arg);

int main()
{
    int *res_suma, *res_resta, *res_mult, *res_div;
    pthread_t tid_suma, tid_resta, tid_mult, tid_div;

    pthread_create(&tid_suma, NULL, suma, NULL);
    pthread_create(&tid_resta, NULL, resta, NULL);
    pthread_create(&tid_mult, NULL, mult, NULL);
    pthread_create(&tid_div, NULL, divi, NULL);

    pthread_join(tid_suma, (void *) &res_suma);
    pthread_join(tid_resta, (void *) &res_resta);
    pthread_join(tid_mult, (void *) &res_mult);
    pthread_join(tid_div, (void *) &res_div);

    printf("La suma es %d\n", *res_suma);
    printf("La resta es %d\n", *res_resta);
    printf("La mult es %d\n", *res_mult);
    printf("La div es %d\n", *res_div);

    return 0;
}

void * suma(void *arg)
{
    static int res;

    res = num1 + num2;
    pthread_exit((void *) &res);
}

void * resta(void *arg)
{
    static int res;

    res = num1 - num2;
    pthread_exit((void *) &res);
}

void * mult(void *arg)
{
    int *res = malloc(sizeof(int));

    *res = num1 * num2;
    pthread_exit((void *) res);
}

void * divi(void *arg)
{
    int *res = (int *)malloc(sizeof(int));

    *res = num1 / num2;
    pthread_exit((void *) res);
}