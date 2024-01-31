#pragma once
#include <utility>
#include <algorithm>
#include "parser.hpp"
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"

Error error_char;
void char_casts_init(std::vector<std::string> cur_line,int line_number ,std::string var_type);

void char_var_dec(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_char.missing_semicolon(line_number);
        exit(-1);
    }   

    if(cur_line[2] != "=")
    {
        error_char.invalid_op(line_number);
        exit(-1);
    }   

    for(int i = 0; i < allkeywords.size(); ++i)
    {
        if(allkeywords[i] == cur_line[1])
        {
            error_char.is_keyword(cur_line[1]);
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
            if(allvars[i].second == cur_line[1])
            {
                error_char.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        std::string var_type = "";

        for(int i = 0; i < allvars.size(); ++i)
        {
            if(cur_line[3] == allvars[i].second)
            {
                var_type = allvars[i].first;
                char_casts_init(cur_line,line_number,var_type);
            }
        }

        if(var_type == "")        //if the right operand was not found it means that the right operand is literal or undefined variable or keyword
        {
            for(int i = 0; i < allkeywords.size(); ++i)  //maybe keyword  Char c = break ;
            {
                if(allkeywords[i] == cur_line[3])
                {
                    error_char.is_keyword(cur_line[3]);
                    exit(-1);
                }
            }

            //Char c = 'c' ;
            if((cur_line[cur_line.size() - 2].size() == 3) || ((cur_line[cur_line.size() - 2][0] == '\'') || (cur_line[cur_line.size() - 2][2] == '\'')))
            {
                charmap[cur_line[1]] = cur_line[cur_line.size() - 2][1];
                allvars.push_back(std::make_pair("Char", cur_line[1])); 
                return;
            }

            else  //int to char
            {
                auto it = charmap.find(cur_line[3]);  
                if (it == charmap.end())
                {
                    try
                    {
                        int value = std::stoi(cur_line[3]);
                        if (value < 0 || value > 127)
                        {
                            error_char.type_incompatiblity(line_number); // Error if the integer is out of the valid range [0, 127]
                            exit(-1);
                        }
                        else
                        {
                            charmap[cur_line[1]] = static_cast<char>(value);
                            allvars.push_back(std::make_pair("Char", cur_line[1])); // Push back the new variable only if the assignment is valid
                            return;
                        }
                    }
                    catch (const std::invalid_argument &e)
                    {
                        error_char.was_not_dec(cur_line[3]);
                        exit(-1);
                    }
                }

                else
                {
                    for(int i = 0; i < allvars.size(); ++i)
                    {
                        if(allvars[i].first == "Char" && allvars[i].second == cur_line[3])
                        {
                            char_casts_init(cur_line, line_number, "Char");
                            allvars.push_back(std::make_pair("Char" , cur_line[1]));
                            return;
                        } 
                    }
                }
            }    
        }
    }
}

void char_casts_init(std::vector<std::string> cur_line , int line_number , std::string var_type)
{
    if(var_type == "")
    {
        return ;
    }

    if(var_type == "String" || var_type == "Double" || var_type == "Float" || var_type == "Bool" )       //ony int can be assigned to char
    {
        error_char.type_incompatiblity(line_number);   
        exit(-1);
    }

    else if(var_type == "Char")
    {
        auto it = charmap.find(cur_line[3]);
        charmap[cur_line[1]] = (it -> second);
        allvars.push_back(std::make_pair("Char" , cur_line[1]));
        return;
    }

    else if (var_type == "Int")
    {
        auto it = intmap.find(cur_line[3]);
        if(it -> second < 0 || it -> second > 127)
        {
            error_char.type_incompatiblity(line_number);   
            exit(-1);
        }
        charmap[cur_line[1]] = static_cast<char>(it -> second);
        allvars.push_back(std::make_pair("Char" , cur_line[1]));
        return;
    }
}