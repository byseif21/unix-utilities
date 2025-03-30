#include <stdio.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <unistd.h>  

#define BUFFER_SIZE 1024  

int cp_main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }

    int src = open(argv[1], O_RDONLY);
    if (src == -1) {
        perror("Error opening source file");
        return 1;
    }

    int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest == -1) {
        perror("Error opening destination file");
        close(src);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    
    while ((bytesRead = read(src, buffer, BUFFER_SIZE)) > 0) {
        if (write(dest, buffer, bytesRead) != bytesRead) {
            perror("Error writing to destination file");
            close(src);
            close(dest);
            return 1;
        }
    }

    if (bytesRead == -1) {
        perror("Error reading source file");
    }

    close(src);
    close(dest);
    
    return (bytesRead == -1) ? 1 : 0;
}
