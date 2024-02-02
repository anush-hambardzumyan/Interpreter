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
void bool_var_dec(std::vector<std::string> cur_line,int line_number);
void bool_casts_init(std::vector<std::string> cur_line, int line_number,std::string var_type);
void bool_var_assign(std::vector<std::string> cur_line,int line_number);
void bool_casts_assign(std::vector<std::string> cur_line, int line_number, std::string var_type);
void bool_casts_comp_assign(std::vector<std::string> cur_line , int line_number , std::string var_type , std::string operation);
void add_bool(std::vector<std::string> cur_line , bool operand1,bool operand2);
void sub_bool(std::vector<std::string> cur_line , bool operand1,bool operand2);
void mul_bool(std::vector<std::string> cur_line , bool operand1,bool operand2);
void div1_bool(std::vector<std::string> cur_line , bool operand1,bool operand2);
void div2_bool(std::vector<std::string> cur_line , bool operand1,bool operand2);
void operation_choice_bool(std::vector<std::string> cur_line ,std::string operation, bool operand1,bool operand2 , int line_number);
void compound_add_bool(std::vector<std::string>cur_line, bool operand);
void compound_sub_bool(std::vector<std::string>cur_line, bool operand);
void compound_mul_bool(std::vector<std::string>cur_line, bool operand);
void compound_div1_bool(std::vector<std::string>cur_line, bool operand);
void compound_div2_bool(std::vector<std::string>cur_line, bool operand);
bool return_bool(std::string var_type , std::string name , int line_number);


