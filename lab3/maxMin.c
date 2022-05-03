#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long long int N; // dimensao do vetor de entrada
int nthreads;    // numero de threads
double *vetor;   // vetor de entrada com dimensao dim

typedef struct {
    double max, min;
} maxMin;

// fluxo das threads
void *tarefa(void *arg)
{
    long int id = (long int)arg; // identificador da thread
    maxMin *resultado;           // variavel local do maior elemento
    resultado = (maxMin *)malloc(sizeof(maxMin));
    if (resultado == NULL)
    {
        exit(1);
    }
    // soma os elementos do bloco da thread
    resultado ->max = vetor[id];
    resultado -> min = vetor[id];
    for (long int i = id; i < N; i+= nthreads){
        resultado ->max = vetor[i] > resultado ->max ? vetor[i] : resultado ->max;
        resultado -> min = vetor[i] < resultado -> min ? vetor[i] : resultado -> min;
    }
    // retorna o resultado da soma local
    pthread_exit((void *)resultado);
}

// fluxo principal
int main(int argc, char *argv[])
{
    double ini, fim, maxSeq, maxConc, minSeq, minConc; // tomada de tempo
    pthread_t *tid;                   // identificadores das threads no sistema
    double *retorno;                  // valor de retorno das threads
    double timeSeq, timeConc;

    // recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if (argc < 3)
    {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1;
    }
    N = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    // aloca o vetor de entrada
    vetor = (double *)malloc(sizeof(double) * N);
    if (vetor == NULL)
    {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    srand(time(NULL));
    // preenche o vetor de entrada
    for (long int i = 0; i < N; i++)
        vetor[i] = rand() % N;

    // analise sequencial dos elementos
    GET_TIME(ini);
    maxSeq = vetor[0];
    minSeq = vetor[0];
    for (long long int i = 0; i < N; i++){
        maxSeq = vetor[i] > maxSeq ? vetor[i] : maxSeq;
        minSeq = vetor[i] < minSeq ? vetor[i] : minSeq;
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
    for (long int i = 0; i < nthreads; i++)
    {
        if (pthread_create(tid + i, NULL, tarefa, (void *)i))
        {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    maxConc = vetor[0];
    minConc = vetor[0];
    // aguardar o termino das threads
    for (long int i = 0; i < nthreads; i++)
    {
        if (pthread_join(*(tid + i), (void **)&retorno))
        {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
        // soma global
        maxMin *resultado = (maxMin*) retorno;
        maxConc = resultado->max > maxConc ? resultado->max : maxConc;
        minConc = resultado->min < minConc ? resultado->min : minConc;
    }
    GET_TIME(fim);
    timeConc = fim - ini;
    printf("Tempo concorrente:  %lf\n", timeConc);

    // exibir os resultados
    printf("maior seq:  %.12lf menor seq: %.12lf\n", maxSeq, minSeq);
    printf("maior conc: %.12lf menor conc: %.12lf\n", maxConc, minConc);
    printf("Aceleração: %lf\n", timeSeq/timeConc);

    // libera as areas de memoria alocadas
    free(vetor);
    free(tid);

    return 0;
}