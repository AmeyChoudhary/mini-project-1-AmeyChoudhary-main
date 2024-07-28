#include "headers.h"
extern int background_proc[MAX_BACKGROUND_PROCESSES];
extern int background_proc_ptr;

int bg(int fore_background, char ** token_array , int size_array)
{
    if(size_array != 2)
    {
        printf("Invalid number of arguments\n");
        return 0;
    }

    int pid = atoi (token_array[1]);

    if(pid == 0)
    {
        printf("Invalid pid.\n");
        return 0;
    }

    int co_flag = 0;

    for (int i = 0; i < background_proc_ptr; i++)
    {
        if(background_proc[i] == pid)
        {
            co_flag = 1;
            break;
        }
    }

    if(co_flag == 0)
    {
        printf("No such pid exists spawned by this shell.\n");
        return 0;
    }



    // change the status of process in background itself
    int kill_status = kill(pid, SIGCONT);

    if(kill_status < 0)
    {
        perror("kill");
        return 0;
    }
    return 0;
}