#pragma once
#include <iostream>
#include "int_handling.hpp"
#include "bool_handling.hpp"
#include "float_handling.hpp"
#include "double_handling.hpp"
#include "char_handling.hpp"
#include "string_handling.hpp"
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"

Error error_cin;
void input(std::vector<std::string> cur_line, int line_number);
void scan_var(std::vector<std::string> cur_line, std::string var_type ,int line_number);
void scan_array(std::vector<std::string> cur_line, int line_number);

void input(std::vector<std::string> cur_line, int line_number)
{
    if(cur_line[1] != ">>")
    {
        error_cin.invalid_op(line_number);
        exit(-1);
    }

    if(cur_line.size() != 4) //std::cin >> a ;
    {
        error_cin.invalid_op(line_number);
        exit(-1);
    }

    auto it = std::find(allkeywords.begin(),allkeywords.end() , cur_line[2]);
    if(it != allkeywords.end()) //std::cout << break ;
    {
        error_cin.is_keyword(cur_line[2]);
        exit(-1);
    }

    std::string input_type;
    for(int i = 0; i < allvars.size() ; ++i)
    {
        if(cur_line[2] == allvars[i].second)
        {
            input_type = allvars[i].first;
        }
    }

    //check if the input is array name  std::cin >> arr[3] ;
    if(input_type == "")
    {
        error_cin.was_not_dec(cur_line[2]);
        exit(-1);
    }

    else
    {
        scan_var(cur_line,input_type,line_number);
    }

}

void scan_var(std::vector<std::string> cur_line, std::string var_type ,int line_number)
{
    if(var_type == "Int")
    {
        int var;
        std::cin >> var;
        auto it = intmap.find(cur_line[2]);
        it -> second = var;
    }

    if(var_type == "Double")
    {
        double var;
        std::cin >> var;
        auto it = doublemap.find(cur_line[2]);
        it -> second = var;
    }

    if(var_type == "Float")
    {
        float var;
        std::cin >> var;
        auto it = floatmap.find(cur_line[2]);
        it -> second = var;
    }

    if(var_type == "Char")
    {
        int var;
        std::cin >> var;
        auto it = charmap.find(cur_line[2]);
        it -> second = static_cast<char>(var);
    }

    if(var_type == "String")
    {
        std::string var;
        std::cin >> var;
        auto it = stringmap.find(cur_line[2]);
        it -> second = var;
    }
}