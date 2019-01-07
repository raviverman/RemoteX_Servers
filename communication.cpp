#include "def.h"

static struct sockaddr_in server;
struct sockaddr_in  monitor;
struct monitor_info mon_info;
int serverSocket;
extern bool isVerbose;
extern std::unordered_map<string, string> config;
// Each "client" is a server to which a central manager connects to assign  

int startServer()
{
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = PF_INET;
    server.sin_port = htons(4020);

    serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int sockopt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
    if(bind(serverSocket, (struct sockaddr*)&server, sizeof(server) )  < 0)
    {
        //replace with actual perrror
        std::cout<<"Port bind error\n";
        return -1;
    }

    listen(serverSocket, 1);
    socklen_t monitorSize = sizeof(monitor);
    int monitorSocket = accept(serverSocket, (struct sockaddr*)&monitor, &monitorSize);

    //copying monitor info to data structure
    memcpy(&mon_info.monitor, &monitor, sizeof(struct sockaddr_in));
    mon_info.socket = monitorSocket;

    return monitorSocket;
}

void readMessage(char *buffer, message* m)
{
    memcpy(m, buffer, sizeof(message));
}

void writeResponse(response* resp, char* buffer)
{
    memcpy(buffer, resp ,sizeof(response));
}

void* commandHandler(void* id)
{
    int serverSocket = mon_info.socket;
    //read message
    char buffer[512];
    while(1)
    {
        int x = read(serverSocket, buffer, sizeof(buffer));
        message m;
        if(sizeof(message) == x)
        {
            readMessage(buffer, &m);
            if(m.type == MESS_EXEC_COMM)
            {
                if(m.isshellcmd)
                {
                    executeCommandAsync(m.command);
                }
                else
                {
                    char cmd[256] = {0};
                    const char format[256] = "\"cd %s && %s\"";
                    sprintf(cmd, format, config["ROOT"], m.command);
                    executeCommandAsync(cmd);
                }
            }
        }

    }
}