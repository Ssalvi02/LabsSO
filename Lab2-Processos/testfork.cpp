#include <stdio.h>
#include <unistd.h>

int main() {
   int i;

   for(i=0; i<3; i++) {
      pid_t pid = fork();
      if(pid == 0) { // processo filho
         printf("Processo filho %d executando iteração %d\n", getpid(), i);
         sleep(1);
      }
      else if(pid > 0) { // processo pai
         printf("Processo pai criou processo filho %d para executar iteração %d\n", pid, i);
      }
      else { // erro ao criar processo filho
         fprintf(stderr, "Erro ao criar processo filho\n");
         return 1;
      }
   }

   return 0;
}
