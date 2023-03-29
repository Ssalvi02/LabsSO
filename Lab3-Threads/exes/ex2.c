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

// vetores de resultado
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

int main()
{
    int row, col; 
    int op;
    int **mat;
    char filename[200];

    // seleciona ação
    do{
        printf("choose what to do:\n[1] - generate matrix\n[2] - load matrix from file\n");
        scanf("%d", &op);
    } while(op != 1 && op != 2);

    if (op == 1){

        printf("select number of lines >> ");
        scanf("%d", &row);
        printf("select number of columns >> ");
        scanf("%d", &col);

        // gera matriz aleatória
        mat = create_matrix(row, col);
        srand(time(NULL));  
        generate_elements(mat, row, col, 100);

        printf("select output matrix file name >> ");
        scanf("%s", filename);
        strcat(filename, ".in");
        
        // escreve matriz em um arquivo para uso posterior
        write_matrix_to_file(filename, mat, row, col);

    } else {
        // carrega matriz de um arquivo
        printf("select input matrix file >> ");
        scanf("%s", filename);
        printf("reading matrix...\n");
        mat = read_matrix_from_file(filename, &row, &col);
        printf("done reading matrix of size %dx%d.\n", row, col);
    }

    int n_threads = 16;
    printf("select number of threads >> ");
    scanf("%d", &n_threads);

    /*--------------------------------------------------------------------------------------*/

    // define o número de threads como o maior valor entre linhas, colunas e n_threads selecionado
    // para que threads não fiquem ociosas
    if(n_threads > row || n_threads > col)
        n_threads = (row < col) ? row : col;
    
    printf("number of threads: %d\n", n_threads);
    
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

    /* --- fim do processamento --- */
    
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