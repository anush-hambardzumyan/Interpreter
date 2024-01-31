#pragma once
#include <utility>
#include <algorithm>
#include "parser.hpp"
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"

Error error_string;
void string_casts_init(std::vector<std::string> cur_line,int line_number ,std::string var_type);

void string_var_dec(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_string.missing_semicolon(line_number);
        exit(-1);
    }   

    if(cur_line[2] != "=")
    {
        error_string.invalid_op(line_number);
        exit(-1);
    }   

    for(int i = 0; i < allkeywords.size(); ++i)
    {
        if(cur_line[1] == allkeywords[i])
        {
            error_string.is_keyword(cur_line[1]);
            exit(-1);
        }
    }

    if(cur_line.size() != 5)
    {
        error_char.invalid_assignment(line_number);
        exit(-1);
    }

    else
    {
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[1] == allvars[i].second)
            {
                error_char.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        std::string var_type = "";
        for(int i = 0; i < allvars.size(); ++i)
        {
            if(cur_line[3] == allvars[i].second )
            {
                if(allvars[i].first != "String")
                {
                    error_string.type_incompatiblity(line_number);
                    exit(-1);
                }
                var_type = "String";
                string_casts_init(cur_line,line_number,var_type);
            }
        }

        if(var_type == "")
        {
            for(int i = 0; i < allkeywords.size(); ++i) 
            {
                if(allkeywords[i] == cur_line[3])
                {
                    error_string.is_keyword(cur_line[3]);
                    exit(-1);
                }
            }

            std::string last_string = cur_line[cur_line.size() - 2];
            if(last_string[0] == '\"' && last_string[last_string.size() - 1] == '\"')
            {
                stringmap[cur_line[1]] = last_string;
                allvars.push_back(std::make_pair("String", cur_line[1])); 
                return;
            }

            for(int i = 0; i < allvars.size(); ++i)
            {
                if(allvars[i].first == "String" && allvars[i].second == cur_line[3])
                {
                    string_casts_init(cur_line, line_number, "String");
                    allvars.push_back(std::make_pair("String" , cur_line[1]));
                    return;
                } 
            }
        }
    }
}

void string_casts_init(std::vector<std::string> cur_line , int line_number , std::string var_type)
{
    if(var_type == "")
    {
        return ;
    }

    if(var_type == "String")
    {
        auto it = stringmap.find(cur_line[3]);
        stringmap[cur_line[1]] = it -> second;
        allvars.push_back(std::make_pair("String" , cur_line[1]));
        return;
    }

    if(var_type != "String")
    {
        error_string.type_incompatiblity(line_number);   
        exit(-1);
    }
}