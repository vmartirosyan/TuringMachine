#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iostream>

#ifdef _DEBUG_
#define Debug(msg) { std::cerr << msg << std::endl; }
#else
#define Debug(msg)
#endif

enum class Direction
{
    Unknown,
    Right,
    Left 
};

enum class CommandType
{
    Unknown,
    If, 
    While
};

struct Command
{
    CommandType type;
    char input;
    Direction dir;
    char output;
};

class Compiler
{
public:
    Compiler(std::string inFile);
    bool Compile(std::string outFile);

private:
    bool Parse(std::string inFile, std::string& error);
    
    bool WriteConfig(std::string outFile, std::string& error);

    std::string getNextState(Command cmd, Direction curDir, int count);

    bool isIndented(std::string);
    CommandType isCommand(std::string);

    std::vector<Command> program;

    
};
