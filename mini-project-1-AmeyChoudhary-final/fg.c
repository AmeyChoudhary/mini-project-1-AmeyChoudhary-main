#include "headers.h"

extern int background_proc[MAX_BACKGROUND_PROCESSES];
extern int background_proc_ptr;
extern int background_proc_copy[MAX_BACKGROUND_PROCESSES];
extern int background_proc_ptr_copy;
extern int shell_pid;
extern int curr_pid;

int flag_g;

void sig_z_f()
{
    if (curr_pid == -1)
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
                flag = 1;
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
        flag_g = 0;
    }
}

int fg(int fore_background, char **token_array, int size_array)
{
    flag_g = 1;
    if (size_array != 2)
    {
        printf("Invalid number of arguments\n");
        return 0;
    }

    int pid = atoi(token_array[1]);

    if (pid == 0)
    {
        printf("No such pid exists.\n");
        return 0;
    }

    int co_flag = 0;

    for (int i = 0; i < background_proc_ptr; i++)
    {
        if (background_proc[i] == pid)
        {
            co_flag = 1;
            break;
        }
    }

    if (co_flag == 0)
    {
        printf("No such pid exists spawned by this shell.\n");
        return 0;
    }

    // copilot
    signal(SIGTTOU, SIG_IGN); // restarting the signal of CTRL C and CTRL D here
    signal(SIGTTIN, SIG_IGN);
    kill(pid, SIGCONT);
    curr_pid = pid;
    if (tcsetpgrp(0, getpgid(pid)))
    {
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        return 0;
    }
    int status;
    waitpid(pid, &status, WUNTRACED);

    tcsetpgrp(0, shell_pid);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTSTP, sig_z_f);

    // removing the process from background process list
    if (flag_g == 1) // if the process is not stopped before, then only remove it
    {
        for (int i = 0; i < background_proc_ptr; i++)
        {
            if (background_proc[i] == pid)
            {
                for (int j = i; j < background_proc_ptr - 1; j++)
                {
                    background_proc[j] = background_proc[j + 1];
                    background_proc_copy[j] = background_proc_copy[j + 1];
                }
                background_proc_ptr--;
                background_proc_ptr_copy--;
                break;
            }
        }
    }

    return 0;
}