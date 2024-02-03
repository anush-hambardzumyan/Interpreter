#pragma once
#include <utility>
#include <algorithm>
#include "parser.hpp"
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"

Error error_string;
void string_var_dec(std::vector<std::string> cur_line,int line_number);
void string_casts_init(std::vector<std::string> cur_line,int line_number ,std::string var_type);
void string_var_assign(std::vector<std::string> cur_line,int line_number);
void string_casts_assign(std::vector<std::string> cur_line, int line_number, std::string var_type);
void string_casts_comp_assign(std::vector<std::string> cur_line , int line_number , std::string var_type , std::string operation);
void add_string(std::vector<std::string> cur_line , std::string operand1,std::string operand2);
void operation_choice_string(std::vector<std::string> cur_line ,std::string operation, std::string operand1, std::string operand2 , int line_number);
void compound_add_string(std::vector<std::string>cur_line, std::string operand, int line_number);
std::string return_string(std::string var_type , std::string name , int line_number);


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
        error_string.invalid_assignment(line_number);
        exit(-1);
    }

    else
    {
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[1] == allvars[i].second)
            {
                error_string.redeclaration(line_number,cur_line[1]);
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
                last_string.erase(last_string.begin());
                last_string.erase(last_string.end() - 1);
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

void string_var_assign(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_string.missing_semicolon(line_number);
        exit(-1);
    }

    if(cur_line.size() == 3 ) 
    {
        error_string.invalid_op(line_number);
        exit(-1);
    }

    auto it1 = std::find(comparison_ops.begin() , comparison_ops.end(), cur_line[1]);
    if(it1 != comparison_ops.end())
    {
        return ; //a > b ;  expression should be optimized
    }

    auto it2 = std::find(arithmetical_ops_binary.begin() , arithmetical_ops_binary.end(), cur_line[1]);
    if(it2 != arithmetical_ops_binary.end())
    {
        return ; //a + b ;  expression should be optimized
    }

    if(cur_line.size() == 4) //it should be ordinary assignment or compound assignment
    {
        auto it = std::find(allkeywords.begin() , allkeywords.end() ,cur_line[2]);
        if(it != allkeywords.end())
        {
            error_string.is_keyword(cur_line[2]);   // a = break ;
            exit(-1);
        }
        
        if(cur_line[1] == "=")
        {
            std::string predicted_type = "";
            for(int i = 0; i < allvars.size(); ++i)   //a = b ;
            {
                if(cur_line[2] == allvars[i].second)
                {
                    predicted_type = allvars[i].first;
                    string_casts_assign(cur_line,line_number,predicted_type);
                }
            }

            if(predicted_type == "")
            {               
                if(cur_line[2][0] == '\"' && cur_line[2][(cur_line[2].size()) - 1] == '\"')
                {
                    std::string cur_str = cur_line[2];
                    cur_str.erase(cur_str.begin());
                    cur_str.erase(cur_str.end() - 1);
                    stringmap[cur_line[0]] = cur_str;
                    return;
                }
                
                error_string.was_not_dec(cur_line[2]);
                exit(-1);
                
            }
        }  


        else //this means it's compound assingnment
        {
            auto it = std::find(comp_assignments.begin(),comp_assignments.end() , cur_line[1]);
            if(it == comp_assignments.end())
            {
                error_string.invalid_assignment(line_number);
                exit(-1);
            }

            else    // a += b ;
            {
                std::string type = var_check(cur_line[2]);
                string_casts_comp_assign(cur_line,line_number,type,cur_line[1]);
            }
        } 
    }  

    if(cur_line.size() == 6) // assigns an expression
    {
        auto it1 = std::find(allkeywords.begin() , allkeywords.end() , cur_line[2]);
        auto it2 = std::find(allkeywords.begin() , allkeywords.end() , cur_line[4]);
        if(it1 != allkeywords.end())
        {
            error_string.is_keyword(cur_line[2]);
            exit(-1);
        }

        if(it2 != allkeywords.end())
        {
            error_string.is_keyword(cur_line[4]);
            exit(-1);
        }

        std::string operation = "";
        for(int i = 0; i < arithmetical_ops_binary.size() ; ++i)
        {
            if(cur_line[3] == arithmetical_ops_binary[i])
            {
                operation = cur_line[3];
            }
        }
        if(operation == "" || operation != "+")
        {
            error_char.invalid_assignment(line_number);
            exit(-1);
        }

        //case Char a,b .. ; c = a + b ; (or a and b can be another types except string)
        //flags are for checking if the operands are variables or literals
        //if in the end of checking value of any of flags stays false, assignment is invalid

        std::string val1 = "-1000000";
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[2] == allvars[i].second)
            {
                val1 = return_string(allvars[i].first , allvars[i].second ,line_number);
            }
        }

        std::string val2 = "-1000000";
        for(int i = 0; i < allvars.size(); ++i)
        {
            if(cur_line[4] == allvars[i].second)
            {
                val2 = return_string(allvars[i].first , allvars[i].second , line_number);
            }
        }

        std::string op1 = cur_line[2];
        std::string op2 = cur_line[4];
        if(val1 == "-1000000")
        {
            if(op1[0] == '\"' && op1[op1.size() - 1] == '\"')
            {
                op1.erase(op1.begin());
                op1.erase(op1.end() - 1);
                val1 = op1;
            }

            else
            {
                error_string.invalid_assignment(line_number);
                exit(-1);
            }
        }

        if(val2 == "-1000000")
        {   
            if(op2[0] == '\"' && op2[op2.size() - 1] == '\"')
            {
                op2.erase(op2.begin());
                op2.erase(op2.end() - 1);
                val2 = op2;
            }

            else
            {
                error_string.invalid_assignment(line_number);
                exit(-1);
            }
        }

        if(val1 != "-1000000"  && val2 != "-1000000")
        {
            operation_choice_string(cur_line , operation, val1 , val2 , line_number);
            return;
        }

        else
        {
            error_string.invalid_assignment(line_number);
        }
    }  
}

