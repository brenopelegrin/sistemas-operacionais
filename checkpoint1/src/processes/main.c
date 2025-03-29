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
        perror("Falha no fork() - Verifique se há recursos suficientes ou permissões adequadas");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Código executado pelo filho
        printf("  Processo filho criado! PID: %d, PPID: %d\n", getpid(), getppid());
        
        // Trabalho do filho
        printf("  Filho está trabalhando...\n");
        sleep(1);
        for (int i = 0; i < 1000000; i++) {
            volatile int temp = i * i;
        }
        exit(0);
    } else {
        // Código executado pelo pai
        printf("  Processo pai continua. PID: %d\n", getpid());
        
        // Primitiva wait()
        printf("\n[2] Primitiva wait() - Sincronização:\n");
        int child_status;
        waitpid(pid, &child_status, 0);
        
        if (WIFEXITED(child_status)) {
            printf("  Filho terminou com status: %d\n", WEXITSTATUS(child_status));
        } else {
            printf("  Filho não terminou normalmente.\n");
        }
    }

    // Primitiva exec()
    printf("\n[3] Primitiva exec() - Substituição de um processo:\n");
    pid_t exec_pid = fork();
    
    if (exec_pid < 0) {
        perror("Falha ao criar processo para exec");
        exit(EXIT_FAILURE);
    } else if (exec_pid == 0) {
        // Código executado pelo filho
        printf("  Processo filho criado para executar comando! PID: %d\n", getpid());
        printf("  Processo filho vai substituir...\n");
        
        // Substitui por 'ls -l'
        execlp("ls", "ls", "-l", NULL);
        
        // Se chegou aqui, o exec falhou
        perror("  Falha no exec");
        exit(EXIT_FAILURE);
    } else {
        // Código executado pelo pai
        int exec_status;
        waitpid(exec_pid, &exec_status, 0);
        
        if (WIFEXITED(exec_status)) {
            printf("  Comando executado com status: %d\n", WEXITSTATUS(exec_status));
        } else {
            printf("  Comando não terminou normalmente.\n");
        }
        printf("  Processo pai verificou execução do comando\n");
    }

    return 0;
}