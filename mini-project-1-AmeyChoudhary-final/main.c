#include "headers.h"

// I have used Chatgpt and Copilot for assistance here. Specified in the comments. Might have missed

// defining global variables
char hd[MAX_PATH_SIZE];
char prev_dir[MAX_PATH_SIZE];
char global_input[INPUT_SIZE];
extern int background_proc[MAX_BACKGROUND_PROCESSES];
extern int background_proc_ptr;
extern int background_proc_copy[MAX_BACKGROUND_PROCESSES];
extern int background_proc_ptr_copy;
int shell_pid = 0;
int curr_pid = -1;


struct termios orig_termios;

void die_m(const char *s)
{
    perror(s);
    exit(1);
}

void disableRawMode_m()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die_m("tcsetattr");
}

void enableRawMode_m()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die_m("tcgetattr");
    atexit(disableRawMode_m);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die_m("tcsetattr");
}

// function to check if pointer pointing to string contains pastevents or pastevents execute
// used the aid of Copilot in aid for string parsing
int check_pastevents(char *str)
{
    char address_past_events[MAX_PATH_SIZE];
    strcpy(address_past_events, hd);
    strcat(address_past_events, "/.pastevents");
    FILE *file = fopen(address_past_events, "r");
    if (file == NULL)
    {
        // File doesn't exist, so create it
        file = fopen(address_past_events, "w");
        if (file == NULL)
        {
            perror("Error creating file");
            return 1;
        }
        // printf(".pastevents file created.\n");
    }

    char *pastevents_array[PASTEVENTS_NUMBER];
    char line[STRING_SIZE];
    int pastevents_array_ptr = 0;
    while (fgets(line, sizeof(line), file))
    {
        // printf("%s", line);
        pastevents_array[pastevents_array_ptr] = strdup(line);
        pastevents_array[pastevents_array_ptr][strcspn(pastevents_array[pastevents_array_ptr], "\n")] = '\0'; // remove the \n from the string
        pastevents_array_ptr++;
        if (pastevents_array_ptr == PASTEVENTS_NUMBER)
        {
            break;
        }
    }
    fclose(file);

    int can_be_stored = 1;
    char temp_str[STRING_SIZE];
    strcpy(temp_str, str);

    char *token = strtok(str, ";&");
    char *token_list[TOKEN_SIZE];
    int token_list_ptr = 0;
    while (token != NULL)
    {
        token_list[token_list_ptr] = token;
        token_list_ptr++;
        token = strtok(NULL, ";&");
    }
    token_list[token_list_ptr] = NULL;

    for (int i = 0; i < token_list_ptr; i++)
    {
        char temp_str2[STRING_SIZE];
        strcpy(temp_str2, token_list[i]);
        char *token2 = strtok(temp_str2, " \n\t\r");
        char *token2_list[TOKEN_SIZE];
        int token2_list_ptr = 0;
        while (token2 != NULL)
        {
            token2_list[token2_list_ptr] = token2;
            token2_list_ptr++;
            token2 = strtok(NULL, " \n\t\r");
        }
        token2_list[token2_list_ptr] = NULL;
        if (strcmp(token2_list[0], "pastevents") == 0)
        {
            if (token2_list_ptr == 1)
            {
                // pastevents
                can_be_stored = 0;
            }
            else if (strcmp(token2_list[1], "purge") == 0)
            {
                // pastevents purge
                can_be_stored = 0;
            }
            else if (strcmp(token2_list[1], "execute") == 0)
            {
                // pastevent
                int index = atoi(token2_list[2]);

                // replace token2_list[2] with the command at that index
                strcpy(token_list[i], pastevents_array[pastevents_array_ptr - index]);
            }
        }
    }

    // using modified token_list, create a new string with same delimiters as original string
    char new_str[STRING_SIZE];
    strcpy(new_str, "");
    for (int i = 0; i < token_list_ptr; i++)
    {
        strcat(new_str, token_list[i]);
        int stringlength = strlen(new_str);
        // now check what delimiter was in original input and add it to new_str
        if (temp_str[stringlength] == '&')
        {
            strcat(new_str, "&");
        }
        else if (temp_str[stringlength] == ';')
        {
            strcat(new_str, ";");
        }
        else
        {
            strcat(new_str, "");
        }
    }

    strcpy(global_input, new_str);
    global_input[strcspn(global_input, "\n")] = '\0'; // remove the \n from the string

    // printf("%s\n", global_input);

    return can_be_stored;
}

