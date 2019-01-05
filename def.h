// PROCESS STATES
#include<string>
#include<iostream>
#include<fstream>
#include<unordered_map>
#include<string>
#include<unistd.h>
#include<cstdio>
#include<regex>

#include<wait.h>
#include<stdlib.h>
#include<pthread.h>
#include<queue>

#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unordered_map>

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

//configuration
int loadDefaultConfig();
int loadConfig();
std::string getConfig(std::string configKey);

//executer
int executeCommandAsync(char* command);