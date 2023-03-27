#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "matriz.h"

#define N 30
#define M 30
#define N_THREADS 1

double resp_med_geom[M];
double resp_med_arit[N];

struct data_chunk 
{
    int seq;
    int row, col;
    int ini, fim;
    int **mat;
};

void *med_arit(void* param)
{
    struct data_chunk *dados = param;

    for(int i = dados->ini; i < dados->fim; i++)
    {
        for(int j = 0; j < dados->col; j++)
        {
            printf(" %d \n" , dados->mat[i][j]);
            resp_med_arit[dados->seq] += dados->mat[i][j];
        }
    }
    resp_med_arit[dados->seq] /= (double)dados->row;
}

void *med_geom(void* param)
{
    struct data_chunk *dados = param;
    
    resp_med_arit[dados->seq] = 1;
    
    for(int j = dados->ini; j < dados->fim; j++)
    {
        for(int i = 0; i < dados->row; i++)
        {
            resp_med_arit[dados->seq] *= dados->mat[i][j];
        }
    }
    pow(resp_med_arit[dados->seq], 1.0/dados->col);
}


int main()
{
    printf("AAAAAAAA");
    int row, col; 
    int **matrix = read_matrix_from_file("matriz_6por8.in", &row, &col);
    struct data_chunk dados[N_THREADS];
    pthread_t t[N_THREADS];

    int tam_parte = row/N_THREADS;
    int inicial = 0, final = tam_parte;

    for(int i = 0; i < N_THREADS; i++)
    {
        dados[i].mat = matrix;
        dados[i].row = row;
        dados[i].col = col;
        dados[i].seq = dados->row/N_THREADS;
        dados[i].ini = inicial;
        dados[i].fim = final;


        pthread_create(&t[i], NULL, med_arit, &dados[i]);
        inicial = final+1;
        final += tam_parte;
        if(i == N_THREADS-1)
        {
            final = N_THREADS;
        }
    }

    for(int i = 0; i < N_THREADS; i++)
    {
        pthread_join(t[i], NULL);
    }

    /*
    tam_parte = col/N_THREADS;
    inicial = 0, final = tam_parte;

    for(int i = 0; i < N_THREADS; i++)
    {
        dados[i].row = row;
        dados[i].col = col;
        dados[i].seq = dados->row/N_THREADS;
        dados[i].ini = inicial;
        dados[i].fim = final;


        pthread_create(&t[i], NULL, med_geom, &dados[i]);
        inicial = final+1;
        final += tam_parte;
        if(i == N_THREADS-1)
        {
            final = N_THREADS;
        }
    }
    for(int i = 0; i < N_THREADS; i++)
    {
        pthread_join(t[i], NULL);
    }
    */

    printf("Vetor da média aritimétrica:\n");
    for(int i = 0; i < row; i++)
    {
        printf("%lf, ", resp_med_arit[i]);
    }
    printf("\n");
    printf("Vetor da média geométrica:\n");
    for(int i = 0; i < col; i++)
    {
        printf("%lf, ", resp_med_geom[i]);
    }
    printf("\n");

    return 0;
}