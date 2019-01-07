
#include "def.h"

bool isVerbose = false;


using namespace std;

int main(int argc, char const *argv[])
{
    
    char s[]="echo helow";
    printf("Something\n");  
    executeCommandAsync(s);
    executeCommandAsync(s);
    
    return 0;
}
