#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N_BUFF 20

int buffer[N_BUFF];
int pos_ins = 0, pos_rem = 0;

sem_t full, empty, mutex;

void *producer(void *arg)
{
    int item;
    while(1)
    {
        item = rand() % 500; //Gera um item aleatório

        sem_wait(&empty); //Checa e aguarda se o buffer tá vazio
        sem_wait(&mutex); // Lock

        //Adiciona o item
        buffer[pos_ins] = item;
        printf("Producer: item %d added in index %d\n", item, pos_ins);
        pos_ins = (pos_ins + 1) % N_BUFF;

        //Libera o mutex e sinaliza que tem um lugar vazio
        sem_post(&mutex);
        sem_post(&full);
        sleep(3);
    }
}

void *consumer(void *arg)
{
    int item;

    while(1)
    {
        sem_wait(&full); //Checa se há um item disponivel no buffer
        sem_wait(&mutex); //Lock

        //Remove o item
        item = buffer[pos_rem];
        printf("Consumer: item %d removed from index %d\n", item, pos_rem);
        pos_rem = (pos_rem + 1) % N_BUFF;

        //Unlock mutex e sinaliza que há espaço disponível
        sem_post(&mutex);
        sem_post(&empty);

        //Avisa que consumiu
        printf("Consumer: item %d consumed\n", item); 
        sleep(3);
    }

}
int main()
{
    pthread_t prod_t, cons_t;

    //Incializar semaforos e mutex
    sem_init(&empty, 0, N_BUFF);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    //Cria as threads
    pthread_create(&prod_t, NULL, producer, NULL);
    pthread_create(&cons_t, NULL, consumer, NULL);

    //Espera as threads terminarem
    pthread_join(prod_t, NULL);
    pthread_join(cons_t, NULL);

    //DESTRUIÇÃO E CAOS!!!!
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
