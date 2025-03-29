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
        // Se fork falhar, imprime erro detalhado e sai
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Código executado pelo filho
        printf("  Processo filho criado! PID: %d, PPID: %d\n", getpid(), getppid());
        
        // Trabalho do filho
        printf("  Filho está trabalhando...\n");
        // Simula trabalho com sleep
        sleep(1);
        // Loop de CPU para simular trabalho e aumentar o user time
        for (int i = 0; i < 1000000; i++) {
            volatile int temp = i * i;
        }
        exit(0);  // Termina com status 0 - Um exemplo
    } else {
        // Código executado pelo pai
        printf("  Processo pai continua. PID: %d\n", getpid());
        
      // Primitiva wait()
        printf("\n[2] Primitiva wait() - Sincronização:\n");
        int child_status;
        waitpid(pid, &child_status, 0);  // Espera pelo filho
        
        if (WIFEXITED(child_status)) {
            printf("  Filho terminou com status: %d\n", WEXITSTATUS(child_status));
        } else {
            printf("  Filho não terminou normalmente.\n");
        }
    }

    // Primitiva exec()
    // O segundo fork é usado para criar um processo filho que será substituído por um novo programa usando exec
    // Isso é necessário porque a primitiva exec substitui a imagem do processo atual pelo novo programa
    // Ao criar um novo processo antes de chamar exec, temos que ter certeza que o processo pai original continue
    // sua execução normalmente, enquanto o processo filho é substituído pelo novo programa
    printf("\n[3] Primitiva exec() - Substituição de um processo:\n");
    pid_t exec_pid = fork();
    
    if (exec_pid == 0) {
        // Código executado pelo filho
        printf("  Processo filho criado para executar comando! PID: %d\n", getpid());
        execlp("ls", "ls", "-l", NULL);  // Substitui por 'ls -l'
        // O filho não vai voltar para o código abaixo do exec
        // O exec não retorna se for bem-sucedido
        // Se falhar, o código abaixo do exec será executado
        perror("  Falha no exec");       // Só executa se falhar
        exit(1);                         // Termina imediatamente se exec falhar
        printf("  Processo filho vai substituir...\n");
        perror("  Falha no exec");       // Só executa se falhar
        exit(1);
    } else {
        wait(NULL);
        int exec_status = 0;
        waitpid(exec_pid, &exec_status, 0);
        if (WIFEXITED(exec_status)) {
        int exec_status;
            printf("  Filho não terminou normalmente.\n");
        }
        printf("  Processo pai verificou execução do comando\n");
    }

    return 0;
}
