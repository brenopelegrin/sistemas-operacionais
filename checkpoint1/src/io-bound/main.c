#include <stdio.h>
#include <fcntl.h>      // lib for open()
#include <unistd.h>     // lib for write() and close()
#include <string.h>     // lib for strlen()
#include <sys/stat.h>   // lib for mkdir()
#include <errno.h>      // lib for errno and EEXIST

// Number of files to create
#define FILE_COUNT 100

// Number of times to write to each file
#define WRITE_TIMES 2000
#define WRITE_DATA "Testing string blah blah blah.\n"

// Directory to use for writing files
#define BASE_DIR "/tmp/checkpoint1/io-bound/"  

/**
 * Function: create_directory
 * --------------------------
 * Creates the test directory if it doesn't exist
 */
void create_directory(const char *path) {
    if (mkdir("/tmp", 0777) && errno != EEXIST) {
        perror("Failed to create /tmp");
        return;
    }
    if (mkdir("/tmp/checkpoint1", 0777) && errno != EEXIST) {
        perror("Failed to create /tmp/checkpoint1");
        return;
    }
    if (mkdir("/tmp/checkpoint1/io-bound", 0777) && errno != EEXIST) {
        perror("Failed to create /tmp/checkpoint1/io-bound");
        return;
    }
}

/**
 * Function: create_and_write_files
 * --------------------------------
 * Creates files in the test directory and write to these files using Linux syscalls
 */
void create_and_write_files() {
    // Buffer for the file path
    char filename[128];  
    char buffer[] = WRITE_DATA;
    size_t buffer_len = strlen(buffer);

    // First, verify if the directory exists
    create_directory(BASE_DIR);

    for (int i = 0; i < FILE_COUNT; i++) {
        // Generate file paths like: /tmp/checkpoint1/io-bound/output_{counter}.txt
        snprintf(filename, sizeof(filename), "%soutput_%d.txt", BASE_DIR, i);

        // Open the file using Linux open() syscall
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("Error opening file");
            continue;
        }

        // Write WRITE_TIMES lines with the test string to the file
        for (int j = 0; j < WRITE_TIMES; j++) {
            if (write(fd, buffer, buffer_len) == -1) {
                perror("Error writing to file");
                break;
            }
        }

        // Close the file using Linux close() syscall
        if (close(fd) == -1) {
            perror("Error closing file");
        }
    }
}

int main() {
    create_and_write_files();
    return 0;
}
