#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Variável global dos garfos
pthread_mutex_t garfos[5];

//Mesa dos filosofos
void *filosofo(void *n)
{
    int i = *(int*)n;
    //Garfo da esqueda
    int esq = i;
    //Garfo da direita
    int dir = (i + 1) % 5;

    //loop infinito dos filosofos pensando e comendo
    while(1){
        // 1. PENSAR
        printf("Filósofo %d está pensando...\n", i+1);
        sleep(2); // Tempo fixo de 2 segundos pensando

        // 2. TENTAR PEGAR OS GARFOS (Lógica Anti-Deadlock)
        if (i == 4) { //Se for o ultimo filosofo pega primeiro o garfo da direita
            pthread_mutex_lock(&garfos[dir]);
            pthread_mutex_lock(&garfos[esq]);
        } else { // Se não pega primeiro o garfo da esquerda
            pthread_mutex_lock(&garfos[esq]);
            pthread_mutex_lock(&garfos[dir]);
        }
        
        // 3. COMER
        printf("Filósofo %d está COMENDO\n", i+1);
        sleep(3); // Tempo fixo de 3 segundos comendo

        // 4. SOLTAR GARFOS
        pthread_mutex_unlock(&garfos[dir]);
        pthread_mutex_unlock(&garfos[esq]);
        
        printf("Filósofo %d terminou de comer e soltou os garfos.\n", i+1);
        printf("------------------------------------------\n");
    }

    return NULL;
}

int main()
{
    pthread_t filosofos[5];
    int ids[5];
    
    // Inicialização dos mutexes
    for (int i = 0; i < 5; i++)
    {
        pthread_mutex_init(&garfos[i], NULL);
    }
    
    // Criação das threads para cada filósofo
    for (int i = 0; i < 5; i++)
    {
        ids[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, &ids[i]);
    }
    
    // O programa rodará indefinidamente devido ao while(1)
    for (int i = 0; i < 5; i++)
    {
        pthread_join(filosofos[i], NULL);
    }
    
    return 0;
}