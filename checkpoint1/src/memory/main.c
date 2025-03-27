#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

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
        NULL,                   // Endereço sugerido (deixa o kernel escolher)
        4096,                   // Tamanho de 1 página
        PROT_READ | PROT_WRITE, // Permissões: leitura/escrita
        MAP_PRIVATE | MAP_ANONYMOUS, // Memória anônima (não vinculada a arquivo)
        -1,                     // Descritor de arquivo (não usado)
        0                       // Offset (não usado)
    );

    if (mapped_mem == MAP_FAILED) {
        perror("Falha no mmap");
        return 1;
    }
    printf("Memória mapeada em:      %p\n", mapped_mem);

    // ----------------------------------------
    // 3. Demonstração de mprotect (Proteção)
    // ----------------------------------------
    printf("\n[3] Alterando permissões com mprotect:\n");
    
    // Muda para somente leitura
    if (mprotect(mapped_mem, 4096, PROT_READ) == -1) {
        perror("Falha no mprotect");
        munmap(mapped_mem, 4096);
        return 1;
    }
    printf("Permissões alteradas para somente leitura!\n");

    // ----------------------------------------
    // Limpeza
    // ----------------------------------------
    munmap(mapped_mem, 4096);  // Libera memória mapeada
    brk(initial_break);        // Restaura o heap ao tamanho original

    return 0;
}