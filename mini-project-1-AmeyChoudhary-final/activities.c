#include "headers.h"

extern int background_proc_copy[MAX_BACKGROUND_PROCESSES];
extern int background_proc_ptr_copy;

int getProcessNameFromPID_a(int pid, char *name, int len)
{
    char proc_path_stat[MAX_PATH_SIZE];
    sprintf(proc_path_stat, "/proc/%d/stat", pid);
    // printf("%s\n", proc_path_stat);

    FILE *stat_file = fopen(proc_path_stat, "r");
    if (stat_file == NULL)
    {
        // perror("Error in opening stat filevjnedjv");
        return 1;
    }

    fscanf(stat_file, "%*d %s", name);
    fclose(stat_file);

    return 0;
}

int activities(int fore_background, char ** token_array , int size_array)
{
    int array_size = background_proc_ptr_copy;

    if (array_size == 0)
    {
        printf("No background processes to show.\n");
        return 1;
    }

    char * process_name = (char *)calloc(STRING_SIZE , sizeof(char));
    int process_pid[ array_size ];

    for (int i = 0; i < array_size; i++)
    {
        process_pid[i] = background_proc_copy[i];
    }

    // sorting the process pids in ascending order
    for (int i = 0; i < array_size; i++)
    {
        for (int j = i + 1; j < array_size; j++)
        {
            if (process_pid[i] > process_pid[j])
            {
                int temp = process_pid[i];
                process_pid[i] = process_pid[j];
                process_pid[j] = temp;
            }
        }
    }

    for (int i = 0; i < array_size; i++)
    {
        int status;
        getProcessNameFromPID_a(process_pid[i], process_name, STRING_SIZE);
        int result = waitpid(process_pid[i], &status, WNOHANG);
        char proc_path_stat[MAX_PATH_SIZE];
        sprintf(proc_path_stat, "/proc/%d/stat", process_pid[i]);
        char proc_status[STRING_SIZE];
        if(result == 0)
        {
            FILE *stat_file = fopen(proc_path_stat, "r");
            char line[STRING_SIZE];
            // char proc_status[STRING_SIZE];
            while(fgets(line, sizeof(line), stat_file))
            {
                char *token;
                token = strtok(line, " ");
                int i = 0;
                while (token != NULL)
                {
                    if (i == 2)
                    {
                        strcpy(proc_status, token);
                        break;
                    }
                    token = strtok(NULL, " ");
                    i++;
                }

            }

            fclose(stat_file);
        }
        // used assistance from Copilot in coming up with cases

        if (WIFSIGNALED(status))
        {
            printf("%d  : %s    killed by a signal\n", process_pid[i] , process_name);
        }
        else if (WIFSTOPPED(status))
        {
            printf("%d  : %s    stopped\n", process_pid[i] , process_name);
        }
        else if (WIFCONTINUED(status))
        {
            printf("%d  : %s    continued\n", process_pid[i] , process_name);
        }
        else if ( strcmp(proc_status, "T") == 0)
        {
            printf("%d  : %s    stopped\n", process_pid[i] , process_name);
        }
        else if ( strcmp(proc_status, "R") == 0 )
        {
            printf("%d  : %s    running\n", process_pid[i] , process_name);
        }
        else if ( strcmp(proc_status, "S") == 0 )
        {
            printf("%d  : %s    running\n", process_pid[i] , process_name);
        }

    }


    return 0;
}