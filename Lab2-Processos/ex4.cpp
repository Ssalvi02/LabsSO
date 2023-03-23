/*
    Descrição: Um shell simples para executar comandos Linux. O programa
    divide o comando recebido e o aloca em um vetor para uma manipualção
    mais fácil.
    Autores: João Victor Salvi Silva
             Gabriela Paola Sereniski
    Data de criação: 20/03/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char comando[1024];
    char* args[64];
    int background = 0;

    while (1) 
    {
        printf(">> ");
        fgets(comando, 1024, stdin);

        // Impede de quebrar linha
        comando[strcspn(comando, "\n")] = '\0';

        // Quebra o comando por espaços e manda para um vetor
        char* token = strtok(comando, " ");
        int i = 0;

        while (token != NULL) 
        {
            args[i++] = token;

            // Vê se é pra rodar em background
            if (strcmp(token, "&") == 0) 
            {
                background = 1;
                args[i - 1] = NULL; // Remove o simbolo da lista de argumentos
                break;
            }
            token = strtok(NULL, " "); // NULL para o token 
        }

        args[i] = NULL;

        // Faz o fork
        pid_t pid = fork();

        if (pid == 0) //FILHO 
        {
            execvp(args[0], args);
            printf("Esse comando não existe.\n");
            exit(1);
        }
        else //PAI
        {
            if (!background) 
            {
                // Espera o filho acabar se não for em background
                int status;
                waitpid(pid, &status, 0);
            } 
            else 
            {
                printf("Processo %d executando em background\n", pid);
            }
        }
    }

    return 0;
}
