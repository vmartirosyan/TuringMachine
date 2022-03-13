#include "compiler.h"
#include <sstream>
#include <set>

Compiler::Compiler(std::string inFile)
{
    std::string error;
    if (!Parse(inFile, error))
    {
        throw std::runtime_error("Error parsing input file. " + error);
    }
}

bool Compiler::Parse(std::string inFile, std::string& error)
{
    std::ifstream in(inFile);

    CommandType cmdType = CommandType::Unknown;
    int lineNum = 0;
    
    Command* cmd = nullptr;
    
    while (!in.eof())
    {
        char buf[255];
        in.getline(buf, 255);
        lineNum++;


        std::string line = buf;

        Debug(line);

        std::stringstream str(buf);

        std::string token;

        str >> token;

        if ((cmdType = isCommand(token)) != CommandType::Unknown)
        {
            // Store the previous command if any
            if (cmd)
            {
                program.push_back(*cmd);
            }
            
            cmd = new Command;
            cmd->type = cmdType;

            str >> token;

            if (token.size() != 1)
            {
                error = "Condition must be set on one-char-length values only on line " + std::to_string(lineNum);
                return false;
            }

            cmd->input = token[0];
            
        } else if (isIndented(line) && cmd == nullptr)
        {
            error = "Indented command found on line " + std::to_string(lineNum);
            return false;
        } else if (isIndented(line) && cmd != nullptr)
        {
            std::string token2;
            
            str >> token2;
            if (token == "go")
            {
                cmd->dir = token2 == "right" ? Direction::Right : token2 == "left" ? Direction::Left : Direction::Unknown;
                if (cmd->dir == Direction::Unknown)
                {
                    error  = "Unknown direction " + token2 + " on line " + std::to_string(lineNum);
                    return false;
                }
            } else if (token == "write")
            {
                cmd->output = token2[0];
            }
            else
            {
                error = "Unknown token " + token + " on line " + std::to_string(lineNum);
                return false;
            }       
        }
        else
        {
            error  = "Unknown command '" + token + "' on line " + std::to_string(lineNum);
            return false;
        }
    }

    // Add the last command
    if (cmd)
    {
        program.push_back(*cmd);
    }

    return true;
}

bool Compiler::Compile(std::string outFile)
{
    std::string error;
    if (!WriteConfig(outFile, error))
    {
        throw std::runtime_error("Error parsing input file. " + error);
        return false;
    }

    return true;
}

bool Compiler::WriteConfig(std::string outFile, std::string& error)
{
    std::set<char> A; // Tape alphabet
    std::set<std::string> Q; // Internal states

    std::map<std::pair<char, std::string>, char> lambda; // Output symbol
    std::map<std::pair<char, std::string>, std::string> delta; // Next internal state
    std::map<std::pair<char, std::string>, Direction> nu; // movement direction

    std::string state = "q0";

    Direction curDir = Direction::Unknown;

    for (auto cmd  = program.begin(); cmd != program.end(); ++cmd)
    {
        if (cmd->output == 0)
            cmd->output = cmd->input;

        auto key = std::pair<char, std::string>{cmd->input, state};
        std::string nextState = getNextState(*cmd, curDir, Q.size());
        if (nextState != "")
            state = nextState;
        lambda[key] = cmd->output;
        delta[key] = state;
        nu[key] = cmd->dir;

        A.insert(cmd->input);
        Q.insert(state);

        curDir = cmd->dir;
    }

    std::ofstream fout(outFile);

    for (auto p = A.begin(); p != A.end(); ++p)
        fout << *p << ", ";

    // Remove the last comma
    long pos = fout.tellp(); 
    fout.seekp(pos - 2);

    fout << std::endl;

    
    for (auto p = Q.begin(); p != Q.end(); ++p)
        fout << *p << ", ";

    // Remove the last comma
    pos = fout.tellp(); 
    fout.seekp(pos - 2);

    fout << std::endl;
    fout << std::endl;
    fout << std::endl;

    for (auto q = Q.begin(); q != Q.end(); ++q)
    {
        for (auto a = A.begin(); a != A.end(); ++a)
        {
            auto key = std::pair<char, std::string>{*a,*q};

            if (lambda.find(key) == lambda.end())
            {
                fout << "\tX\t |";
            }
            else
            {
                fout << " " <<  lambda[key] << ", " << delta[key] << ", " << ( (nu[key] == Direction::Right) ? ">" : "<" ) << "\t |";
            }

            
        }

        // Remove the last pipe
            pos = fout.tellp(); 
            fout.seekp(pos - 1);
            
            fout << "\n";
    
    }
    


    return true;
}

std::string Compiler::getNextState(Command cmd, Direction curDir, int count)
{
    if ((cmd.input != cmd.output) || (cmd.dir != curDir))
        return "q" + std::to_string(count);
    
    return "";
}

bool Compiler::isIndented(std::string str)
{
    return (str[0] == ' ' || str[0] == '\t');
}

CommandType Compiler::isCommand(std::string str)
{
    return (str == "while" ? CommandType::While : str == "if" ? CommandType::If : CommandType::Unknown);
}