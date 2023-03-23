/*
    Descrição: Recebe um comando linux e o executa via execvp.
    Autores: João Victor Salvi Silva
             Gabriela Paola Sereniski
    Data de criação: 20/03/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t id;
    id = fork(); //Cria o filho, faz o fork

    char* comando = "grep";
    char *args[] = {"grep", "wait.h", "ex2.cpp", NULL};

    if(id)
    {
        wait(NULL); // Espera o filho
    }
    else
    {
        execvp(comando, args); // Executa o comando
    }

    return 0;
}