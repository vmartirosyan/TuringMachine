#include <iostream>
#include "compiler.h"

int main(int argc, char** argv)
{
    try
    {
        Compiler c("code.tm");

        c.Compile("config1");
    }
    catch (std::runtime_error e)
    {
        std::cerr << "Error while compiling. " << e.what() << std::endl;
        return 1;
    }
    return 0;
}