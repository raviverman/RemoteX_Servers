#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<unistd.h>
#include<signal.h>
#include<string>
#include<unordered_map>

#include "configuration.cpp"
#include "executer.cpp"
bool isVerbose = false;


using namespace std;

int main(int argc, char const *argv[])
{
    char* s="md5sum helow";
    executeCommandAsync(s);
    executeCommandAsync(s);
    
    return 0;
}
