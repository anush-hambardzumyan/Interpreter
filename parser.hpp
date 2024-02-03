#pragma once
#include <iostream>
#include <utility>
#include <algorithm>
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"

Error error_pars;
std::string type_check(std::string predicted_type);
std::string var_check(std::string predicted_var);
void type_cordinator(std::string predicted_type,std::vector<std::string> cur_line , int line_number);
void var_cordinator(std::string predicted_var,std::vector<std::string> cur_line , int line_number);
void prefix_inc_dec(std::vector<std::string> cur_line, int line_number);

#include "output.hpp"
#include "input.hpp"

void parser(std::vector<std::string> cur_line,int line_number)
{
    if(type_check(cur_line[0]) != "")       //check if the first token of line is type
    {
        type_cordinator(cur_line[0] , cur_line , line_number);
    }

    if(var_check(cur_line[0]) != "")      //check if the first token of line is variable
    {
        var_cordinator( var_check(cur_line[0]) , cur_line , line_number);
    }

    if(cur_line[0] == "++" || cur_line[0] == "--")  //check for prefix increment and decrement
    {
        prefix_inc_dec(cur_line , line_number);
    }

    if(cur_line[0] == "std::cout" || cur_line[0] == "std::cin")
    {
        if(iostream_usage != true)
        {
            error_pars.was_not_dec(cur_line[0]);
        }

        else if(cur_line[0] == "std::cout")
        {
            output(cur_line,line_number);
        }    

        else if(cur_line[0] == "std::cin")
        {
            input(cur_line,line_number);
        }
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

std::string var_check(std::string predicted_var)
{
    for(int i = 0; i < allvars.size() ; ++i)
    {
        if(predicted_var == allvars[i].second)
        {
            return allvars[i].first;
        }
    }
    return "";
}

#include "int_handling.hpp"
#include "bool_handling.hpp"
#include "float_handling.hpp"
#include "double_handling.hpp"
#include "char_handling.hpp"
#include "string_handling.hpp"

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

    if(predicted_type == "Double")
    {
        double_var_dec(cur_line , line_number);
    }

    if(predicted_type == "Char")
    {
        char_var_dec(cur_line , line_number);
    }

    if(predicted_type == "String")
    {
        string_var_dec(cur_line , line_number);
    }
}

void var_cordinator(std::string predicted_var,std::vector<std::string> cur_line , int line_number)
{
    if(predicted_var == "Int")
    {
        int_var_assign(cur_line , line_number);
    }

    if(predicted_var == "Float")
    {
        float_var_assign(cur_line , line_number);
    }

    if(predicted_var == "Double")
    {
        double_var_assign(cur_line , line_number);
    }

    if(predicted_var == "Bool")
    {
        bool_var_assign(cur_line , line_number);
    }

    if(predicted_var == "Char")
    {
        char_var_assign(cur_line , line_number);
    }

    if(predicted_var == "String")
    {
        string_var_assign(cur_line , line_number);
    }
}

void prefix_inc_dec(std::vector<std::string> cur_line, int line_number)
{
    if(cur_line.size() != 3 || var_check(cur_line[1]) == "")
    {
        error_pars.invalid_op(line_number);
        exit(-1);
    }

    if(cur_line[2] != ";")
    {
        error_pars.missing_semicolon(line_number);
        exit(-1);
    }

    std::string var_type = var_check(cur_line[1]);
    if(var_type == "" || var_type == "String")
    {
        error_pars.invalid_op(line_number);
        exit(-1);
    }

    std::swap(cur_line[0] , cur_line[1]);
    if(var_type == "Int")
    {
        if(cur_line[1] == "++") // a ++ ;
        {
            intmap[cur_line[0]] += 1;
        }

        else if(cur_line[1] == "--") //a --;
        {
            intmap[cur_line[0]] -= 1;
        }
    }

    if(var_type == "Double")
    {
        if(cur_line[1] == "++") // a ++ ;
        {
            doublemap[cur_line[0]] += 1;
        }

        else if(cur_line[1] == "--") //a --;
        {
            doublemap[cur_line[0]] -= 1;
        }
    }

    if(var_type == "Float")
    {
        if(cur_line[1] == "++") // a ++ ;
        {
            floatmap[cur_line[0]] += 1;
        }

        else if(cur_line[1] == "--") //a --;
        {
            floatmap[cur_line[0]] -= 1;
        }
    }

    if(var_type == "Bool")
    {
        if(cur_line[1] == "++") // a ++ ;
        {
            boolmap[cur_line[0]] += 1;
        }

        else if(cur_line[1] == "--") //a --;
        {
            boolmap[cur_line[0]] -= 1;
        }
    }

    if(var_type == "Char")
    {
        if(cur_line[1] == "++") // a ++ ;
        {
            compound_add_char(cur_line,1,line_number);
        }

        else if(cur_line[1] == "--") //a --;
        {
            compound_sub_char(cur_line,1,line_number);
        }
    }
}