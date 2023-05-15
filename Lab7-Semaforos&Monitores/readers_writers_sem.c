/*
Autores: 
    João Victor Salvi Silva
    Gabriela Paola Sereniski
    Ingrid Reupke Sbeguen Moran

Descrição:
    Implementação do problema de concorrencia leitores/escritores usando semáforos.
    Os leitores podem ler simultaneamente, mas os escritores devem escrever apenas quando não há leitores ou outros escritores escrevendo.
*/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

#define N_READERS 5 // Número de leitores
#define N_WRITERS 5 // Número de escritores

//Contadores
int reader_count = 0; // De leitores
int writer_count = 0; // De escritores

// Semáforos
sem_t sem_writers; 
sem_t mutex;

// Leitores --------------------------------------------------------------------------
void *leitor(void *arg)
{
    int id = *(int *)arg; // Pega o id do leitor
    printf("Leitor %d entrou\n", id);

    sem_wait(&mutex); // Espera o mutex

    ++reader_count;
    
    // É o primeiro leitor? Se sim, entra no semáforo de escritores
    if (reader_count == 1)
    {
        sem_wait(&sem_writers);
    }
    sem_post(&mutex); // Libera o mutex
        
    printf("Leitor %d está lendo: %d, %d\n", id, reader_count, writer_count);   
    sleep(1);

    sem_wait(&mutex); // Espera o mutex
    reader_count--;
    
    // Se o contador de leitores - 1 for igual a zero, libera o escritor
    if (reader_count == 0)
    {
        sem_post(&sem_writers);
    }
    sem_post(&mutex); // Libera o mutex

    printf("Leitor %d saiu\n", id);

    pthread_exit(NULL);
}

// Escritores -----------------------------------------------------------------------
void *escritor(void *arg) 
{
    int id = *(int *)arg; // Pega o id do escritor
  
    printf("Escritor %d entrou\n", id);

    // Espera o semáforo de escritores
    sem_wait(&sem_writers); 
    ++writer_count; 

    printf("Escritor %d está escrevendo: %d, %d\n", id, reader_count, writer_count);
    
    sleep(1);

    --writer_count;
    sem_post(&sem_writers); // Libera o semáforo de escritores

    printf("Escritor %d saiu\n", id); 
    
    pthread_exit(NULL);
}

int main() 
{
    int i;
    pthread_t readers[N_READERS], writers[N_WRITERS];
    int ids[N_READERS];

    sem_init(&mutex, 0, 1);         // inicialização do semáforo de leitores
    sem_init(&sem_writers, 0, 1);   // inicialização do semáforo de escritores

    for (i = 0; i < N_READERS; i++)
    {
        ids[i] = i;
        pthread_create(&readers[i], NULL, leitor, &ids[i]);
    }

    for (i = 0; i < N_WRITERS; i++)
    {
        pthread_create(&writers[i], NULL, escritor, &ids[i]);
    }

    for (i = 0; i < N_READERS; i++)
    {
        pthread_join(readers[i], NULL);
    }

    for (i = 0; i < N_WRITERS; i++)
    {
        pthread_join(writers[i], NULL);
    }

    // Destruição dos semáforos
    sem_destroy(&sem_writers);
    sem_destroy(&mutex);

    return 0;
}