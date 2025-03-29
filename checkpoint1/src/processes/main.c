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
        // Se fork falhar, imprime erro e sai
        exit(1);
    } else if (pid == 0) {
        // Código executado pelo filho
        printf("  Processo filho criado! PID: %d\n", getpid());
        
        // Simula trabalho do filho
        printf("  Filho está trabalhando...\n");
        // Simula trabalho com sleep
        sleep(1);
        // Loop de CPU para também simular trabalho e aumentar user time
        // Simula trabalho de CPU para fins educacionais
        for (int i = 0; i < 1000000; i++) {
            // Operação simples para consumir CPU
            volatile int temp = i * i;
        }
        exit(0);  // Termina com status 0 - Um exemplo
    } else {
        // Código executado pelo pai
        printf("  Processo pai continua. PID: %d\n", getpid());
        
      // Primitiva wait()
        printf("\n[2] Primitiva wait() - Sincronização:\n");
        int status;
        waitpid(pid, &status, 0);  // Espera pelo filho
        
        if (WIFEXITED(status)) {
            printf("  Filho terminou com status: %d\n", WEXITSTATUS(status));
        } else {
            printf("  Filho não terminou normalmente.\n");
        }
    }

    // Primitiva exec()
    // O segundo fork é usado para criar um processo filho que será substituído por um novo programa usando exec.
    printf("\n[3] Primitiva exec() - Substituição de um processo:\n");
    pid_t exec_pid = fork();
    
    if (exec_pid == 0) {
        // Código executado pelo filho
        printf("  Processo filho criado para executar comando! PID: %d\n", getpid());
        // Simula trabalho do filho
        printf("  Filho está trabalhando...\n");
        // Simula trabalho com sleep
        execlp("ls", "ls", "-l", NULL);  // Substitui por 'ls -l'
        // O filho não vai voltar para o código abaixo do exec
        // O exec não retorna se for bem-sucedido
        // Se falhar, o código abaixo do exec será executado
        perror("  Falha no exec");       // Só executa se falhar
        exit(1);                         // Termina imediatamente se exec falhar
        // Se falhar, o código abaixo do exec será executado
        printf("  Processo filho vai substituir...\n");
        perror("  Falha no exec");       // Só executa se falhar
        exit(1);
    } else {
        wait(NULL);
        int exec_status;
        waitpid(exec_pid, &exec_status, 0);
        if (WIFEXITED(exec_status)) {
            printf("  Filho terminou com status: %d\n", WEXITSTATUS(exec_status));
        } else {
            printf("  Filho não terminou normalmente.\n");
        }
        printf("  Processo pai verificou execução do comando\n");

    return 0;
}
