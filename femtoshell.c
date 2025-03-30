#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int femtoshell_main(int argc, char *argv[]) {
    char *input = NULL;
    size_t size = 0;
    int running = 1;
    
    while (running) {
        printf("FemtoShell> ");
        fflush(stdout);
        
        if (getline(&input, &size, stdin) == -1) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "") == 0) {
            continue;
        }
        
        char *command = strtok(input, " ");
        
        if (command == NULL) {
            continue;
        }
        
        if (strcmp(command, "echo") == 0) {
            char *rest = input + 5; // Keeps everything after "echo "
            if (*rest) {
                printf("%s\n", rest);
            } else {
                printf("\n");
            }
        }
        else if (strcmp(command, "exit") == 0) {
            printf("Good Bye\n");
            running = 0;
        }
        else {
            printf("Invalid command\n");
        }
    }
    
    free(input);
    return 0;
}
