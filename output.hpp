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

Error error_cout;
void output(std::vector<std::string> cur_line, int line_number);
void print_var(std::vector<std::string> cur_line, std::string var_type ,int line_number);
void print_literal(std::vector<std::string> cur_line, int line_number);
void print_array(std::vector<std::string> cur_line, int line_number);

void output(std::vector<std::string> cur_line, int line_number)
{
    if(cur_line[1] != "<<")
    {
        error_cout.invalid_op(line_number);
        exit(-1);
    }

    if(cur_line.size() == 5)
    {
        if(cur_line[2][0] == '\"' && cur_line[3][0] == '\"')
        {
            std::cout << " ";
            return;
        }
        
        else
        {
            error_cout.invalid_op(line_number);
            exit(-1);
        }
    }

    if(cur_line.size() != 4) //std::cout << a ;
    {
        error_cout.invalid_op(line_number);
        exit(-1);
    }

    if(cur_line[2] == "std::endl")
    {
        std::cout << std::endl;
        return;
    }

    auto it = std::find(allkeywords.begin(),allkeywords.end() , cur_line[2]);
    if(it != allkeywords.end()) //std::cout << break ;
    {
        error_cout.is_keyword(cur_line[2]);
        exit(-1);
    }

    if(var_check(cur_line[2]) != "")
    {
        print_var(cur_line,var_check(cur_line[2]) ,line_number);
    }

    //else if for printing arrays

    else 
    {
        print_literal(cur_line , line_number);
    }
}

void print_var(std::vector<std::string> cur_line, std::string var_type ,int line_number)
{
    if(var_type == "Int")
    {
        auto it = intmap.find(cur_line[2]);
        std::cout << it -> second ;
    }

    if(var_type == "Double")
    {
        auto it = doublemap.find(cur_line[2]);
        std::cout << it -> second ;
    }

    if(var_type == "Float")
    {
        auto it = floatmap.find(cur_line[2]);
        std::cout << it -> second ;
    }

    if(var_type == "Bool")
    {
        auto it = boolmap.find(cur_line[2]);
        std::cout << it -> second ;
    }

    if(var_type == "Char")
    {
        auto it = charmap.find(cur_line[2]);
        std::cout << it -> second ;
    }

    if(var_type == "String")
    {
        auto it = stringmap.find(cur_line[2]);
        std::cout << it -> second ;
    }
}

void print_literal(std::vector<std::string> cur_line, int line_number)
{
    std::string literal = cur_line[2];
    if(literal[0] == '\"' && literal[literal.size() - 1] == '\"')
    {
        literal.erase(literal.begin());
        literal.erase(literal.end() - 1);
        std::cout << literal;
        return;
    }

    if(literal.size() == 3 && literal[0] == '\'' && literal[2] == '\'')
    {
        std::cout << literal[1];
        return;
    }

    else
    {   
        try
        {
            std::cout << std::stod(literal);
        }

        catch(const std::exception& e)
        {
            error_cout.was_not_dec(literal);
            exit(-1);
        }
    }
}