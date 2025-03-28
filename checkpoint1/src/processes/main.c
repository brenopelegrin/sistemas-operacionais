#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
   
    // Primitiva fork()
    printf("\n[1] Primitiva fork() - Criação de um processo:\n");
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Falha no fork()");
        exit(1);
    } else if (pid == 0) {
        // Código executado pelo filho
        printf("  Processo filho criado! PID: %d\n", getpid());
        sleep(1); // Simula trabalho
        // Loop de CPU para também simular trabalho e aumentar user time:
        for (int i = 0; i < 10000000; i++) {}
        exit(42);  // Termina com status 42
    } else {
        // Código executado pelo pai
        printf("  Processo pai continua. PID: %d\n", getpid());
        
      // Primitiva wait()
        printf("\n[2] Primitiva wait() - Sincronização:\n");
        int status;
        waitpid(pid, &status, 0);  // Espera pelo filho
        
        if (WIFEXITED(status)) {
            printf("  Filho terminou com status: %d\n", WEXITSTATUS(status));
        }
    }

    // Primitiva exec()
    printf("\n[3] Primitiva exec() - Substituição de um processo:\n");
    pid_t exec_pid = fork();
    
    if (exec_pid == 0) {
        printf("  Processo filho vai substituir...\n");
        execlp("ls", "ls", "-l", NULL);  // Substitui por 'ls -l'
        perror("  Falha no exec");       // Só executa se falhar
        exit(1);
    } else {
        wait(NULL);
        printf("  Processo pai verificou execução do comando\n");
    }

    return 0;
}
