/*
    Descrição: Calcula média aritmética de cada linha e média geométrica de cada coluna de uma matriz.
    Autores: João Victor Salvi Silva
             Gabriela Paola Sereniski
    Data de criação: 29/03/2023
*/

#include <stdio.h>   
#include <stdlib.h>     //atoi(), srand()
#include <time.h>       //time() 
#include "matriz.h"
#include <pthread.h>
#include <math.h>
#include <string.h>     //strcat()
#include <time.h>       //clock_t, clock(), CLOCKS_PER_SEC

#define N 20000
#define M 20000

double a_means_result[N];
double g_means_result[M];

struct data_chunk {
    int row, col;
    int beg, end;
    int **mat;
};

// função que calcula média aritmética de cada linha
void *a_means(void *param)
{
    struct data_chunk *d = param;

    for(int i = d->beg; i < d->end; i++)
    {
        a_means_result[i] = 0;
        for(int j = 0; j < d->col; j++)
            a_means_result[i] += (double)d->mat[i][j] / d->col;
    }
}

// função que calcula média geométrica de cada coluna
// antilog é usado para evitar overflow
void *g_means(void *param)
{
    struct data_chunk *d = param;

    for(int j = d->beg; j < d->end; j++)
    {
        double sum = 1.0;
        for(int i = 0; i < d->row; i++)
            sum += log((double)d->mat[i][j]);

        g_means_result[j] = exp(sum/d->row);
    }
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        // passa número de threads pela linha de comando
        printf("use: <number_of_threads>"); 
        exit(0);
    }

    int row = 20000, col = 20000; 
    // cria matriz
    printf("generating matrix...\n");
    int **mat = create_matrix(row, col);
    generate_elements(mat, row, col, 100);
    printf("done generating matrix of size %dx%d.\n", row, col);
    
    int n_threads = atoi(argv[1]);
    
    struct data_chunk data[n_threads];

    struct timespec start, finish;

    /* ---- inicia calculo da média aritmética ---- */

    // tempo de inicio do processamento
    clock_gettime(CLOCK_MONOTONIC, &start);  

    // distribui as linhas entre as threads
    int part = row/n_threads;
    int beg = 0;
    int end = part;

    for(int i = 0; i < n_threads; i++)
    {
        data[i].beg = beg, data[i].end = end;
        data[i].row = row, data[i].col = col;
        data[i].mat = mat;

        beg = end, end += part;  
    }
    data[n_threads-1].end = row;

    pthread_t t[n_threads];

    for(int i = 0; i < n_threads; i++)
        pthread_create(&t[i], NULL, a_means, &data[i]);

    for(int i = 0; i < n_threads; i++)
        pthread_join(t[i], NULL);

    /* ---- inicia calculo da média geométrica ---- */

    //distribui as colunas entre as threads
    part = col/n_threads;
    beg = 0;
    end = part;
    
    for(int i = 0; i < n_threads; i++)
    {
        data[i].beg = beg, data[i].end = end;
        data[i].row = row, data[i].col = col;
        data[i].mat = mat;

        beg = end, end += part;  
    }
    data[n_threads-1].end = col;

    for(int i = 0; i < n_threads; i++)
        pthread_create(&t[i], NULL, g_means, &data[i]);
    
    for(int i = 0; i < n_threads; i++)
        pthread_join(t[i], NULL);

    // tempo de fim do processamento
    clock_gettime(CLOCK_MONOTONIC, &finish);
    // calcula tempo total
    double elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("time spent: %fs\n", elapsed);

    /*--------------------------------------------------------------------------------------*/
    
    // escreve saída em arquivos texto
    FILE *fd;

    fd = fopen("ex2_arithmetic_means.txt", "w");
    for(int i = 0; i < row; i++)
        fprintf(fd, "%lf\n", a_means_result[i]);
    fclose(fd);

    fd = fopen("ex2_geometric_means.txt", "w");
    for(int i = 0; i < col; i++)
        fprintf(fd, "%lf\n", g_means_result[i]);
    fclose(fd);

    return 0;
}