// function to check for presence of redirection operators or pipes
int check_pipes(char **tokenarray, int sizearray)
{
    int pipe_flag = 0;
    int input_flag = 0;
    int output_flag_1 = 0; // >
    int output_flag_2 = 0; // >>

    for (int i = 0; i < sizearray; i++)
    {
        if (strcmp(tokenarray[i], "|") == 0)
        {
            pipe_flag++;
        }
        else if (strcmp(tokenarray[i], "<") == 0)
        {
            input_flag++;
        }
        else if (strcmp(tokenarray[i], ">") == 0)
        {
            output_flag_1++;
        }
        else if (strcmp(tokenarray[i], ">>") == 0)
        {
            output_flag_2++;
        }
    }

    if (pipe_flag > 0 || input_flag > 0 || output_flag_1 > 0 || output_flag_2 > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// used the aid of Copilot in aid for string parsing
int parse_string_exec(char *str, int exit_status)
{
    // check if the string contains pastevents or pastevents execute
    int can_be_stored = check_pastevents(str);
    strcpy(str, global_input);

    str[strcspn(str, "\n")] = '\0'; // remove the \n from the string

    char *token = strtok(str, ";");
    char *token_list[TOKEN_SIZE];
    int token_list_ptr = 0;

    while (token != NULL)
    {
        // adding tokens to a list
        token_list[token_list_ptr] = token;
        token_list_ptr++;
        token = strtok(NULL, ";");
    }
    token_list[token_list_ptr] = NULL;

    for (int i = 0; i < token_list_ptr; i++)
    {
        char temp[STRING_SIZE];
        strcpy(temp, token_list[i]);
        // check if the string contains &

        int and_flag = 0;
        for (int j = 0; j < strlen(temp); j++)
        {
            if (temp[j] == '&')
            {
                and_flag++;
            }
        }

        char *token2 = strtok(temp, "&");
        char *token2_list[TOKEN_SIZE];
        int token2_list_ptr = 0;
        while (token2 != NULL)
        {
            // adding tokens to a list
            token2_list[token2_list_ptr] = token2;
            token2_list_ptr++;
            token2 = strtok(NULL, "&");
        }
        token2_list[token2_list_ptr] = NULL;

        int fore_background = 0; // 0 for foreground, 1 for background. Default is foreground
        for (int j = 0; j < token2_list_ptr; j++)
        {
            if (and_flag > 0)
            {
                fore_background = 1;
                and_flag--;
            }
            else
            {
                fore_background = 0;
            }
            char temp2[STRING_SIZE];
            strcpy(temp2, token2_list[j]);

            char temp3[STRING_SIZE];
            strcpy(temp3, token2_list[j]); // in passing to exec_redirec, we need to pass the original string

            // find the commands and arguments
            char *token3 = strtok(temp2, " \n\t\r");
            char *token3_list[TOKEN_SIZE];
            int token3_list_ptr = 0;
            while (token3 != NULL)
            {
                // adding tokens to a list
                token3_list[token3_list_ptr] = token3;
                token3_list_ptr++;
                token3 = strtok(NULL, " \n\t\r");
            }
            token3_list[token3_list_ptr] = NULL;
            if (strcmp(token3_list[0], "exit") == 0)
            {
                if (can_be_stored == 1)
                {
                    pastevents(fore_background, token3_list, token3_list_ptr, can_be_stored);
                }

                if (token3_list_ptr == 1)
                {
                    exit_status = 1;
                    break;
                }
            }

            int redirec = check_pipes(token3_list, token3_list_ptr);

            if (redirec == 1)
            {
                exec_redirec(fore_background, token3_list, token3_list_ptr, can_be_stored, temp3);
                // ensuring that
            }
            else
            {
                exec_command(fore_background, token3_list, token3_list_ptr, can_be_stored);
            }
        }

        if (exit_status == 1)
        {
            break;
        }
    }

    return exit_status;
}

void sig_z()
{
    if(curr_pid == -1)
    {
        return;
    }
    else 
    {
        int flag = 0;
        for (int i = 0; i < background_proc_ptr; i++)
        {
            if (background_proc[i] == curr_pid)
            {
                flag =1;
                break;
            }
        }

        if (flag == 0)
        { 
            // will come flag == 0  always, but still just checking for safety
            printf("Process with pid %d stopped and sent to background.\n", curr_pid);
            background_proc[background_proc_ptr++] = curr_pid;
            background_proc_copy[background_proc_ptr_copy++] = curr_pid;
        
        }
        kill(curr_pid, SIGTSTP);
        curr_pid = -1;
    }

}



int main()
{
    // char hd[MAX_PATH_SIZE];    // Home directory
    getcwd(hd, MAX_PATH_SIZE);       // Get the home directory, it keeps track of where the shell is running from
    getcwd(prev_dir, MAX_PATH_SIZE); // Initialise the prev_dir as home directory

    int exit_status = 0; // Exit status of the last command

    memset(background_proc, 0, sizeof(background_proc));
    background_proc_ptr = 0;
    memset(background_proc_copy, 0, sizeof(background_proc_copy));
    background_proc_ptr_copy = 0;

    char temp_addr[STRING_SIZE];
    strcpy(temp_addr, hd);
    strcat(temp_addr, "/tmp.txt");

    FILE *file = fopen(temp_addr, "r+");

    if (file == NULL)
    {
        // File doesn't exist, so create it
        file = fopen(temp_addr, "w");
        if (file == NULL)
        {
            perror("Error creating file");
            return 1;
        }
    }

    fclose(file);

    shell_pid = getpid();

    // Register signal handler to handle keyboard interrupts
    signal(SIGINT, SIG_IGN);  // CTRL + C, disables the signal
    signal(SIGTSTP, sig_z); // CTRL + Z, goes to sig_z
    signal(SIGTTOU, SIG_IGN); // CTRL + D , disables the signal


    // Keep accepting commands
    while (1)
    {

        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt();
        char input[INPUT_SIZE];
        // fgets(input, INPUT_SIZE, stdin);
        char input_C;
        int len_input = 0;

        enableRawMode_m();
        // changed the input method in order to get keyboard interrupts. Help from Chatgpt and Copilot
        while (EOF != (input_C = getchar()) && input_C != '\n')
        {

            if (input[strlen(input) - 1] == '\n')
            {
                break;
            }
            else if (isprint(input_C))
            {
                printf("%c", input_C);
                input[len_input++] = input_C;
            }
            else if (input_C == 4)
            {
                // printf("EOF\n");

                if (len_input == 0)
                {
                    printf("\n");
                    exit_status = 1;
                }
                else
                {
                    printf("\n");
                    input[len_input++] = '\n';
                }
                break;
            }
            else if (input_C == 127)
            {
                // printf("Backspace\n");
                if (len_input > 0)
                {
                    printf("\b \b");
                    input[len_input--] = '\0';
                }
            }
        }
        printf("\n");
        input[len_input] = '\0';

        // input[strcspn(input, "\n")] = '\0'; // Remove trailing newline character

        strcpy(global_input, input);

        exit_status = parse_string_exec(input, exit_status);

        if (exit_status == 1)
        {
            exit(EXIT_SUCCESS);
        }
        disableRawMode_m();
    }
    return 0;
}