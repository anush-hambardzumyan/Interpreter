#pragma once
#include <utility>
#include <algorithm>
#include "parser.hpp"
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"

Error error_double;
void double_casts_init(std::vector<std::string> cur_line,int line_number ,std::string var_type);

void double_var_dec(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_double.missing_semicolon(line_number);
        exit(-1);
    }   

    if(cur_line[2] != "=")
    {
        error_double.invalid_op(line_number);
        exit(-1);
    }   

    for(int i = 0; i < allkeywords.size(); ++i)
    {
        if(allkeywords[i] == cur_line[1])
        {
            error_double.is_keyword(cur_line[1]);
            exit(-1);
        }
    }

    if(cur_line.size() == 3)
    {
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[1] == allvars[i].second)
            {
                error_double.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        doublemap.insert(std::make_pair(cur_line[1],0));
        allvars.push_back(std::make_pair("Double" , cur_line[1] ));
    }

    else if(cur_line.size() == 5)
    {
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(allvars[i].second == cur_line[1])
            {
                error_double.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        std::string var_type = "";

        for(int i = 0; i < allvars.size(); ++i)
        {
            if(cur_line[3] == allvars[i].second)
            {
                var_type = allvars[i].first;
                double_casts_init(cur_line,line_number,var_type);
            }
        }

        if(var_type == "")        //if the right operand was not found it means that the right operand is literal or undefined variable or keyword
        {
            for(int i = 0; i < allkeywords.size(); ++i)
            {
                if(allkeywords[i] == cur_line[3])
                {
                    error_double.is_keyword(cur_line[3]);
                    exit(-1);
                }
            }

            auto it = doublemap.find(cur_line[3]);   //Float A = 10 ;
            if(it == doublemap.end())                
            {
                try                 
                {                                           //Float A = 10 ;
                    double value = std::stod(cur_line[3]);     //if it's literal ,insert in map
                    doublemap[cur_line[1]] = value;    
                    allvars.push_back(std::make_pair("Double" , cur_line[1]));                  
                    return;
                } 

                catch (const std::invalid_argument& e) 
                {
                    error_double.was_not_dec(cur_line[3]);
                    exit(-1);
                }
            }
        }
    }

    else
    {
        error_double.invalid_assignment(line_number);
    }
}

void double_casts_init(std::vector<std::string> cur_line , int line_number , std::string var_type)
{
    if(var_type == "")
    {
        return ;
    }

    if(var_type == "String")            //trying to assign string to double
    {
        error_float.type_incompatiblity(line_number);   //double cant be assigned to float
        exit(-1);
    }

    else
    {
        if(var_type == "Int")
        {
            auto it = intmap.find(cur_line[3]);
            doublemap[cur_line[1]] = it -> second;
            allvars.push_back(std::make_pair("Double" , cur_line[1]));
            return;
        }

        if(var_type == "Double")
        {
            auto it = doublemap.find(cur_line[3]);
            doublemap[cur_line[1]] = it -> second;
            allvars.push_back(std::make_pair("Double" , cur_line[1]));
            return;
        }

        else if(var_type == "Float")
        {
            auto it = floatmap.find(cur_line[3]);
            doublemap[cur_line[1]] = it -> second;
            allvars.push_back(std::make_pair("Double" , cur_line[1]));
            return;
        }

        else if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[3]);
            doublemap[cur_line[1]] = static_cast<int> (it -> second);
            allvars.push_back(std::make_pair("Double" , cur_line[1]));
            return;
        }

        else if (var_type == "Bool")
        {
            auto it = boolmap.find(cur_line[3]);
            doublemap[cur_line[1]] = static_cast<int>(it->second);
            allvars.push_back(std::make_pair("Double" , cur_line[1]));
            return;
        }
    }
    return;
}