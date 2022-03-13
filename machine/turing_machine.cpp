#include "turing_machine.h"
#include <exception>
#include <fstream>
#include <sstream>
#include <algorithm>

TuringMachine::TuringMachine(std::string configFile)
{
    std::string error;
    if (!Parse(configFile, error))
    {
        throw std::runtime_error(error);
    }
}

bool TuringMachine::Parse(std::string configFile, std::string& error )
{
    // Put "X" as a possible tape symbol in order to identify exceptional cases.
    A.push_back('X'); 
    // Put "H" as the ending state
    Q.push_back("H"); 
    
    std::ifstream inf(configFile);
    char buf[250];
    std::string token;
    // Parse the input alphabet
    {
        inf.getline(buf, 250);
        std::stringstream str(buf);
        while (str >> token)
        {
            if (token[token.size() - 1] == ',')
                token = token.substr(0, token.size() - 1);
            if (token.size() > 1)
            {
                error = "Input alphabet symbol error: " + token;
                return false;
            }
            A.push_back(token[0]);
            
        }
    }

    // Parse the internal states    
    {
        inf.getline(buf, 250);
        std::stringstream str(buf);
        while (str >> token)
        {
            if (token[token.size() - 1] == ',')
                token = token.substr(0, token.size() - 1);
            Q.push_back(token);
            
        }
    }

    buf[0] = 0;
    inf.getline(buf, 250);
    inf.getline(buf, 250);
    buf[0] = 0;
    

    // Parse the functions    
    {
        // Skip "H"
        for (auto i = Q.begin()+1; i < Q.end(); ++i)
        {
            inf.getline(buf, 250);
            std::stringstream str(buf);
                
            // Skip "X"
            for (auto j = A.begin()+1; j != A.end(); ++j )
            {
                std::string token2;
                std::string token3;

                auto key = std::pair<char, std::string>{*j,*i};
                
                str >> token;
            
                if (token[token.size() - 1] == ',')
                    token = token.substr(0, token.size() - 1);
                if (token.size() > 1 || std::find(A.begin(), A.end(), token[0]) == A.end())
                {
                    error = "Wrong tape symbol: " + token;
                    return false;
                }

                // Add halt state
                delta[std::pair<char, std::string>{*j,"H"}] = "H";
                lambda[std::pair<char, std::string>{*j,"H"}] = 'X';


                // In exceptional cases nothing more to parse
                if (token[0] == 'X')
                {
                    
                    //std::cerr << "Key: (" << *j << "," << *i << ")" << std::endl;
                
                    lambda[key] = token[0];
                    delta[key] = "H";
                    nu[key] = Direction::Right;
                    
                    str >> token;
                    continue;
                }

                str >> token2;

                if (token2[token2.size() - 1] == ',')
                    token2 = token2.substr(0, token2.size() - 1);
                if (std::find(Q.begin(), Q.end(), token2) == Q.end())
                {
                    error = "Wrong state: " + token2;
                    return false;
                }

                str >> token3;
                if (token3.size() > 1 || (token3 != ">" && token3 != "<"))
                {
                    error = "Wrong direction: " + token3;
                    return false;
                }


                // TODO: Validate tokens
                //std::cerr << "Key: (" << *j << "," << *i << ")" << std::endl;
                
                lambda[key] = token[0];
                delta[key] = token2;
                nu[key] = token3 == ">" ? Direction::Right : Direction::Left ;

                str >> token;
            }   
            
        }
    }

    error = "";
    return true;
}

std::string TuringMachine::Run(std::string tape)
{
    // Append empty symbols to both sides of the tape
    tape = "E" + tape + "E";
    auto pos = tape.begin() + 1;
    // Skip the first state as it is the halt state
    std::string state = Q[1];
    while (true)
    {
        
        std::string tmp;
        tmp += *pos;

        auto key = std::pair<char, std::string>{*pos,state};

        if (lambda.find(key) == lambda.end())
        {
            throw std::runtime_error("Unknown pair <" + tmp + (std::string)"," + state + ">"  );
        }

        if (lambda[key] == 'X')
        {
            throw std::runtime_error("Exception thrown while running Turing machine at state '" + state + "', with input '" + tmp + "'");
        }

        *pos = lambda[key];
        

        Debug("Processing : <" + tmp + (std::string)"," + state + ">" );
        Debug("lambda(" + tmp + (std::string)"," + state + ")=" + lambda[key] + "\t\tdelta(" + tmp + (std::string)"," + state + ")=" + delta[key]);
        Debug("Tape: " + tape);

        std::string cursor = "      ";
        auto len =  pos - tape.begin();
        for (int i = 0; i < len; ++i)
            cursor += " ";
        cursor += "^";
        Debug(cursor);

        auto dir = nu[key];
        if (dir == Direction::Right)
            pos++;
        else
            pos--;
        state = delta[key];

        if (state == "H")
        {
            break;
        }
    }

    return tape;
}