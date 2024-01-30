#pragma once
#include "parser.hpp"

Error error;

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
}    
