#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    int tam_comando = 1024;
    char comando[tam_comando];
    
    //Loopa igual um jogo
    while (1) {
        // exibe o prompt
        printf("> ");
        
        // se a entrada for NULL, break;
        if (fgets(comando, tam_comando, stdin) == NULL) 
        {
            break;
        }
        
        // não quebra linha
        int len = strlen(comando);
        if (comando[len - 1] == '\n') 
        {
            comando[len - 1] = '\0';
        }
        
        // comando em segundo plano?
        bool background = false;
        if (len > 0 && comando[len - 1] == '&') 
        {
            background = true;
            comando[len - 1] = '\0';
        }
        
        // executa o comando em um processo filho, faz o fork
        pid_t pid = fork();
        
        if (pid == 0) //FILHO 
        {
            system(comando);
            exit(EXIT_SUCCESS);
        } 
        else // PAI 
        {
            if (!background) 
            {
                // espera pelo filho se não estiver sendo executado em segundo plano
                int status;
                int wait = waitpid(pid, &status, 0);
                if (wait == -1) 
                {
                    perror("waitpid"); // deu ruim e não retorna o pid;
                }
            }
        }
    }
    
    return EXIT_SUCCESS;
}
