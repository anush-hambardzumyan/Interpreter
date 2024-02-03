#pragma once
#include "parser.hpp"

void print1()
{
    for (const auto& entry : intmap) 
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
}

void print3()
{
    for (const auto& element : intarrmap) 
    {
        std::cout << "String: " << element.first << ", List of ints: ";
        for (const auto& value : element.second) {
            std::cout << value << " ";
        }
    }       
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
    //print1();
    //print2();
    //print3();
}    
