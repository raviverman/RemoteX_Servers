#include "def.h"

extern bool isVerbose;


void printMessage(char* message, bool onVerbose)
{
    if(isVerbose || onVerbose)
    {
        std::cout<<message<<std::endl;
    }
}