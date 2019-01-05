#include<unistd.h>
#include<wait.h>
#include<stdlib.h>
#include<pthread.h>
#include<queue>
// PROCESS STATES
#define PROCESS_STOPPED 0
#define PROCESS_RUNNING 1
#define PROCESS_ERROR 2

#define R 0
#define W 1

struct process
{
    int pid;
    char* command;
    int exit_status;
    int state;
};


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
        sleep(2);
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