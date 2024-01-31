#pragma once
#include "parser.hpp"

Error error;

void print1()
{
    for (const auto& entry : doublemap) 
    {
        std::cout << "Key: " << entry.first << ", Value: " << entry.second << std::endl;
    }
}

void print2()
{
    for(int i = 0; i < allvars.size(); ++i)
    {
        std::cout << "Type: " << allvars[i].first << " " << "Name: " << allvars[i].second << std::endl;
    }
    //std::cout << allvars.size();
}

void interpreter(std::vector<std::vector<std::string>> TokStream)
{
    for(int lines = 0 ; lines < TokStream.size(); ++lines)
    {
        std::vector<std::string> cur_line = TokStream[lines];
        if(cur_line.size() == 0)
        {
            continue;
        }

        parser(cur_line,lines + 1);
    }
    print1();
    //print2();
    //std::cout << allvars.size();
}    
