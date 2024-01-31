#pragma once
#include "parser.hpp"

Error error;

// void print()
// {
//     for (const auto& entry : boolmap) 
//     {
//         std::cout << "Key: " << entry.first << ", Value: " << entry.second << std::endl;
//     }
// }

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
    //print();
}    
