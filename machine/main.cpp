#include "turing_machine.h"
#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        TuringMachine tm(argc > 1 ? argv[1] : "config1");
    
        std::cerr << "Output:" << tm.Run("1111*111") << std::endl;
    }
    catch(std::runtime_error e)
    {
        std::cerr << "Error running Turing machine: " << e.what() << '\n';
        return -1;
    }
    
    return 0;
}