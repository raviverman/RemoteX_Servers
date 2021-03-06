#include "def.h"

static struct sockaddr_in server;
struct sockaddr_in  monitor;
struct monitor_info mon_info;
int serverSocket;
extern bool isVerbose;
extern std::unordered_map<std::string, std::string> config;
// Each "client" is a server to which a central manager connects to assign  

int startServer()
{
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = PF_INET;
    server.sin_port = htons(atoi(getConfig("PORT").c_str()));

    serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int sockopt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
    if(bind(serverSocket, (struct sockaddr*)&server, sizeof(server) )  < 0)
    {
        //replace with actual perrror
        printMessage("Port bind error", true);
        return -1;
    }

    listen(serverSocket, 1);
    char message[64] = "";
    sprintf(message, "Server running @ %s", getConfig("PORT").c_str());
    printMessage(message, true);
    socklen_t monitorSize = sizeof(monitor);
    int monitorSocket = accept(serverSocket, (struct sockaddr*)&monitor, &monitorSize);

    //copying monitor info to data structure
    memcpy(&mon_info.monitor, &monitor, sizeof(struct sockaddr_in));
    mon_info.socket = monitorSocket;
    printMessage("Host Control connected.", true);

    // start listener
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, commandHandler, NULL);
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