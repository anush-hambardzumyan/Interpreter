#pragma once
#include <iostream>
#include <stack>
#include <map>
#include <utility>
#include <algorithm>
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"

Error error_int;
std::string int_casts_assignment(std::vector<std::string> cur_line, int line_number);

void int_var_dec(std::vector<std::string> cur_line,int line_number)
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
            allvars.push_back(std::make_pair("Int" , cur_line[1] ));
        }

        else
        {
            error_int.redeclaration(line_number,cur_line[1]);
            return;
        }
    }

    else if(cur_line.size() == 5)
    {
        auto it = intmap.find(cur_line[1]);             //Int a = 6 ;    Int a = 8 ;
        if(it != intmap.end())
        {
            error_int.redeclaration(line_number,cur_line[1]);
            return;
        }

        else
        {
            std::string var_type = int_casts_assignment(cur_line,line_number);

            if(var_type == "")        //if the right operand was not found it means that the right operand is literal or undefined variable
            {
                auto it = intmap.find(cur_line[3]);   //Int A = 10 ;
                if(it == intmap.end())                
                {
                    try                 
                    {                                           //Int A = 10 ;
                        int value = std::stoi(cur_line[3]);     //if it's literal ,insert in map
                        intmap[cur_line[1]] = value;                      
                        return;
                    } 

                    catch (const std::invalid_argument& e) 
                    {
                        error_int.was_not_dec(cur_line[3]);
                        exit(-1);
                    }
                }
            }
        }
    }
}

std::string int_casts_assignment(std::vector<std::string> cur_line , int line_number)
{
    std::string var_type = "";

    for(int i = 0; i < allvars.size(); ++i)
    {
        if(cur_line[3] == allvars[i].second)
        {
            var_type = allvars[i].first; 
        }
    }

    if(var_type == "")
    {
        return "";
    }

    if(var_type == "String")            //trying to assign string to int
    {
        error_int.type_incompatiblity(line_number);
        exit(-1);
    }

    else
    {
        if(var_type == "Double")
        {
            auto it = doublemap.find(cur_line[3]);
            intmap[cur_line[1]] = it -> second;
            return "Double";
        }

        else if(var_type == "Float")
        {
            auto it = floatmap.find(cur_line[3]);
            intmap[cur_line[1]] = it -> second;
            return "Float";
        }

        else if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[3]);
            intmap[cur_line[1]] = it -> second;
            return "Char";
        }

        else if(var_type == "Bool")
        {
            auto it = boolmap.find(cur_line[3]);
            intmap[cur_line[1]] = it -> second;
            return "Bool";
        }
    }
    return "";
}