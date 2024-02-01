#pragma once
#include <iostream>
#include <map>
#include <utility>
#include <algorithm>
#include "parser.hpp"
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"

Error error_int;
void int_var_dec(std::vector<std::string> cur_line,int line_number);
void int_casts_init(std::vector<std::string> cur_line, int line_number, std::string var_type);
void int_var_assign(std::vector<std::string> cur_line,int line_number);
void int_casts_assign(std::vector<std::string> cur_line, int line_number, std::string var_type);
void int_casts_comp_assign(std::vector<std::string> cur_line , int line_number , std::string var_type , std::string operation);
void compound_add_int(std::vector<std::string>cur_line, int operand);
void compound_sub_int(std::vector<std::string>cur_line, int operand);
void compound_mul_int(std::vector<std::string>cur_line, int operand);
void compound_div1_int(std::vector<std::string>cur_line, int operand);
void compound_div2_int(std::vector<std::string> cur_line, int operand);


void int_var_dec(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_int.missing_semicolon(line_number);
        exit(-1);
    }  

    if(cur_line[2] != "=")
    {
        error_int.invalid_op(line_number);
        exit(-1);
    }   

    for(int i = 0; i < allkeywords.size(); ++i)
    {
        if(cur_line[1] == allkeywords[i])
        {
            error_int.is_keyword(cur_line[1]);
            exit(-1);
        }
    }

    if(cur_line.size() == 3)
    {
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[1] == allvars[i].second)
            {
                error_int.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        intmap.insert(std::make_pair(cur_line[1],0));
        allvars.push_back(std::make_pair("Int" , cur_line[1]));
    }

    else if(cur_line.size() == 5)
    {
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[1] == allvars[i].second)
            {
                error_int.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        std::string var_type = "";

        for(int i = 0; i < allvars.size(); ++i)
        {
            if(cur_line[3] == allvars[i].second)
            {
                var_type = allvars[i].first;
                int_casts_init(cur_line,line_number,var_type);
            }
        }

        if(var_type == "")        //if the right operand was not found it means that the right operand is literal or undefined variable or keyword
        {
            for(int i = 0; i < allkeywords.size(); ++i)
            {
                if(allkeywords[i] == cur_line[3])
                {
                    error_int.is_keyword(cur_line[3]);
                    exit(-1);
                }
            }

            auto it = intmap.find(cur_line[3]);   //Int A = 10 ;
            if(it == intmap.end())                
            {
                try                 
                {                                           //Int A = 10 ;
                    int value = std::stoi(cur_line[3]);     //if it's literal ,insert in map
                    intmap[cur_line[1]] = value;    
                    allvars.push_back(std::make_pair("Int" , cur_line[1]));                  
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

    else
    {
        error_int.invalid_assignment(line_number);
    }
}

void int_casts_init(std::vector<std::string> cur_line , int line_number , std::string var_type)
{
    if(var_type == "")
    {
        return ;
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
            allvars.push_back(std::make_pair("Int" , cur_line[1]));
            return;
        }

        if(var_type == "Int")
        {
            auto it = intmap.find(cur_line[3]);
            intmap[cur_line[1]] = it -> second;
            allvars.push_back(std::make_pair("Int" , cur_line[1]));
            return;
        }

        else if(var_type == "Float")
        {
            auto it = floatmap.find(cur_line[3]);
            intmap[cur_line[1]] = it -> second;
            allvars.push_back(std::make_pair("Int" , cur_line[1]));
            return;
        }

        else if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[3]);
            intmap[cur_line[1]] = it -> second;
            allvars.push_back(std::make_pair("Int" , cur_line[1]));
            return;
        }

        else if (var_type == "Bool")
        {
            auto it = boolmap.find(cur_line[3]);
            intmap[cur_line[1]] = static_cast<int>(it->second);
            allvars.push_back(std::make_pair("Int" , cur_line[1]));
            return;
        }
    }
}

