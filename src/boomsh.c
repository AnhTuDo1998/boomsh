#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROMPT "boomsh >> "
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
    char * separator = " ";
    char * token;
    int index = 0;

    //Parsing token
    token = strtok(input_string, separator);
    while(token!=NULL){
        command[index] = token;
        index++;

        // Repeat, start from NULL marker of prev token
        token = strtok(NULL, separator);
    }

    // Last index should be null terminated
    // TODO: What if more than MAX_ARGS???
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

        // Execute the command through child process
        child_pid = fork();
        if (child_pid == 0){
            // Inside child body
            execvp(command[0], command);
            printf("This line will never printed if exec success!\n");
        }
        else{
            // Wait for the child process to finish
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }

        // Clean up before next input
        free(input);
        free(command);
    }

    // Clean up shell and Exit
    return EXIT_SUCCESS;
}