#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void buscar(int *vetor, int tamanho, int valor) {
    for (int i = 0; i < tamanho; i++) 
    {
        if (vetor[i] == valor) 
        {
            exit(0);
        }
    }
    exit(1);
}



int main() {
    int tam = 30, vetor[tam], n = 6; //tam -> tamanho do vetor | n -> numero de filhos
    int valor = 5; // valor a ser procurado
    int tamanho = sizeof(vetor) / sizeof(int);
    int intervalo = tamanho / n; // tamanho do intervalo de cada filho
    pid_t pid;
    srand(8343);

    for(int i = 0; i < tam; i++)
    {
        vetor[i] = rand() % 6;
    }


    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid == 0) //FILHO 
        { 
            int inicio = i * intervalo;
            int fim = (i + 1) * intervalo;
            if (i == n - 1) 
            {
                fim = tamanho; // restante do vetor
            }
            buscar(&vetor[inicio], fim - inicio, valor);
        }
    }

    // processo pai espera os filhos terminarem
    int status;
    for (int i = 0; i < n; i++) 
    {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) 
        {
            printf("Filho %d encontrou o valor\n", waitpid(-1, &status, 0));
        }
    }

    return 0;
}