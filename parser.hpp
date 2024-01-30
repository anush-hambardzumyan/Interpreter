#pragma once
#include <iostream>
#include <stack>
#include <map>
#include <utility>
#include <algorithm>
#include "types.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"
#include "int_handling.hpp"

std::vector<std::string> types = {"Bool","Char","Int","Float","Double","String"};

std::string type_check(std::string predicted_type);
void type_cordinator(std::string predicted_type,std::vector<std::string> cur_line , int line_number);


void parser(std::vector<std::string> cur_line,int line_number)
{
    if(type_check(cur_line[0]) != "")
    {
        type_cordinator(cur_line[0] , cur_line , line_number);
    }
}

std::string type_check(std::string predicted_type)
{
    auto it = std::find(types.begin() , types.end() , predicted_type);
    if(it != types.end())
    {
        return predicted_type;
    }
    return "";
}

void type_cordinator(std::string predicted_type,std::vector<std::string> cur_line , int line_number)
{
    if(predicted_type == "Int")
    {
        int_var_dec(cur_line , line_number);
    }

    if(predicted_type == "Bool")
    {
        //.....
    }
}


