
#include "def.h"

std::queue<process*> processes;


process* createProcessEntry(int pid, char* command)
{
    process* newProcess = (process*)malloc(sizeof(process));
    newProcess->pid = pid;
    newProcess->command = command;
    newProcess->state = PROCESS_RUNNING;
    return newProcess;
     
}

void updateProcessEntry(process* p_node, int status)
{
    p_node->exit_status = status;
    p_node->state = PROCESS_STOPPED;
}

void* executeCommand(void* args)
{
    char* command = (char*)args;
    int pid = fork();
    if(pid==0)
    {
        // child
        execl("/bin/sh","sh","-c",command,NULL);
    }
    process* id = createProcessEntry(pid, command);
    //add process to queue
    processes.push(id);
    //memoryleak
    // sleep(3);
    int *status = (int*)malloc(sizeof(int));
    wait(status);
    updateProcessEntry(id, *status);
    return status;
}

int executeCommandAsync(char* command)
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, executeCommand, command);
    // pthread_join(thread_id, NULL);
}