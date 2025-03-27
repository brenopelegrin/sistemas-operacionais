#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#define ITERATIONS 100000000 // 100 milhões de iterações

int main() {
    // ----------------------------------------
    // 1. Demonstração de brk/sbrk (Heap)
    // ----------------------------------------
    printf("\n[1] Modificando o limite do heap com sbrk:\n");
    void *initial_break = sbrk(0); // Break inicial
    printf("Endereço inicial do heap: %p\n", initial_break);

    // Aumenta o heap em 4096 bytes (1 página)
    sbrk(4096);
    void *new_break = sbrk(0);
    printf("Novo endereço do heap:    %p\n", new_break);

    // ----------------------------------------
    // 2. Demonstração de mmap (Memória mapeada)
    // ----------------------------------------
    printf("\n[2] Alocando memória com mmap:\n");
    void *mapped_mem = mmap(
        NULL,                   
        4096,                   
        PROT_READ | PROT_WRITE, 
        MAP_PRIVATE | MAP_ANONYMOUS, 
        -1,                     
        0                       
    );

    if (mapped_mem == MAP_FAILED) {
        perror("Falha no mmap");
        return 1;
    }
    printf("Memória mapeada em:      %p\n", mapped_mem);

    // ----------------------------------------
    // 3. Operações intensivas na memória
    // ----------------------------------------
    printf("\n[3] Escrevendo na memória %d vezes...\n", ITERATIONS);
    volatile char *mem = (char *)mapped_mem; // volatile evita otimizações
    for (int i = 0; i < ITERATIONS; i++) {
        mem[i % 4096] = i % 256; // Acesso cíclico à memória
    }

    // ----------------------------------------
    // 4. Demonstração de mprotect (Proteção)
    // ----------------------------------------
    printf("\n[4] Alterando permissões com mprotect:\n");
    
    if (mprotect(mapped_mem, 4096, PROT_READ) == -1) {
        perror("Falha no mprotect");
        munmap(mapped_mem, 4096);
        return 1;
    }
    printf("Permissões alteradas para somente leitura!\n");

    // ----------------------------------------
    // Limpeza
    // ----------------------------------------
    munmap(mapped_mem, 4096);
    brk(initial_break);

    return 0;
}