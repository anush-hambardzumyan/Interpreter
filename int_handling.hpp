#pragma once
#include <iostream>
#include <stack>
#include <map>
#include <utility>
#include <algorithm>
//#include "types.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"

Error error_int;

std::map<std::string,int> intmap;
void int_var_dec(std::vector<std::string> cur_line, int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_int.missing_semicolon(line_number);
        return;
    }
    
    else if(cur_line.size() == 3)
    {
        auto it = intmap.find(cur_line[1]);             //Int a ;
        if(it == intmap.end())
        {
            intmap.insert(std::make_pair(cur_line[1],0));
        }

        else
        {
            error_int.redeclaration(line_number,cur_line[1]);
            return;
        }
    }

    else if(cur_line.size() == 5)
    {
        auto it = intmap.find(cur_line[1]);             //Int a ;
        if(it != intmap.end())
        {
            error_int.redeclaration(line_number,cur_line[1]);
            return;
        }

        else
        {
            auto it = intmap.find(cur_line[3]);   //Int A = 10
            if(it == intmap.end())
            {
                try                 
                {
                    int value = std::stoi(cur_line[3]);
                    intmap[cur_line[1]] = value;                        //update the value of already existing key
                    return;
                } 

                catch (const std::invalid_argument& e) 
                {
                    //error_int.type_incompatiblity(line_number);
                    error_int.was_not_dec(cur_line[3]);
                    exit(-1);
                }
            }
        }
    }

    // else if()
    // {
    //     dec_and_init_val(cur_line);
    //     dec_and_init_var(cur_line);
    // }
}

void dec_and_init_val(const std::vector<std::string>& cur_line)
{

}

void dec_and_init_var(const std::vector<std::string>& cur_line)
{

}