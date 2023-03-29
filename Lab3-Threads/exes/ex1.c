#include<stdio.h>
#include<pthread.h>

#define N 3
#define TAM_VET 10

struct data_chunk 
{
    int num;
    int key;
    int ini, fim;
    int *vet;
};


void *buscar(void* param)
{
    struct data_chunk *dados = param;

    for(int i = dados->ini; i < dados->fim; i++)
        if(dados->vet[i] == dados->key)
            printf("Thread %ld achou o valor %d na posição %d!\n", pthread_self(), dados->key, i);
}



int main()
{
    int vet[TAM_VET] = {2, 3, 5, 6, 2, 8, 4, 7, 5, 2}, tamanho_parte = TAM_VET/N;
    int inicial = 0, final = tamanho_parte, chave = 2;    
    
    struct data_chunk dados[N];
    
    pthread_t t[N];
    for(int i = 0; i < N; i++)
    {
        dados[i].num = i;
        dados[i].ini = inicial;
        dados[i].fim = final;
        dados[i].vet = vet;
        dados[i].key = chave;

        inicial = final;
        final += tamanho_parte;
    }
    dados[N-1].fim = TAM_VET;

    for(int i = 0; i < N; i++)
        pthread_create(&t[i], NULL, buscar, &dados[i]);

    for(int i = 0; i < N; i++)
    {
        pthread_join(t[i], NULL);
    }

    return 0;
}