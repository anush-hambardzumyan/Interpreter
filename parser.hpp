#pragma once
#include <iostream>
#include <stack>
#include <map>
#include <utility>
#include <algorithm>
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"
//tipery taky haytarari


std::string type_check(std::string predicted_type);
void type_cordinator(std::string predicted_type,std::vector<std::string> cur_line , int line_number);
void analyze_maps(std::vector<std::string>& cur_line , std::string type , std::string name , int line_number);

// void print()
// {
//     for (const auto& entry : intmap) 
//     {
//         std::cout << "Key: " << entry.first << ", Value: " << entry.second << std::endl;
//     }
// }


void parser(std::vector<std::string> cur_line,int line_number)
{
    if(type_check(cur_line[0]) != "")       //check if the first token of line is type
    {
        type_cordinator(cur_line[0] , cur_line , line_number);
    }
    //print();
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

void analyze_maps(std::vector<std::string>& cur_line , std::string type , std::string name , int line_number)
{
    if(type == "String")
    {
        auto it = stringmap.find(name);
        if(it != stringmap.end())
        {
            cur_line[3] = it -> second;
        }
    }

    if(type == "Bool")
    {
        auto it = boolmap.find(name);
        if(it != boolmap.end())
        {
            cur_line[3] = it -> second;
        }
    }

    if(type == "Double")
    {
        auto it = doublemap.find(name);
        if(it != doublemap.end())
        {
            cur_line[3] = it -> second;
        }
    }

    if(type == "Float")
    {
        auto it = floatmap.find(name);
        if(it != floatmap.end())
        {
            cur_line[3] = it -> second;
        }
    }

    if(type == "Char")
    {
        auto it = charmap.find(name);
        if(it != charmap.end())
        {
            cur_line[3] = it -> second;
        }
    }
}

#include "int_handling.hpp"
#include "bool_handling.hpp"
#include "float_handling.hpp"


void type_cordinator(std::string predicted_type,std::vector<std::string> cur_line , int line_number)
{
    if(predicted_type == "Int")
    {
        int_var_dec(cur_line , line_number);
    }

    if(predicted_type == "Bool")
    {
        bool_var_dec(cur_line , line_number);
    }

    if(predicted_type == "Float")
    {
        float_var_dec(cur_line , line_number);
    }
}
