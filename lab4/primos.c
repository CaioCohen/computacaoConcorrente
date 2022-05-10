#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long long int N, iGlobal = 0;
int nthreads;    // numero de threads
int *vetor;   // vetor de entrada com dimensao dim
pthread_mutex_t lock;

int ehPrimo(int n){
    double raiz = sqrt(n);
    if(n <= 2){
        return 0;
    }
    for(int i = 2; i <= raiz; i++){
        if (!(n % i)){
            return 0;
        }
    }
    return 1;
}

// fluxo das threads
void *tarefa(void* arg)
{
    double* vetorLocal = (double *) arg;
    // soma os elementos do bloco da thread
    int i = 0;
    while(i < N){
        //fazer
        pthread_mutex_lock(&lock);
        i = iGlobal++;
        pthread_mutex_unlock(&lock);
        vetorLocal[i] = ehPrimo(vetor[i]) ? sqrt(vetor[i]) : vetor[i];
    }
    // retorna o resultado da soma local
    pthread_exit(NULL);
}

// fluxo principal
int main(int argc, char *argv[])
{
    double ini, fim; // tomada de tempo
    pthread_t *tid;                   // identificadores das threads no sistema
    double timeSeq, timeConc;
    float * vetorSeq, * vetorConc;

    // recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if (argc < 3)
    {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1;
    }
    N = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    // aloca o vetor de entrada
    vetor = (int *)malloc(sizeof(int) * N);
    if (vetor == NULL)
    {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    vetorSeq = (float *)malloc(sizeof(float) *  N);
    vetorConc = (float *)malloc(sizeof(float) *  N);
    srand(time(NULL));
    // preenche o vetor de entrada
    for (long int i = 0; i < N; i++)
        vetor[i] = rand() % N;

    // analise sequencial dos elementos
    GET_TIME(ini);
    for (long long int i = 0; i < N; i++){
        //fazer
        vetorSeq[i] = ehPrimo(vetor[i]) ? sqrt(vetor[i]) : vetor[i];
    }

    GET_TIME(fim);
    timeSeq = fim - ini;
    printf("Tempo sequencial:  %lf\n", timeSeq);

    // analise concorrente dos elementos
    GET_TIME(ini);
    tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL)
    {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    // criar as threads
    pthread_mutex_init(&lock, NULL);
    for (long int i = 0; i < nthreads; i++)
    {
        if (pthread_create(tid + i, NULL, tarefa, (void *)vetorConc))
        {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    // aguardar o termino das threads
    for (long int i = 0; i < nthreads; i++)
    {
        if (pthread_join(*(tid + i), NULL))
        {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    pthread_mutex_destroy(&lock);
    GET_TIME(fim);
    timeConc = fim - ini;
    printf("Tempo concorrente:  %lf\n", timeConc);

    // exibir os resultados
    printf("Aceleração: %lf\n", timeSeq/timeConc);

    // libera as areas de memoria alocadas
    free(vetor);
    free(tid);

    return 0;
}