/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "Hello World" usando threads em C */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NTHREADS 4 // total de threads a serem criadas

double presasFunc(double x, double y)
{
    return x - (x * y) / 2;
}

double predadoresFunc(double x, double y)
{
    return (-0.75) * y + (x * y) / 2;
}

double *EulerAnimal(double passo, int predadoresInic, int presasInic, int tempoInic, int tempoObjetivo)
{
    double numbers[(int)((tempoObjetivo - tempoInic) / passo)];
    double predadores = (double)predadoresInic;
    double presas = (double)presasInic;
    double tempoAtual = (double)tempoInic;
    while (tempoAtual < tempoObjetivo)
    {
        predadores = predadores + (passo * predadoresFunc(presas, predadores));
        presas = presas + (passo * presasFunc(presas, predadores));
        if (presas < 0)
        {
            presas = 0;
        }
        if (predadores < 0)
        {
            predadores = 0;
        }
        tempoAtual += passo;
    }
    numbers[0] = presas;
    numbers[1] = predadores;
    return numbers;
}

typedef struct
{
    double passo, predadoresInic, presasInic, tempoFinal;
    int indiceInicial;
} eulerConfig;

double* presas;
double* predadores;
double* tempo;

//--funcao executada pelas threads
void *anotarDados(void *arg)
{
    eulerConfig *argumento = (eulerConfig *)arg;

    double i = (argumento->passo) * (argumento->indiceInicial);
    presas[0] = argumento->presasInic;
    predadores[0] = argumento->predadoresInic;
    tempo[0] = 0;
    int contador = argumento->indiceInicial;
    while (i < argumento->tempoFinal)
    {
        tempo[contador] = i;
        presas[contador] = EulerAnimal(argumento->passo, argumento->predadoresInic, argumento->presasInic, 0, i)[0];
        predadores[contador] = EulerAnimal(argumento->passo, argumento->predadoresInic, argumento->presasInic, 0, i)[1];
        i += (argumento->passo)*NTHREADS;
        contador += NTHREADS;
    }
    pthread_exit(NULL);
}

//--funcao principal do programa
int main(void)
{
    pthread_t tid_sistema[NTHREADS]; // identificadores das threads no sistema
    int thread;                      // variavel auxiliar
    eulerConfig config;
    config.passo = 0.1;
    config.predadoresInic = 5;
    config.presasInic = 2;
    config.tempoFinal = 1;
    presas = malloc(sizeof(double)*(config.tempoFinal) / (config.passo));
    predadores = malloc(sizeof(double)*(config.tempoFinal) / (config.passo));
    tempo = malloc(sizeof(double)*(config.tempoFinal) / (config.passo));

    for (thread = 0; thread <= NTHREADS; thread++)
    {
        config.indiceInicial = thread + 1;
        if (pthread_create(tid_sistema + thread, NULL, anotarDados, (void *)config))
        {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_join(*(tid_sistema + i), NULL);
    }
    for(int i = 0; i < (config.tempoFinal) / (config.passo); i++){
        printf("%lf ", tempo + i);
    }

    pthread_exit(NULL);
}