void bool_var_dec(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_bool.missing_semicolon(line_number);
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


    else if(cur_line[2] != "=")
    {
        error_bool.invalid_op(line_number);
        exit(-1);
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
                    bool value = static_cast<bool> (std::stod(cur_line[3]));     //if it's literal ,insert in map
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

void bool_var_assign(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_bool.missing_semicolon(line_number);
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

    if(cur_line.size() == 3 ) // it should be postfix increment or decrement
    {
        if(cur_line[1] == "++") // a ++ ;
        {
            boolmap[cur_line[0]] = 1;
        }

        else if(cur_line[1] == "--") //a --;
        {
            boolmap[cur_line[0]] = 0;
        }

        else
        {
            error_bool.invalid_assignment(line_number);
            exit(-1);
        }
    }

    if(cur_line.size() == 4) //it should be ordinary assignment or compound assignment
    {
        auto it = std::find(allkeywords.begin() , allkeywords.end() ,cur_line[2]);
        if(it != allkeywords.end())
        {
            error_bool.is_keyword(cur_line[2]);   // a = break ;
            exit(-1);
        }
        
        if(cur_line[1] == "=")
        {
            if(cur_line[2] == "true")
            {
                cur_line[2] = "1";
            } 

            if(cur_line[2] == "false")
            {
                cur_line[2] = "0";
            }

            std::string predicted_type = "";
            for(int i = 0; i < allvars.size(); ++i)   //a = b ;
            {
                if(cur_line[2] == allvars[i].second)
                {
                    predicted_type = allvars[i].first;
                    bool_casts_assign(cur_line,line_number,predicted_type);
                }
            }

            if(predicted_type == "")
            {               
                try                 
                {                                           
                    bool value = std::stoi(cur_line[2]);     
                    boolmap[cur_line[0]] = value;                 
                    return;
                } 

                catch (const std::invalid_argument& e) 
                {
                    if(cur_line[2].size() == 3 && cur_line[2][0] == '\'' && cur_line[2][2] == '\'')
                    {
                        boolmap[cur_line[0]] = 1;  //a = 'A' ;
                        return;
                    }

                    if(cur_line[2][0] == '\"' && cur_line[2][(cur_line[2].size()) - 1] == '\"')
                    {
                        error_bool.invalid_assignment(line_number);   //a = "bckdjs" ;
                        exit(-1);
                    }

                    error_bool.was_not_dec(cur_line[2]);
                    exit(-1);
                }
            }
        }  


        else //this means it's compound assingnment
        {
            auto it = std::find(comp_assignments.begin(),comp_assignments.end() , cur_line[1]);
            if(it == comp_assignments.end())
            {
                error_bool.invalid_assignment(line_number);
                exit(-1);
            }

            else    // a += b ;
            {
                if(cur_line[2] == "true")
                {
                    cur_line[2] = "1";
                } 

                if(cur_line[2] == "false")
                {
                    cur_line[2] = "0";
                }
                std::string type = var_check(cur_line[2]);
                bool_casts_comp_assign(cur_line,line_number,type,cur_line[1]);
            }
        } 
    }

    if(cur_line.size() == 6) // assigns an expression
    {
        auto it1 = std::find(allkeywords.begin() , allkeywords.end() , cur_line[2]);
        auto it2 = std::find(allkeywords.begin() , allkeywords.end() , cur_line[4]);
        if(it1 != allkeywords.end())
        {
            error_bool.is_keyword(cur_line[2]);
            exit(-1);
        }
        if(it2 != allkeywords.end())
        {
            error_bool.is_keyword(cur_line[4]);
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
        if(operation == "")
        {
            error_bool.invalid_assignment(line_number);
            exit(-1);
        }

        //case Bool a,b .. ; c = a + b ; (or a and b can be another types except string)
        //flags are for checking if the operands are variables or literals
        //if in the end of checking value of any of flags stays false, assignment is invalid

        int val1 = -1000000;
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[2] == allvars[i].second)
            {
                val1 = return_bool(allvars[i].first , allvars[i].second ,line_number);
            }
        }

        int val2 = -1000000;
        for(int i = 0; i < allvars.size(); ++i)
        {
            if(cur_line[4] == allvars[i].second)
            {
                val2 = return_bool(allvars[i].first , allvars[i].second , line_number);
            }
        }

        if(val1 == -1000000)
        {
            if(cur_line[2] == "true")
            {
                cur_line[2] = "1";
            } 

            if(cur_line[2] == "false")
            {
                cur_line[2] = "0";
            }

            try
            {
                val1 = static_cast<bool>(std::stod(cur_line[2]));    
            }

            catch(const std::exception& e)
            {
                if(cur_line[2][0] == '\"')  // Bool A ;    A = "ib" + ..
                {
                    error_bool.invalid_assignment(line_number);
                    exit(-1);
                }

                // Bool A ;    A = 'c' + ...
                else if(cur_line[2].size() && cur_line[2][0] == '\'' && cur_line[2][2] == '\'')
                {
                    val1 = static_cast<bool>(cur_line[2][1]);
                }

                else
                {
                    error_bool.invalid_assignment(line_number);
                    exit(-1);
                }
            }
        }

        if(val2 == -1000000)
        {
            if(cur_line[4] == "true")
            {
                cur_line[4] = "1";
            } 

            if(cur_line[4] == "false")
            {
                cur_line[4] = "0";
            }
            try
            {
                val2 = static_cast<double>(std::stod(cur_line[4]));    
            }

            catch(const std::exception& e)
            {
                if(cur_line[4][0] == '\"')  // Bool A ;    A = "ib" + ..
                {
                    error_bool.invalid_assignment(line_number);
                    exit(-1);
                }

                // Bool A ;    A = 'c' + ...
                else if(cur_line[4].size() && cur_line[4][0] == '\'' && cur_line[4][2] == '\'')
                {
                    val2 = static_cast<bool>(cur_line[4][1]);
                }

                else
                {
                    error_bool.invalid_assignment(line_number);
                    exit(-1);
                }
            }
        }

        if(val1 != -1000000  && val2 != -1000000)
        {
            operation_choice_bool(cur_line , operation, val1 , val2 , line_number);
            return;
        }

        else
        {
            error_bool.invalid_assignment(line_number);
        }
    }      
}

void bool_casts_assign(std::vector<std::string> cur_line , int line_number , std::string var_type)
{
    if(var_type == "")
    {
        if(cur_line[2] == "true" || cur_line[2] == "1")
        {
            boolmap[cur_line[0]] = 1;
        } 

        if(cur_line[2] == "false" || cur_line[2] == "0")
        {
            boolmap[cur_line[0]] = 1;
        }
        return;
    }

    if(var_type == "String")       
    {
        error_bool.type_incompatiblity(line_number);
        exit(-1);
    }

    else
    {
        if(var_type == "Double")
        {
            auto it = doublemap.find(cur_line[2]);
            boolmap[cur_line[0]] = static_cast<bool>(it-> second);
            return;
        }

        if(var_type == "Int")
        {
            auto it = intmap.find(cur_line[2]);
            boolmap[cur_line[0]] = static_cast<bool>(it-> second);
            return;
        }

        else if(var_type == "Float")
        {
            auto it = floatmap.find(cur_line[2]);
            boolmap[cur_line[0]] = static_cast<bool>(it-> second);
            return;
        }

        else if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[2]);
            boolmap[cur_line[0]] = static_cast<bool>(it-> second);
            return;
        }

        else if (var_type == "Bool")
        {
            auto it = boolmap.find(cur_line[2]);
            boolmap[cur_line[0]] = static_cast<bool>(it-> second);
            return;
        }
    }
}

void compound_add_bool(std::vector<std::string> cur_line, bool operand)
{
    boolmap[cur_line[0]] += operand;
}

