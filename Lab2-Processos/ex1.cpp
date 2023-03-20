#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int tam = 5;
    pid_t id;

    for(int i = 0; i < tam; i++)
    {
        id = fork();
    }

    sleep(30);
    return 0;
}
