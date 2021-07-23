#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROMPT "boomsh >> "
#define TOKEN_SEPERATOR " \t\r\n"
#define MAX_ARGS 8;

// function to read input string line
char * readline(){
    char *line;
    size_t buff_size = 0;

    buff_size = getline(&line, &buff_size, stdin);

    if (buff_size >= 0)
        return line;
    else
        exit(EXIT_FAILURE);
}

// Function to parse and tokenize command arguments
char ** parse_input(char * input_string){
    // Array of string representing command and arguments
    // 8 is a fixed, max size of arguments
    int max_num_args = MAX_ARGS;
    char ** command = malloc(max_num_args * sizeof(char *));
    char * separator = TOKEN_SEPERATOR;
    char * token;
    int index = 0;

    if (!command){
        fprintf(stderr, "\n\n[ERROR] Memory allocation failed!\n\n");
        exit(EXIT_FAILURE);
    }

    //Parsing token
    token = strtok(input_string, separator);
    while(token!=NULL){
        command[index] = token;
        index++;

        // if the upcoming index is index MAX_ARGS => already exceed
        // cannot put that one to NULL
        // must reallocate
        if (index >= max_num_args){
            max_num_args += MAX_ARGS;
            command = realloc(command, max_num_args * sizeof(char *));
            if (!command){
                fprintf(stderr, "\n\n[ERROR] Memory allocation failed!\n\n");
                exit(EXIT_FAILURE);
            }
        }

        // Repeat, start from NULL marker of prev token
        token = strtok(NULL, separator);
    }

    // Last index should be a null-string/null-pointer as required in man
    command[index] = NULL;
    return command;

}

int main(){
    // Initialize Shell
    char * input;
    char ** command;
    pid_t child_pid;
    int stat_loc;

    // Loop of read, parse and execute
    // TODO: History file?
    while(1){
        // TODO: Do this without this readline ?
        printf(PROMPT);
        input = readline();
        command = parse_input(input);

        // Built in function handling here
        // Check through and execute
        // "cd"
        if (strcmp(command[0],"cd") == 0){
            if(chdir(command[1])<0){
                fprintf(stderr, "\n\n[ERROR] Change Directory Failed!\n\n");
                exit(EXIT_FAILURE);
            }

            // Skip the fork
            continue;
        }

        //"exit"
        else if (strcmp(command[0], "exit") == 0){
            exit(EXIT_SUCCESS);
        }

        else{
            // Execute the command through child process (Non-built-in case)
            child_pid = fork();
            if (child_pid < 0){
                // Fork failed
                fprintf(stderr, "\n\n[ERROR] Forking child process failed!\n\n");
                exit(EXIT_FAILURE);
            }
            else if (child_pid == 0){
                // Inside child body
                // non-built-in
                execvp(command[0], command);
                printf("This line will never printed if exec success!\n");
            }
            else{
                // Wait for the child process to finish
                waitpid(child_pid, &stat_loc, WUNTRACED);
            }
        }
        

        // Clean up before next input
        free(input);
        free(command);
    }

    // Clean up shell and Exit
    return EXIT_SUCCESS;
}