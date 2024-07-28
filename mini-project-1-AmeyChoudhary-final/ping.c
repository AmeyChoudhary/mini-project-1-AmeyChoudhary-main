#include "headers.h"

extern int background_proc[MAX_BACKGROUND_PROCESSES];
extern int background_proc_ptr;

int ping(int forebackgrund, char ** tokenarray, int sizearray)
{
    if (sizearray != 3)
    {
        printf("ping: Invalid number of arguments\n");
        return 1;
    }

    int pid = atoi(tokenarray[1]);

    int sig = atoi(tokenarray[2]);
    sig = sig % MACHINE_MODULO;

    // check if process exists
    char proc_path_stat[MAX_PATH_SIZE];
    sprintf(proc_path_stat, "/proc/%d/stat", pid);

    FILE *stat_file = fopen(proc_path_stat, "r");
    if (stat_file == NULL)
    {
        printf("Error: Process with pid %d does not exist\n", pid);
        return 1;
    }

    fclose(stat_file);

    int co_flag = 0;
    for (int i = 0; i < background_proc_ptr; i++)
    {
        if (background_proc[i] == pid)
        {
            co_flag =1;
            break;
        }
    }

    if(co_flag == 0)
    {
        printf("Error: Process with pid %d exists but not part of this shell. So not sent a signal\n", pid);
        return 1;

    }

    // sent signal to process
    int kill_ret = kill(pid, sig);

    if (kill_ret == -1)
    {
        printf("Error in sending signal to process with pid %d\n", pid);
        return 1;
    }




    return 0;
}