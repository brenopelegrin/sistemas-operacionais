#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#define ITERATIONS 100000000 // 100 million iterations

/**
 * Function: main
 * --------------
 * Calls sbrk(), mmap(), mprotect() syscalls for demonstration.
 *
 * Returns:
 *   0 - executed with success.
 *  -1 - executed with error.
 */
int main() {
    // ----------------------------------------
    // 1. Demonstration for brk/sbrk (Heap)
    // ----------------------------------------
    printf("\n[1] Modificando o limite do heap com sbrk:\n");
    void *initial_break = sbrk(0); // Initial break
    printf("Endereço inicial do heap: %p\n", initial_break);

    // Increases the heap by 4096 bytes (1 page)
    sbrk(4096);
    void *new_break = sbrk(0);
    printf("Novo endereço do heap:    %p\n", new_break);

    // ----------------------------------------
    // 2. Demonstration for mmap (Mapped Memory)
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
    // 3. Memory-intensive operations
    // ----------------------------------------
    printf("\n[3] Escrevendo na memória %d vezes...\n", ITERATIONS);
    volatile char *mem = (char *)mapped_mem; // volatile prevents optimizations
    for (int i = 0; i < ITERATIONS; i++) {
        mem[i % 4096] = i % 256; // Cyclic memory access
    }

    // ----------------------------------------
    // 4. Demonstration of mprotect (Protection)
    // ----------------------------------------
    printf("\n[4] Alterando permissões com mprotect:\n");
    
    if (mprotect(mapped_mem, 4096, PROT_READ) == -1) {
        perror("Falha no mprotect");
        munmap(mapped_mem, 4096);
        return 1;
    }
    printf("Permissões alteradas para somente leitura!\n");

    // ----------------------------------------
    // Cleanup
    // ----------------------------------------
    munmap(mapped_mem, 4096);
    brk(initial_break);

    return 0;
}
