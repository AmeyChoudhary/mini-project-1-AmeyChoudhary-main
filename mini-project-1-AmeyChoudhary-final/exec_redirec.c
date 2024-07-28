#include "headers.h"

// Used Chatgpt (layout) and Copilot (completion) for assistance

int exec_command_1( int fore_background, char **token_array, int size_array, int can_be_stored)
{
    // code for handling input and output redirection

    int input_flag = 0;
    int output_flag_1 = 0; // >
    int output_flag_2 = 0; // >>

    for (int i=0 ; i < size_array; i++)
    {
        if (strcmp(token_array[i], "<") == 0)
        {
            input_flag = i;
        }
        else if (strcmp(token_array[i], ">") == 0)
        {
            output_flag_1 = i;
        }
        else if (strcmp(token_array[i], ">>") == 0)
        {
            output_flag_2 = i;
        }

    }

    if (input_flag > 0)
    {
        // input redirection
        int input_fd = open(token_array[input_flag + 1], O_RDONLY);
        if (input_fd < 0)
        {
            printf("Error in opening file.\n");
            return 0;
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);

        token_array[input_flag] = NULL;
        token_array[input_flag + 1] = NULL;

        size_array = size_array - 2;
    }
    else if (output_flag_1 >0)
    {
        // output redirection
        int output_fd = open(token_array[output_flag_1 + 1], O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR); // used Copilot
        if (output_fd < 0)
        {
            printf("Error in opening file.\n");
            return 0;
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);

        token_array[output_flag_1] = NULL;
        token_array[output_flag_1 + 1] = NULL;

        size_array = size_array - 2;
    }
    else if (output_flag_2 >0)
    {
        // output redirection
        int output_fd = open(token_array[output_flag_2 + 1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR); // used Copilot
        if (output_fd < 0)
        {
            printf("Error in opening file.\n");
            return 0;
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);

        token_array[output_flag_2] = NULL;
        token_array[output_flag_2 + 1] = NULL;

        size_array = size_array - 2;
    }

    exec_command(fore_background, token_array, size_array, can_be_stored);

    return 0;
}

int exec_redirec(int fore_background, char **token_array, int size_array, int can_be_stored, char *temp) // used Chatgpts assistance
{
    // code for handling piping

    int pipe_count = 0;
    int input_flag = 0;
    int output_flag_1 = 0; // >
    int output_flag_2 = 0; // >>

    for (int i = 0; i < size_array; i++)
    {
        if (strcmp(token_array[i], "|") == 0)
        {
            pipe_count++;
        }
    }
    if (pipe_count == 0)
    {
        int input_fd;
        int output_fd;
        int original_stdin = dup(STDIN_FILENO);
        int original_stdout = dup(STDOUT_FILENO);

        for (int i = 0; i < size_array; i++)
        {
            if (strcmp(token_array[i], "<") == 0)
            {
                input_flag = i;
            }
            else if (strcmp(token_array[i], ">") == 0)
            {
                output_flag_1 = i;
            }
            else if (strcmp(token_array[i], ">>") == 0)
            {
                output_flag_2 = i;
            }
        }
        if (input_flag > 0)
        {
            // input redirection
            input_fd = open(token_array[input_flag + 1], O_RDONLY);
            if (input_fd < 0)
            {
                printf("Error in opening file.\n");
                return 0;
            }
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);

            token_array[input_flag] = NULL;
            token_array[input_flag + 1] = NULL;

            size_array = size_array - 2;
        }
        if (output_flag_1 > 0)
        {
            // output redirection
            output_fd = open(token_array[output_flag_1 + 1], O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            if (output_fd < 0)
            {
                printf("Error in opening file.\n");
                return 0;
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);

            token_array[output_flag_1] = NULL;
            token_array[output_flag_1 + 1] = NULL;

            size_array = size_array - 2;
        }
        else if (output_flag_2 > 0)
        {
            // output redirection
            output_fd = open(token_array[output_flag_2 + 1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            if (output_fd < 0)
            {
                printf("Error in opening file.\n");
                return 0;
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);

            token_array[output_flag_2] = NULL;
            token_array[output_flag_2 + 1] = NULL;

            size_array = size_array - 2;
        }

        exec_command(fore_background, token_array, size_array, can_be_stored);

        if (input_flag > 0)
        {
            dup2(STDIN_FILENO, input_fd);
            close(input_fd);
            dup2(original_stdin, STDIN_FILENO);
        }

        if (output_flag_1 > 0 || output_flag_2 > 0)
        {
            dup2(STDOUT_FILENO, output_fd);
            close(output_fd);
            dup2(original_stdout, STDOUT_FILENO);
        }
    }
    else
    {
        char *tokens = strtok(temp, "|");
        char *token_list[TOKEN_SIZE];
        int token_list_ptr = 0;

        while (tokens != NULL)
        {
            // adding tokens to a list
            token_list[token_list_ptr] = tokens;
            token_list_ptr++;
            tokens = strtok(NULL, "|");
        }

        token_list[token_list_ptr] = NULL;

        int pipes[pipe_count][2];

        for (int i = 0; i < pipe_count; i++)
        {
            if (pipe(pipes[i]) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        char temp_z[STRING_SIZE];
        strcpy(temp_z, token_list[0]);
        char *token_temp_1 = strtok(temp_z, " \n\t\r");
        char *token_list_1[TOKEN_SIZE];
        int size_array_1 = 0;

        while (token_temp_1 != NULL)
        {
            token_list_1[size_array_1] = token_temp_1;
            size_array_1++;
            token_temp_1 = strtok(NULL, " \n\t\r");
        }

        token_list_1[size_array_1] = NULL;

        for (int i = 0; i < size_array_1; i++)
        {
            if (strcmp(token_list_1[i], "<") == 0)
            {
                input_flag = i;
            }
        }

        strcpy(temp_z, token_list[pipe_count]);
        char *token_temp_2 = strtok(temp_z, " \n\t\r");
        char *token_list_2[TOKEN_SIZE];
        int size_array_2 = 0;

        while (token_temp_2 != NULL)
        {
            token_list_2[size_array_2] = token_temp_2;
            size_array_2++;
            token_temp_2 = strtok(NULL, " \n\t\r");
        }

        token_list_2[size_array_2] = NULL;

        for (int i = 0; i < size_array_2; i++)
        {
            if (strcmp(token_list_2[i], ">") == 0)
            {
                output_flag_1 = i;
            }
            else if (strcmp(token_list_2[i], ">>") == 0)
            {
                output_flag_2 = i;
            }
        }


        for (int i = 0; i < pipe_count + 1; i++)
        {
            pid_t child_pid = fork();

            if (child_pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (child_pid == 0)
            {
                // Child process

                // Redirect input from the previous pipe (if not the first command)
                if (i > 0)
                {
                    close(pipes[i - 1][1]); // Close write end of the previous pipe
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                    close(pipes[i - 1][0]);
                    // close(pipes[i - 1][0]);
                }

                // Redirect output to the next pipe (if not the last command)
                if (i < pipe_count)
                {
                    close(pipes[i][0]); // Close read end of the current pipe
                    dup2(pipes[i][1], STDOUT_FILENO);
                    close(pipes[i][1]);
                    // close(pipes[i][1]);
                }

                // Execute the command
                char *token_array[TOKEN_SIZE];
                int size_array = 0;

                char *token = strtok(token_list[i], " \n\t\r");
                while (token != NULL)
                {
                    token_array[size_array] = token;
                    size_array++;
                    token = strtok(NULL, " \n\t\r");
                }
                token_array[size_array] = NULL;

                exec_command_1(fore_background, token_array, size_array, can_be_stored);

                exit(EXIT_SUCCESS);
            }
            else
            {
                // Parent process
                // Close both ends of the pipe
                if (i > 0)
                {
                    close(pipes[i - 1][0]);
                    close(pipes[i - 1][1]);
                }
                waitpid(child_pid, NULL, 0);
            }
        }
    }

    return 0;
}