
#include "def.h"

bool isVerbose = false;


using namespace std;

int main(int argc, char const *argv[])
{
    loadConfig();
    startServer();
    while(1);
    return 0;
}
