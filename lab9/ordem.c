#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS 5
#define TCOUNT 20

/* Variaveis globais */
int x = 0;
sem_t condt2, condt3, condt4; 
const char *ordens[5] = {"Volte sempre!", "Fique a vontade", "Sente-se por favor", "Aceita um copo dagua?", "Seja bem-vindo!"};

void *garcom1(void *arg)
{
    long int id = (long int)arg;
    printf("%s\n", ordens[id]);
    for(int i = 0; i < 3; i++){
        sem_post(&condt2);
    }
    pthread_exit(NULL);
}

void *garcom2(void *arg)
{
    long int id = (long int)arg;
    sem_wait(&condt2);
    printf("%s\n", ordens[id]);
    sem_wait(&condt4);
    x++;
    if(x == 3){
        sem_post(&condt3);
    }
    sem_post(&condt4);
    pthread_exit(NULL);
}

void *garcom3(void *arg)
{
    long int id = (long int)arg;
    sem_wait(&condt3);
    printf("%s\n", ordens[id]);
    
    pthread_exit(NULL);
}

/* Funcao principal */
int main()
{
    pthread_t *tid;
    tid = (pthread_t *)malloc(sizeof(pthread_t) * NTHREADS);
    sem_init(&condt2, 0, 0);
    sem_init(&condt3, 0, 0);
    sem_init(&condt4, 0, 1);

    if (pthread_create(tid+0, NULL, garcom3, (void *)0))
    {
        fprintf(stderr, "ERRO--pthread_create\n");
        return 3;
    };
    if (pthread_create(tid+1, NULL, garcom2, (void *)1))
    {
        fprintf(stderr, "ERRO--pthread_create\n");
        return 3;
    };
    if (pthread_create(tid+2, NULL, garcom2, (void *)2))
    {
        fprintf(stderr, "ERRO--pthread_create\n");
        return 3;
    };
    if (pthread_create(tid+3, NULL, garcom2, (void *)3))
    {
        fprintf(stderr, "ERRO--pthread_create\n");
        return 3;
    };
    if (pthread_create(tid+4, NULL, garcom1, (void *)4))
    {
        fprintf(stderr, "ERRO--pthread_create\n");
        return 3;
    };

    /* Espera todas as threads completarem */
    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_join(*(tid + i), NULL);
    }
    printf("FIM.\n");
    
    return 0;
}