void int_var_assign(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_int.missing_semicolon(line_number);
        exit(-1);
    }

    auto it = std::find(comparison_ops.begin() , comparison_ops.end(), cur_line[1]);
    if(it != comparison_ops.end())
    {
        return ; //a > b ;  expression should be optimized
    }

    if(cur_line.size() == 3 ) // it should be postfix increment or decrement
    {
        if(cur_line[1] == "++") // a ++ ;
        {
            intmap[cur_line[0]] += 1;
        }

        else if(cur_line[1] == "--") //a --;
        {
            intmap[cur_line[0]] -= 1;
        }

        else
        {
            error_int.invalid_assignment(line_number);
            exit(-1);
        }
    }

    if(cur_line.size() == 4) //it should be ordinary assignment or compound assignment
    {
        auto it = std::find(allkeywords.begin() , allkeywords.end() ,cur_line[2]);
        if(it != allkeywords.end())
        {
            error_int.is_keyword(cur_line[2]);   // a = break ;
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
                    int_casts_assign(cur_line,line_number,predicted_type);
                }
            }

            if(predicted_type == "")
            {               
                try                 
                {                                           
                    int value = std::stoi(cur_line[2]);     
                    intmap[cur_line[0]] = value;                 
                    return;
                } 

                catch (const std::invalid_argument& e) 
                {
                    if(cur_line[2].size() == 3 && cur_line[2][0] == '\'' && cur_line[2][2] == '\'')
                    {
                        intmap[cur_line[0]] = static_cast<int> (cur_line[2][1]);  //a = 'A' ;
                        return;
                    }

                    if(cur_line[2][0] == '\"' && cur_line[2][(cur_line[2].size()) - 1] == '\"')
                    {
                        error_int.invalid_assignment(line_number);   //a = "bckdjs" ;
                        exit(-1);
                    }
                    
                    error_int.was_not_dec(cur_line[2]);
                    exit(-1);
                }
            }
        }  


        else //this means it's compound assingnment
        {
            auto it = std::find(comp_assignments.begin(),comp_assignments.end() , cur_line[1]);
            if(it == comp_assignments.end())
            {
                error_int.invalid_assignment(line_number);
                exit(-1);
            }

            else    // a += b ;
            {
                std::string type = var_check(cur_line[2]);
                int_casts_comp_assign(cur_line,line_number,type,cur_line[1]);
            }
        } 
    }    
}

void int_casts_assign(std::vector<std::string> cur_line , int line_number , std::string var_type)
{
    if(var_type == "")
    {
        return ;
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
            auto it = doublemap.find(cur_line[2]);
            intmap[cur_line[0]] = it -> second;
            return;
        }

        if(var_type == "Int")
        {
            auto it = intmap.find(cur_line[2]);
            intmap[cur_line[0]] = it -> second;
            return;
        }

        else if(var_type == "Float")
        {
            auto it = floatmap.find(cur_line[2]);
            intmap[cur_line[0]] = it -> second;
            return;
        }

        else if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[2]);
            intmap[cur_line[0]] = it -> second;
            return;
        }

        else if (var_type == "Bool")
        {
            auto it = boolmap.find(cur_line[2]);
            intmap[cur_line[0]] = static_cast<int>(it->second);
            return;
        }
    }

}

void compound_add_int(std::vector<std::string> cur_line,int operand)
{
    intmap[cur_line[0]] += operand;
}

void compound_sub_int(std::vector<std::string> cur_line ,int operand)
{
    intmap[cur_line[0]] -= operand;
}

void compound_mul_int(std::vector<std::string> cur_line, int operand)
{
    intmap[cur_line[0]] *= operand;
}

void compound_div1_int(std::vector<std::string> cur_line, int operand)
{
    intmap[cur_line[0]] /= operand;
}

void compound_div2_int(std::vector<std::string> cur_line, int operand)
{
    intmap[cur_line[0]] %= operand;
}

void int_casts_comp_assign(std::vector<std::string> cur_line , int line_number , std::string var_type , std::string operation)
{
    int val = -10000 ;

    if(var_type != "")
    {
        if(var_type == "String")
        {
            error_int.type_incompatiblity(line_number);
            exit(-1);
        }

        if(var_type == "Int")
        {
            auto it = intmap.find(cur_line[2]);
            val = static_cast<int> (it -> second); 
        }

        if(var_type == "Bool")
        {
            auto it = boolmap.find(cur_line[2]);
            val = static_cast<int> (it -> second); 
        }

        if(var_type == "Double") 
        {
            auto it = doublemap.find(cur_line[2]);
            val = static_cast<int> (it -> second); 
        }   

        if(var_type == "Float")
        {
            auto it = floatmap.find(cur_line[2]);
            val = static_cast<int> (it -> second); 
        }

        if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[2]);
            val = static_cast<int> (it -> second); 
        }
    }

    else //case1: a += 'c' ;  case2: a += 90.829 ;
    {
          //a += 'c' ;
        if(cur_line[2].size() == 3 && cur_line[2][0] == '\'' && cur_line[2][2] == '\'')
        {
            val = static_cast<int>(cur_line[2][1]);
        }

        if(val == -10000)
        {
            try
            {
                val = static_cast<int>(std::stod(cur_line[2]));
            }

            catch(const std::exception& e)
            {
                error_int.invalid_assignment(line_number);
                exit(-1);
            }
            
        }
    }

    if(operation == "+=")
    {
        compound_add_int(cur_line ,val);
    }

    if(operation == "-=")
    {
        compound_sub_int(cur_line ,val);
    }

    if(operation == "*=")
    {
        compound_mul_int(cur_line, val);
    }

    if(operation == "/=")
    {
        compound_div1_int(cur_line , val);
    }

    if(operation == "%=")
    {
        compound_div2_int(cur_line , val);
    }
}