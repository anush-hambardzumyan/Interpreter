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
void add_int(std::vector<std::string> cur_line , int operand1,int operand2);
void sub_int(std::vector<std::string> cur_line , int operand1,int operand2);
void mul_int(std::vector<std::string> cur_line , int operand1,int operand2);
void div1_int(std::vector<std::string> cur_line , int operand1,int operand2);
void div2_int(std::vector<std::string> cur_line , int operand1,int operand2);
void operation_choice_int(std::vector<std::string> cur_line ,std::string operation, int operand1,int operand2);
void compound_add_int(std::vector<std::string>cur_line, int operand, int line_number);
void compound_sub_int(std::vector<std::string>cur_line, int operand, int line_number);
void compound_mul_int(std::vector<std::string>cur_line, int operand, int line_number);
void compound_div1_int(std::vector<std::string>cur_line, int operand, int line_number);
void compound_div2_int(std::vector<std::string> cur_line, int operand, int line_number);
int return_int(std::string var_type , std::string name , int line_number);
void int_arr_dec(std::vector<std::string> cur_line , int line_number);

void int_var_dec(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_int.missing_semicolon(line_number);
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

    if(cur_line.size() == 4 || cur_line.size() == 8) //this cond means its array declaration
    {
        int_arr_dec(cur_line,line_number);
        return;
    }

    if(cur_line.size() == 3)  //just declaration without assigning
    {
        for(int i = 0 ; i < allarrays.size(); ++i)
        {
            if(cur_line[1] == allarrays[i].second)
            {
                error_int.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

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

    else if(cur_line[2] != "=")
    {
        error_int.invalid_op(line_number);
        exit(-1);
    }   

    else if(cur_line.size() == 5)       //Int a = 7 ;   Int A = b ;
    {
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[1] == allvars[i].second)
            {
                error_int.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        for(int i = 0 ; i < allarrays.size(); ++i)
        {
            if(cur_line[1] == allarrays[i].second)
            {
                error_int.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        for(int i = 0 ; i < allarrays.size(); ++i)
        {
            if(cur_line[3] == allarrays[i].second)
            {
                error_int.invalid_assignment(line_number);
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

        for(int i = 0; i < allarrays.size(); ++i)   //a = array ;
        {
            if(cur_line[2] == allarrays[i].second)
            {
                error_int.type_incompatiblity(line_number);
            }
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

    if(cur_line.size() == 6) // assigns an expression
    {
        auto it1 = std::find(allkeywords.begin() , allkeywords.end() , cur_line[2]);
        auto it2 = std::find(allkeywords.begin() , allkeywords.end() , cur_line[4]);
        if(it1 != allkeywords.end())
        {
            error_int.is_keyword(cur_line[2]);
            exit(-1);
        }
        if(it2 != allkeywords.end())
        {
            error_int.is_keyword(cur_line[4]);
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
            error_int.invalid_assignment(line_number);
            exit(-1);
        }

        //case Int a,b .. ; c = a + b ; (or a and b can be another types except string)
        //flags are for checking if the operands are variables or literals
        //if in the end of checking value of any of flags stays false, assignment is invalid

        int val1 = -1000000;
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[2] == allvars[i].second)
            {
                val1 = return_int(allvars[i].first , allvars[i].second ,line_number);
            }
        }

        int val2 = -1000000;
        for(int i = 0; i < allvars.size(); ++i)
        {
            if(cur_line[4] == allvars[i].second)
            {
                val2 = return_int(allvars[i].first , allvars[i].second , line_number);
            }
        }

        if(val1 == -1000000)
        {
            try
            {
                val1 = static_cast<int>(std::stod(cur_line[2]));    
            }

            catch(const std::exception& e)
            {
                if(cur_line[2][0] == '\"')  // Int A ;    A = "ib" + ..
                {
                    error_int.invalid_assignment(line_number);
                    exit(-1);
                }

                // Int A ;    A = 'c' + ...
                else if(cur_line[2].size() && cur_line[2][0] == '\'' && cur_line[2][2] == '\'')
                {
                    val1 = static_cast<int>(cur_line[2][1]);
                }

                else
                {
                    error_int.invalid_assignment(line_number);
                    exit(-1);
                }
            }
        }

        if(val2 == -1000000)
        {
            try
            {
                val2 = static_cast<int>(std::stod(cur_line[4]));    
            }

            catch(const std::exception& e)
            {
                if(cur_line[4][0] == '\"')  // Int A ;    A = "ib" + ..
                {
                    error_int.invalid_assignment(line_number);
                    exit(-1);
                }

                // Int A ;    A = 'c' + ...
                else if(cur_line[4].size() && cur_line[4][0] == '\'' && cur_line[4][2] == '\'')
                {
                    val2 = static_cast<int>(cur_line[4][1]);
                }

                else
                {
                    error_int.invalid_assignment(line_number);
                    exit(-1);
                }
            }
        }

        if(val1 != -1000000  && val2 != -1000000)
        {
            operation_choice_int(cur_line , operation, val1 , val2);
            return;
        }

        else
        {
            error_int.invalid_assignment(line_number);
        }
    }  
}

void operation_choice_int(std::vector<std::string> cur_line ,std::string operation, int operand1,int operand2)
{
    if(operation == "+")
    {
        add_int(cur_line,operand1,operand2);
        return;
    }

    if(operation == "-")
    {
        sub_int(cur_line,operand1,operand2);
        return;
    }

    if(operation == "*")
    {
        mul_int(cur_line,operand1,operand2);
        return;
    }

    if(operation == "/")
    {
        div1_int(cur_line,operand1,operand2);
        return;
    }

    if(operation == "%")
    {
        div2_int(cur_line,operand1,operand2);
        return;
    }
}

void add_int(std::vector<std::string> cur_line , int operand1,int operand2)
{
    intmap[cur_line[0]] = operand1 + operand2;
}

void sub_int(std::vector<std::string> cur_line ,int operand1,int operand2)
{
    intmap[cur_line[0]] = operand1 - operand2;
}

void mul_int(std::vector<std::string> cur_line ,int operand1,int operand2)
{
    intmap[cur_line[0]] = operand1 * operand2;
}

void div1_int(std::vector<std::string> cur_line ,int operand1,int operand2)
{
    intmap[cur_line[0]] = operand1 / operand2;
}

void div2_int(std::vector<std::string> cur_line ,int operand1,int operand2)
{
    intmap[cur_line[0]] = operand1 % operand2;
}

int return_int(std::string var_type , std::string name , int line_number)
{
    if(var_type == "String")
    {
        error_int.invalid_assignment(line_number);
        exit(-1);
    }

    if(var_type == "Int")
    {
        auto it = intmap.find(name);
        return it -> second;
    }

    if(var_type == "Double")
    {
        auto it = doublemap.find(name);
        return it -> second;
    }

    if(var_type == "Float")
    {
        auto it = floatmap.find(name);
        return it -> second;
    }

    if(var_type == "Bool")
    {
        auto it = boolmap.find(name);
        return it -> second;
    }

    if(var_type == "Char")
    {
        auto it = charmap.find(name);
        return it -> second;
    }
    return 0;
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

void compound_add_int(std::vector<std::string> cur_line,int operand , int line_number)
{
    intmap[cur_line[0]] += operand;
}

void compound_sub_int(std::vector<std::string> cur_line ,int operand ,int line_number)
{
    intmap[cur_line[0]] -= operand;
}

void compound_mul_int(std::vector<std::string> cur_line, int operand, int line_number)
{
    intmap[cur_line[0]] *= operand;
}

void compound_div1_int(std::vector<std::string> cur_line, int operand,int line_number)
{
    intmap[cur_line[0]] /= operand;
}

void compound_div2_int(std::vector<std::string> cur_line, int operand, int line_number)
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
        compound_add_int(cur_line ,val,line_number);
    }

    if(operation == "-=")
    {
        compound_sub_int(cur_line ,val,line_number);
    }

    if(operation == "*=")
    {
        compound_mul_int(cur_line, val,line_number);
    }

    if(operation == "/=")
    {
        compound_div1_int(cur_line , val,line_number);
    }

    if(operation == "%=")
    {
        compound_div2_int(cur_line , val,line_number);
    }
}

void int_arr_dec(std::vector<std::string> cur_line , int line_number)
{
    for(int i = 0; i < allvars.size() ; ++i)
    {
        if(cur_line[1] == allvars[i].second)
        {
            error_int.redeclaration(line_number,cur_line[1]);
            exit(-1);
        }
    }

    for(int i = 0; i < allarrays.size() ; ++i)
    {
        if(cur_line[1] == allarrays[i].second)
        {
            error_int.redeclaration(line_number,cur_line[1]);
            exit(-1);
        }
    }

    std::string size_str = cur_line[2];  
    int array_size = 0;      
    if(size_str[0] != '[' || size_str[size_str.size() - 1] != ']')
    {
        error_int.invalid_dec(line_number);
        exit(-1);
    }
    size_str.erase(size_str.begin());
    size_str.erase(size_str.end() - 1);
    std::string pred_type = "";

    for(int i = 0; i < allvars.size() ; ++i)
    {
        if(size_str == allvars[i].second)
        {
            pred_type = allvars[i].first;
        }
    }

    if(pred_type != "")
    {
        if(pred_type == "Int")
        {
            auto it = intmap.find(size_str);
            array_size = it -> second;
        }

        else
        {
            error_int.invalid_dec(line_number);
            exit(-1);
        }
    }

    else
    {
        try
        {
            array_size = std::stoi(size_str);
        }
        catch(const std::exception& e)
        {
            error_int.invalid_dec(line_number);
            exit(-1);
        }
    }
    //Int array1 [size] ; just declaration
    if(cur_line.size() == 4)
    {
        intarrmap.insert(intarrmap.begin(), {cur_line[0], std::list<int>(array_size, 0)});
        allarrays.push_back(std::make_pair("Int" , cur_line[1]));
    }

    //Int array [5] = { 1,2,3,4,5 } ;
    if(cur_line.size() == 8)
    {
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[1] == allvars[i].second)
            {
                error_int.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        for(int i = 0; i < allarrays.size() ; ++i)
        {
            if(cur_line[1] == allarrays[i].second)
            {
                error_int.redeclaration(line_number,cur_line[1]);
                exit(-1);
            }
        }

        std::string size_str = cur_line[2];  
        int array_size = 0;      
        if(size_str[0] != '[' || size_str[size_str.size() - 1] != ']' || cur_line[3] != "=")
        {
            error_int.invalid_dec(line_number);
            exit(-1);
        }

        if(cur_line[4] != "{" || cur_line[6] != "}")
        {
            error_int.invalid_dec(line_number);
            exit(-1);
        }

        size_str.erase(size_str.begin());
        size_str.erase(size_str.end() - 1);
        std::string pred_type = "";

        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(size_str == allvars[i].second)
            {
                pred_type = allvars[i].first;
            }
        }

        if(pred_type != "")
        {
            if(pred_type == "Int")
            {
                auto it = intmap.find(size_str);
                array_size = it -> second;
            }

            else
            {
                error_int.invalid_dec(line_number);
                exit(-1);
            }
        }

        else
        {
            try
            {
                array_size = std::stoi(size_str);
            }
            catch(const std::exception& e)
            {
                error_int.invalid_dec(line_number);
                exit(-1);
            }
        }

        std::string input = cur_line[5] ;
        std::istringstream ss(input);
        std::string var;
        std::list<int> vars;

        while (std::getline(ss, var, ',')) 
        {
            try 
            {
                int value = std::stoi(var);
                vars.push_back(value);
            } 
            catch (const std::exception& e) 
            {
                error_int.type_incompatiblity(line_number);
                exit(-1);
            } 
        }    

        if(vars.size() > array_size)
        {
            error_int.too_many_args(cur_line[1]);
            exit(-1);
        }

        intarrmap.insert(intarrmap.begin(), {cur_line[0], vars});
        allarrays.push_back(std::make_pair("Int" , cur_line[1]));
    }        
}
