#include <stdio.h>
#include <fcntl.h>      // For open()
#include <unistd.h>     // For write() and close()
#include <string.h>     // For strlen()
#include <sys/stat.h>   // For mkdir()
#include <errno.h>      // For errno and EEXIST

#define FILE_COUNT 100       // Number of files to create
#define WRITE_TIMES 2000      // Number of times to write to each file
#define WRITE_DATA "This is a test line written to the file.\n"

#define BASE_DIR "/tmp/checkpoint1/io-bound/"  // Target directory

/**
 * Function: create_directory
 * --------------------------
 * Creates the required directory structure if it does not exist.
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
 * Creates multiple files in the target directory and writes data using Linux system calls.
 */
void create_and_write_files() {
    char filename[128];  // Buffer to hold the file path
    char buffer[] = WRITE_DATA;
    size_t buffer_len = strlen(buffer);

    // Ensure the target directory exists
    create_directory(BASE_DIR);

    for (int i = 0; i < FILE_COUNT; i++) {
        // Generate unique file paths: /tmp/checkpoint1/io-bound/output_{counter}.txt
        snprintf(filename, sizeof(filename), "%soutput_%d.txt", BASE_DIR, i);

        // Open the file using low-level Linux system calls
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("Error opening file");
            continue;  // Skip this file and move to the next
        }

        // Write multiple lines to the file
        for (int j = 0; j < WRITE_TIMES; j++) {
            if (write(fd, buffer, buffer_len) == -1) {
                perror("Error writing to file");
                break;  // Stop writing if an error occurs
            }
        }

        // Close the file descriptor
        if (close(fd) == -1) {
            perror("Error closing file");
        }
    }
}

int main() {
    create_and_write_files();
    return 0;
}
