#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t id;
    id = fork();

    char* comando = "grep";
    char *args[] = {"grep", "wait.h", "ex2.cpp", NULL};

    if(id)
    {
        wait(NULL);
    }
    else
    {
        execvp(comando, args);
    }

    return 0;
}