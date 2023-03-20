#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void print_array(int* array, int size){
    printf("[");
    int i;
    for(i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("]\n");
}

int main()
{

    int max_num = 5, tam_vet = 10, n_filhos = 3, inicio, fim, valor = 3;
    pid_t pid;
    int vet_full[10];
    srand(8573);

    //Preencher vetor
    for (int i = 0; i < tam_vet; i++)
    {
        vet_full[i] = rand() % max_num; 
    }
    print_array(vet_full, tam_vet);
    int step = tam_vet/n_filhos;
    int resto = tam_vet % n_filhos;

    for(int i = 0; i < n_filhos; i++)
    {
        pid = fork();
        

        if(pid == 0)
        {
            if (i < n_filhos - 1)
            {
                inicio = i * step;
                fim = inicio + step - 1;
            }
            else
            {
                inicio = i * step;
                fim = tam_vet - 1;
            }

            for(int j = inicio; j <= fim; j++)
            {
                if(vet_full[j] == valor)
                {
                    printf("PID: %d, %d \n", pid, j);
                    return 1;
                }
            }
        }
        else
        {
            wait(NULL);
        }
    }
}