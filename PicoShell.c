#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

int picoshell_main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    int last_status = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int running = 1;

    while (running) {
        printf("Pico shell prompt > ");
        fflush(stdout);

        nread = getline(&line, &len, stdin);
        if (nread == -1) break;

        if (line[nread - 1] == '\n') line[nread - 1] = '\0';

        char **args = NULL;
        int arg_count = 0;
        char *token = strtok(line, " ");
        while (token != NULL) {
            args = (char**) realloc(args, sizeof(char*) * (arg_count + 2));
            args[arg_count] = strdup(token);
            arg_count++;
            token = strtok(NULL, " ");
        }

        if (arg_count == 0) {
            free(args);
            continue;
        }

        args[arg_count] = NULL;

        if (strcmp(args[0], "exit") == 0) {
            printf("Good Bye\n");
            running = 0;
        } else if (strcmp(args[0], "echo") == 0) {
            for (int i = 1; i < arg_count; i++) {
                printf("%s", args[i]);
                if (i < arg_count - 1) printf(" ");
            }
            printf("\n");
        } else if (strcmp(args[0], "pwd") == 0) {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("pwd");
                last_status = 1;
            }
        } else if (strcmp(args[0], "cd") == 0) {
            if (arg_count < 2) {
                fprintf(stderr, "cd: missing operand\n");
                last_status = 1;
            } else if (chdir(args[1]) != 0) {
                fprintf(stderr, "cd: %s: %s\n", args[1], strerror(errno));
                last_status = 1;
            } else {
                last_status = 0;
            }
        } else {
            pid_t pid = fork();
            if (pid == 0) {
                execvp(args[0], args);
                fprintf(stderr, "%s: command not found\n", args[0]);
                exit(1);
            } else if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    last_status = WEXITSTATUS(status);
                } else {
                    last_status = 1;
                }
            } else {
                perror("fork");
                last_status = 1;
            }
        }

        for (int i = 0; i < arg_count; i++) {
            free(args[i]);
        }
        free(args);
    }

    free(line);
    return last_status;
}