void string_casts_assign(std::vector<std::string> cur_line, int line_number, std::string var_type)
{
    if(var_type == "")
    {
        if(cur_line[2][0] == '\"' && cur_line[2][(cur_line[2].size()) - 1] == '\"')
        {
            std::string cur_str = cur_line[2];
            cur_str.erase(cur_str.begin());
            cur_str.erase(cur_str.end() - 1);
            stringmap[cur_line[0]] = cur_str;
            return;
        }
    }

    if(var_type != "String")       
    {
        error_string.type_incompatiblity(line_number);
        exit(-1);
    }

    else
    {
        auto it = stringmap.find(cur_line[2]);
        stringmap[cur_line[0]] = it-> second;
        return;
    }
}

void compound_add_string(std::vector<std::string> cur_line, std::string operand , int line_number)
{
    stringmap[cur_line[0]] += operand;
}

void string_casts_comp_assign(std::vector<std::string> cur_line , int line_number , std::string var_type , std::string operation)
{
    if(operation != "+=")
    {
        error_string.invalid_op(line_number);
        exit(-1);
    }

    std::string val = "-10000" ;

    if(var_type != "")
    {
        if(var_type != "String")
        {
            error_string.type_incompatiblity(line_number);
            exit(-1);
        }

        else
        {
            auto it = stringmap.find(cur_line[2]);
            val = it -> second; 
        }
    }

    else 
    {
        if(cur_line[2][0] == '\"' && cur_line[2][(cur_line[2].size()) - 1] == '\"')
        {
            std::string cur_str = cur_line[2];
            cur_str.erase(cur_str.begin());
            cur_str.erase(cur_str.end() - 1);
            stringmap[cur_line[0]] = cur_str;
            return;
        }

        else
        {
            error_string.invalid_op(line_number);
            exit(-1);
        }
    }

    compound_add_string(cur_line , val , line_number);
}

void add_string(std::vector<std::string> cur_line , std::string operand1,std::string operand2)
{
    stringmap[cur_line[0]] = operand1 + operand2;
}

void operation_choice_string(std::vector<std::string> cur_line ,std::string operation, std::string operand1,std::string operand2 , int line_number)
{
    if(operation != "+")
    {
        error_string.invalid_op(line_number);
        exit(-1);
    }
    add_string(cur_line,operand1,operand2);
    return;
}

std::string return_string(std::string var_type , std::string name , int line_number)
{
    if(var_type == "")
    {

    }
    
    if(var_type != "String")
    {
        error_string.invalid_assignment(line_number);
        exit(-1);
    }

    auto it = stringmap.find(name);
    return it -> second;
}    