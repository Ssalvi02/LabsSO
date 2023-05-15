/*
Autores: 
    João Victor Salvi Silva
    Gabriela Paola Sereniski
    Ingrid Reupke Sbeguen Moran

Descrição:
    Implementação do problema de concorrencia barbeiro dorminhoco usando monitores.
    O barbeiro corta o cabelo de um cliente de cada vez, enquanto os outros clientes esperam na sala de espera. 
    Se não há clientes na sala de espera, o barbeiro dorme.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CLIENTES 10
#define NUM_CADEIRAS 5

sem_t sem_barbeiro;      // semáforo do barbeiro
sem_t sem_cadeiras;      // semáforo das cadeiras da sala de espera
sem_t sem_corte_concluido;  // semáforo para indicar quando o corte de cabelo foi concluído

int num_clientes_aguardando = 0;  // número de clientes aguardando

void *barbeiro(void *arg) {
    while (1) {
        // o barbeiro dorme enquanto não há clientes
        printf("Barbeiro dormindo...\n");
        sem_wait(&sem_barbeiro);
        sleep(1);

        // atende um cliente
        printf("Barbeiro atendendo um cliente...\n");
        sleep(1);
        sem_post(&sem_corte_concluido);
    }
    pthread_exit(NULL);
}

void *cliente(void *arg) {
    int id = *((int *) arg);
    printf("Cliente %d chegou na barbearia.\n", id);
    sleep(1);

    // verifica se há cadeiras disponíveis na sala de espera
    if (num_clientes_aguardando < NUM_CADEIRAS) {
        num_clientes_aguardando++;
        printf("Cliente %d aguardando na sala de espera.\n", id);
        sleep(1);

        // sinaliza ao barbeiro que há um cliente aguardando
        sem_post(&sem_barbeiro);

        // aguarda pelo atendimento do barbeiro
        sem_wait(&sem_corte_concluido);

        // libera a cadeira na sala de espera
        num_clientes_aguardando--;
        printf("Cliente %d deixando a barbearia.\n", id);
        sleep(1);
    } else {
        // não há cadeiras disponíveis, o cliente vai embora
        printf("Não há cadeiras disponíveis. Cliente %d vai embora.\n", id);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t tid_barbeiro;
    pthread_t tid_clientes[NUM_CLIENTES];
    int ids[NUM_CLIENTES];

    // inicializa os semáforos
    sem_init(&sem_barbeiro, 0, 0);
    sem_init(&sem_cadeiras, 0, NUM_CADEIRAS);
    sem_init(&sem_corte_concluido, 0, 0);

    // cria a thread do barbeiro
    pthread_create(&tid_barbeiro, NULL, barbeiro, NULL);

    // cria as threads dos clientes
    for (int i = 0; i < NUM_CLIENTES; i++) {
        ids[i] = i;
        pthread_create(&tid_clientes[i], NULL, cliente, &ids[i]);
        sleep(1);;  // espera um pouco antes de criar o próximo cliente
    }

    // espera as threads dos clientes terminarem
    for (int i = 0; i < NUM_CLIENTES; i++) {
        pthread_join(tid_clientes[i], NULL);
    }

    // cancela a thread do barbeiro
    pthread_cancel(tid_barbeiro);

    // destroi os semáforos
    sem_destroy(&sem_barbeiro);
    sem_destroy(&sem_cadeiras);
    sem_destroy(&sem_corte_concluido);

    return 0;
}