#pragma once
#include <iostream>
#include <stack>
#include <map>
#include <utility>
#include <algorithm>
//#include "types.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"
#include "parser.hpp"

Error error;
std::stack<std::string> parentheses;
//std::string type_check(std::string predicted_type);

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