void compound_sub_bool(std::vector<std::string> cur_line, bool operand)
{
    boolmap[cur_line[0]] -= operand;
}

void compound_mul_bool(std::vector<std::string> cur_line, bool operand)
{
    boolmap[cur_line[0]] *= operand;
}

void compound_div1_bool(std::vector<std::string> cur_line, bool operand)
{
    boolmap[cur_line[0]] /= operand;
}

void compound_div2_bool(std::vector<std::string> cur_line, bool operand)
{
    boolmap[cur_line[0]] %= operand;
}

void bool_casts_comp_assign(std::vector<std::string> cur_line , int line_number , std::string var_type , std::string operation)
{
    double val = -10000 ;

    if(var_type != "")
    {
        if(var_type == "String")
        {
            error_bool.type_incompatiblity(line_number);
            exit(-1);
        }

        if(var_type == "Int")
        {
            auto it = intmap.find(cur_line[2]);
            val = static_cast<bool> (it -> second); 
        }

        if(var_type == "Double")
        {
            auto it = doublemap.find(cur_line[2]);
            val = static_cast<bool> (it -> second); 
        }

        if(var_type == "Bool")
        {
            auto it = boolmap.find(cur_line[2]);
            val = static_cast<bool> (it -> second); 
        } 

        if(var_type == "Float")
        {
            auto it = floatmap.find(cur_line[2]);
            val = static_cast<bool> (it -> second); 
        }

        if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[2]);
            val = static_cast<bool> (it -> second); 
        }
    }

    else 
    {
        //a += 'c' ;
        if(cur_line[2].size() == 3 && cur_line[2][0] == '\'' && cur_line[2][2] == '\'')
        {
            val = static_cast<bool>(cur_line[2][1]);
        }

        if(val == -10000)
        {
            try
            {
                val = static_cast<bool>(std::stod(cur_line[2]));
            }

            catch(const std::exception& e)
            {
                error_bool.invalid_assignment(line_number);
                exit(-1);
            }
            
        }
    }

    if(operation == "+=")
    {
        compound_add_bool(cur_line ,val);
    }

    if(operation == "-=")
    {
        compound_sub_bool(cur_line ,val);
    }

    if(operation == "*=")
    {
        compound_mul_bool(cur_line, val);
    }

    if(operation == "/=")
    {
        compound_div1_bool(cur_line , val);
    }

    if(operation == "%=")
    {
        compound_div2_bool(cur_line , val);
    }
}

void add_bool(std::vector<std::string> cur_line , bool operand1 , bool operand2)
{
    boolmap[cur_line[0]] = operand1 + operand2;
}

void sub_bool(std::vector<std::string> cur_line , bool operand1 , bool operand2)
{
    boolmap[cur_line[0]] = operand1 - operand2;
}

void mul_bool(std::vector<std::string> cur_line , bool operand1 , bool operand2)
{
    boolmap[cur_line[0]] = operand1 * operand2;
}

void div1_bool(std::vector<std::string> cur_line , bool operand1 , bool operand2)
{
    boolmap[cur_line[0]] = operand1 / operand2;
}

void div2_bool(std::vector<std::string> cur_line , bool operand1 , bool operand2)
{
    boolmap[cur_line[0]] = operand1 % operand2;
}

void operation_choice_bool(std::vector<std::string> cur_line ,std::string operation, bool operand1 , bool operand2 , int line_number)
{
    if(operation == "+")
    {
        add_bool(cur_line,operand1,operand2);
        return;
    }

    if(operation == "-")
    {
        sub_bool(cur_line,operand1,operand2);
        return;
    }

    if(operation == "*")
    {
        mul_bool(cur_line,operand1,operand2);
        return;
    }

    if(operation == "/")
    {
        div1_bool(cur_line,operand1,operand2);
        return;
    }

    if(operation == "%")
    {
        div2_bool(cur_line,operand1,operand2);
        return;
    }
}

bool return_bool(std::string var_type , std::string name , int line_number)
{
    if(var_type == "String")
    {
        error_bool.invalid_assignment(line_number);
        exit(-1);
    }

    if(var_type == "Double")
    {
        auto it = doublemap.find(name);
        return static_cast<bool>(it -> second);
    }

    if(var_type == "Int")
    {
        auto it = intmap.find(name);
        return static_cast<bool>(it -> second);
    }

    if(var_type == "Float")
    {
        auto it = floatmap.find(name);
        return static_cast<bool>(it -> second);
    }

    if(var_type == "Bool")
    {
        auto it = boolmap.find(name);
        return static_cast<bool>(it -> second);
    }

    if(var_type == "Char")
    {
        auto it = charmap.find(name);
        return static_cast<bool>(it -> second);
    }
    return 0;
}    