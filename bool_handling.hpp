#pragma once
#include <iostream>
#include <map>
#include <utility>
#include <algorithm>
#include "parser.hpp"
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"

Error error_bool;
void bool_casts_init(std::vector<std::string> cur_line, int line_number,std::string var_type);

void bool_var_dec(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_bool.missing_semicolon(line_number);
        exit(-1);
    }   

    if(cur_line[2] != "=")
    {
        error_bool.invalid_op(line_number);
        exit(-1);
    }  

    for(int i = 0; i < allkeywords.size(); ++i)
    {
        if(cur_line[1] == allkeywords[i])
        {
            error_bool.is_keyword(cur_line[1]);
            exit(-1);
        }
    }

    if(cur_line.size() == 3)
    {
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[1] == allvars[i].second)
            {
                error_bool.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        boolmap.insert(std::make_pair(cur_line[1],0));  
        allvars.push_back(std::make_pair("Bool" , cur_line[1]));
    }

    else if(cur_line.size() == 5)
    {
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[1] == allvars[i].second)
            {
                error_bool.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        if(cur_line[3] == "true")
        {
            cur_line[3] = "1";
        } 

        if(cur_line[3] == "false")
        {
            cur_line[3] = "0";
        }

        std::string var_type = "";

        for(int i = 0; i < allvars.size(); ++i)
        {
            if(cur_line[3] == allvars[i].second)
            {
                var_type = allvars[i].first;
                bool_casts_init(cur_line,line_number,var_type);
            }
        }

        if(var_type == "")        //if the right operand was not found it means that the right operand is literal or undefined variable or keyword
        {
            for(int i = 0; i < allkeywords.size(); ++i)
            {
                if(allkeywords[i] == cur_line[3])
                {
                    error_bool.is_keyword(cur_line[3]);
                    exit(-1);
                }
            }

            auto it = boolmap.find(cur_line[3]);   //Bool A = 1 ;
            if(it == boolmap.end())                
            {
                try                 
                {                                                               //Bool A = 10 ;
                    bool value = static_cast<bool> (std::stoi(cur_line[3]));     //if it's literal ,insert in map
                    boolmap[cur_line[1]] = value;    
                    allvars.push_back(std::make_pair("Bool" , cur_line[1]));                  
                    return;
                } 

                catch (const std::invalid_argument& e) 
                {
                    error_bool.was_not_dec(cur_line[3]);
                    exit(-1);
                }
            }
        }
        
    }

    else
    {
        error_bool.invalid_assignment(line_number);
    }
}

void bool_casts_init(std::vector<std::string> cur_line , int line_number , std::string var_type)
{

    if(var_type == "")
    {
        return ;
    }

    if(var_type == "String")            //trying to assign string to bool
    {
        error_bool.type_incompatiblity(line_number);
        exit(-1);
    }

    else
    {
        if(var_type == "Double")
        {
            auto it = doublemap.find(cur_line[3]);
            boolmap[cur_line[1]] = bool(it -> second);
            allvars.push_back(std::make_pair("Bool" , cur_line[1]));
            return;
        }

        else if(var_type == "Float")
        {
            auto it = floatmap.find(cur_line[3]);
            boolmap[cur_line[1]] = bool(it -> second); 
            allvars.push_back(std::make_pair("Bool" , cur_line[1]));
            return;
        }

        else if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[3]);
            boolmap[cur_line[1]] = 1;               //any char assigning to bool makes it true
            allvars.push_back(std::make_pair("Bool" , cur_line[1]));
            return;
        }

        else if(var_type == "Int")
        {
            auto it = intmap.find(cur_line[3]);
            boolmap[cur_line[1]] = bool(it -> second);
            allvars.push_back(std::make_pair("Bool" , cur_line[1]));
            return;
        }

        else if(var_type == "Bool")
        {
            auto it = boolmap.find(cur_line[3]);
            boolmap[cur_line[1]] = bool(it -> second);
            allvars.push_back(std::make_pair("Bool" , cur_line[1]));
            return;
        }
    }
}