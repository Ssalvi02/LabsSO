/*
    Descrição: Cria uma hierarquia de processos com N níveis.
    Autores: João Victor Salvi Silva
             Gabriela Paola Sereniski
    Data de criação: 20/03/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int tam = 5; //N filhos
    pid_t id;

    for(int i = 0; i < tam; i++)
    {
        id = fork(); //Faz os forks
    }

    sleep(5);
    return 0;
}
