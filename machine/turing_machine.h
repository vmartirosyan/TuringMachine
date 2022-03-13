#pragma once
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
    Right,
    Left 
};

class TuringMachine
{
public:
    TuringMachine(std::string configFile);
    std::string Run(std::string);
private:
    bool Parse(std::string configFile, std::string& error );
    std::vector<char> A; // Input alphabet
    std::vector<std::string> Q; // Internal states

    std::map<std::pair<char, std::string>, char> lambda; // Next internal state
    std::map<std::pair<char, std::string>, std::string> delta; // Output symbol
    std::map<std::pair<char, std::string>, Direction> nu; // movement direction
    

};
