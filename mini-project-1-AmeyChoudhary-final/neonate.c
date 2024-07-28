#include "headers.h"

// Used Chatgpt assistance for helping in main function

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO );
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

void printPidPeriodically(int time_arg)
{
    while (1)
    {

        sleep(time_arg);

        FILE *stat_file = fopen("/proc/loadavg", "r");

        if (stat_file == NULL)
        {
            printf("Error in opening file");
            exit(1);
        }

        char line[STRING_SIZE];

        if (fgets(line, STRING_SIZE, stat_file) != NULL)
        {
            long long int last_pid = 0;

            char *token = strtok(line, " ");

            while (token != NULL)
            {
                last_pid = atoi(token);
                token = strtok(NULL, " ");
            }

            printf("%lld\n", last_pid);
        }

        fclose(stat_file);
    }
}

int neonate(int fore_background, char **token_array, int size_array)

{

    if (size_array != 3)
    {
        printf("Invalid number of arguments\n");
        return 1;
    }

    int num = atoi(token_array[2]);
    if (num < 0)
    {
        printf("Invalid argument\n");
        return 1;
    }

    int time_arg = num;

    enableRawMode();
    // used ChatGPT assistance for helping from here 
    pid_t pid = fork();

    if (pid == -1)
    {
        die("fork");
    }
    else if (pid == 0)
    {
        // Child process
        printPidPeriodically(time_arg);
        exit(0);
    }
    else
    {
        // Parent process
        char c;
        while (1)
        {
            if (read(STDIN_FILENO, &c, 1) == 1)
            {
                if (c == 'x')
                {
                    kill(pid, SIGTERM); // Terminate the PID printing process
                    break;
                }
            }
        }
    }

    disableRawMode();

    return 0;
}
