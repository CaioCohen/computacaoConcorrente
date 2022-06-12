/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "Hello World" usando threads em C */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NTHREADS 4 // total de threads a serem criadas

int system(const char *command);

double presasFunc(double x, double y)
{
    return x - (x * y) / 2;
}

double predadoresFunc(double x, double y)
{
    return (-0.75) * y + (x * y) / 2;
}

double *EulerAnimal(double passo, int predadoresInic, int presasInic, int tempoInic, double tempoObjetivo)
{
    double *numbers;
    double predadores = (double)predadoresInic;
    double presas = (double)presasInic;
    double tempoAtual = (double)tempoInic;
    numbers = malloc(sizeof(double)*((tempoObjetivo - tempoInic) / passo));
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
int primeiraVez = 1;
pthread_mutex_t mutex;

//--funcao executada pelas threads
void *anotarDados(void *arg)
{
    eulerConfig *argumento = (eulerConfig *)arg;
    double i = (argumento->passo) * (argumento->indiceInicial);
    pthread_mutex_lock(&mutex);
    if(primeiraVez){
        primeiraVez = 0;
        pthread_mutex_unlock(&mutex);
        presas[0] = argumento->presasInic;
        predadores[0] = argumento->predadoresInic;
        tempo[0] = 0;
    }
    pthread_mutex_unlock(&mutex);

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
    int thread,tempoFinal = 100, presasInic=2,predadoresInic=5;
    double passo = 0.1;
    FILE * fp;
    eulerConfig *config;

    config=malloc(sizeof(eulerConfig)*(NTHREADS));
    if(config ==NULL){
        printf("erro malloc");
        return 0;
    }
    for(int i = 0; i < NTHREADS; i++){
        config[i].passo = passo;
        config[i].predadoresInic = predadoresInic;
        config[i].presasInic = presasInic;
        config[i].tempoFinal = tempoFinal;
    }

    pthread_mutex_init(&mutex, NULL);
    presas = malloc(sizeof(double)*((int)(tempoFinal /passo)));
    predadores = malloc(sizeof(double)*((int)(tempoFinal /passo)));
    tempo = malloc(sizeof(double)*((int)(tempoFinal /passo)));

    for (thread = 0; thread < NTHREADS; thread++)
    {
        config[thread].indiceInicial = thread + 1;
        if (pthread_create(&tid_sistema[thread], NULL, anotarDados, (void *)(config + thread)))
        {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_join(*(tid_sistema + i), NULL);
    }
    fp = fopen ("file.txt", "w");
    for(int i = 0; i <= (config->tempoFinal) / (config->passo); i++){
        fprintf(fp, "%lf,%lf,%lf,", presas[i], predadores[i], tempo[i]);
    }
    fclose(fp);
    system("python3 plot.py");
    return 0;    
}
