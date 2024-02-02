#pragma once
#include <utility>
#include <algorithm>
#include "parser.hpp"
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"

Error error_double;
void double_var_dec(std::vector<std::string> cur_line,int line_number);
void double_casts_init(std::vector<std::string> cur_line,int line_number ,std::string var_type);
void double_var_assign(std::vector<std::string> cur_line,int line_number);
void double_casts_assign(std::vector<std::string> cur_line, int line_number, std::string var_type);
void double_casts_comp_assign(std::vector<std::string> cur_line , int line_number , std::string var_type , std::string operation);
void add_double(std::vector<std::string> cur_line ,  double operand1 ,double operand2);
void sub_double(std::vector<std::string> cur_line , double operand1 ,double operand2);
void mul_double(std::vector<std::string> cur_line , double operand1 ,double operand2);
void div1_double(std::vector<std::string> cur_line , double operand1 ,double operand2);
void operation_choice_double(std::vector<std::string> cur_line ,std::string operation, double operand1 ,double operand2 , int line_number);
void compound_add_double(std::vector<std::string>cur_line, double operand);
void compound_sub_double(std::vector<std::string>cur_line, double operand);
void compound_mul_double(std::vector<std::string>cur_line, double operand);
void compound_div1_double(std::vector<std::string>cur_line, double operand);
int return_double(std::string var_type , std::string name , int line_number);



void double_var_dec(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_double.missing_semicolon(line_number);
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

    else if(cur_line[2] != "=")
    {
        error_double.invalid_op(line_number);
        exit(-1);
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

void double_var_assign(std::vector<std::string> cur_line,int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_double.missing_semicolon(line_number);
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
            doublemap[cur_line[0]] += 1;
        }

        else if(cur_line[1] == "--") //a --;
        {
            doublemap[cur_line[0]] -= 1;
        }

        else
        {
            error_double.invalid_assignment(line_number);
            exit(-1);
        }
    }

    if(cur_line.size() == 4) //it should be ordinary assignment or compound assignment
    {
        auto it = std::find(allkeywords.begin() , allkeywords.end() ,cur_line[2]);
        if(it != allkeywords.end())
        {
            error_double.is_keyword(cur_line[2]);   // a = break ;
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
                    double_casts_assign(cur_line,line_number,predicted_type);
                }
            }

            if(predicted_type == "")
            {               
                try                 
                {                                           
                    double value = std::stod(cur_line[2]);     
                    doublemap[cur_line[0]] = value;                 
                    return;
                } 

                catch (const std::invalid_argument& e) 
                {
                    if(cur_line[2].size() == 3 && cur_line[2][0] == '\'' && cur_line[2][2] == '\'')
                    {
                        doublemap[cur_line[0]] = static_cast<int> (cur_line[2][1]);  //a = 'A' ;
                        return;
                    }

                    if(cur_line[2][0] == '\"' && cur_line[2][(cur_line[2].size()) - 1] == '\"')
                    {
                        error_double.invalid_assignment(line_number);   //a = "bckdjs" ;
                        exit(-1);
                    }
                    
                    error_double.was_not_dec(cur_line[2]);
                    exit(-1);
                }
            }
        }  


        else //this means it's compound assingnment
        {
            auto it = std::find(comp_assignments.begin(),comp_assignments.end() , cur_line[1]);
            if(it == comp_assignments.end())
            {
                error_double.invalid_assignment(line_number);
                exit(-1);
            }

            else    // a += b ;
            {
                std::string type = var_check(cur_line[2]);
                double_casts_comp_assign(cur_line,line_number,type,cur_line[1]);
            }
        } 
    }

    if(cur_line.size() == 6) // assigns an expression
    {
        auto it1 = std::find(allkeywords.begin() , allkeywords.end() , cur_line[2]);
        auto it2 = std::find(allkeywords.begin() , allkeywords.end() , cur_line[4]);
        if(it1 != allkeywords.end())
        {
            error_double.is_keyword(cur_line[2]);
            exit(-1);
        }
        if(it2 != allkeywords.end())
        {
            error_double.is_keyword(cur_line[4]);
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
            error_double.invalid_assignment(line_number);
            exit(-1);
        }

        //case Double a,b .. ; c = a + b ; (or a and b can be another types except string)
        //flags are for checking if the operands are variables or literals
        //if in the end of checking value of any of flags stays false, assignment is invalid

        double val1 = -1000000;
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[2] == allvars[i].second)
            {
                val1 = return_double(allvars[i].first , allvars[i].second ,line_number);
            }
        }

        double val2 = -1000000;
        for(int i = 0; i < allvars.size(); ++i)
        {
            if(cur_line[4] == allvars[i].second)
            {
                val2 = return_double(allvars[i].first , allvars[i].second , line_number);
            }
        }

        if(val1 == -1000000)
        {
            try
            {
                val1 = static_cast<double>(std::stod(cur_line[2]));    
            }

            catch(const std::exception& e)
            {
                if(cur_line[2][0] == '\"')  // Int A ;    A = "ib" + ..
                {
                    error_float.invalid_assignment(line_number);
                    exit(-1);
                }

                // Int A ;    A = 'c' + ...
                else if(cur_line[2].size() && cur_line[2][0] == '\'' && cur_line[2][2] == '\'')
                {
                    val1 = static_cast<double>(cur_line[2][1]);
                }

                else
                {
                    error_double.invalid_assignment(line_number);
                    exit(-1);
                }
            }
        }

        if(val2 == -1000000)
        {
            try
            {
                val2 = static_cast<double>(std::stod(cur_line[4]));    
            }

            catch(const std::exception& e)
            {
                if(cur_line[4][0] == '\"')  // Float A ;    A = "ib" + ..
                {
                    error_double.invalid_assignment(line_number);
                    exit(-1);
                }

                // Float A ;    A = 'c' + ...
                else if(cur_line[4].size() && cur_line[4][0] == '\'' && cur_line[4][2] == '\'')
                {
                    val2 = static_cast<double>(cur_line[4][1]);
                }

                else
                {
                    error_double.invalid_assignment(line_number);
                    exit(-1);
                }
            }
        }

        if(val1 != -1000000  && val2 != -1000000)
        {
            operation_choice_double(cur_line , operation, val1 , val2 , line_number);
            return;
        }

        else
        {
            error_double.invalid_assignment(line_number);
        }
    }      
}

void double_casts_assign(std::vector<std::string> cur_line , int line_number , std::string var_type)
{
    if(var_type == "")
    {
        return ;
    }

    if(var_type == "String")       
    {
        error_double.type_incompatiblity(line_number);
        exit(-1);
    }

    else
    {
        if(var_type == "Double")
        {
            auto it = doublemap.find(cur_line[2]);
            doublemap[cur_line[0]] = static_cast<double>(it-> second);
            return;
        }

        if(var_type == "Int")
        {
            auto it = intmap.find(cur_line[2]);
            doublemap[cur_line[0]] = static_cast<double>(it-> second);
            return;
        }

        else if(var_type == "Float")
        {
            auto it = floatmap.find(cur_line[2]);
            doublemap[cur_line[0]] = static_cast<double>(it-> second);
            return;
        }

        else if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[2]);
            doublemap[cur_line[0]] = static_cast<double>(it-> second);
            return;
        }

        else if (var_type == "Bool")
        {
            auto it = boolmap.find(cur_line[2]);
            doublemap[cur_line[0]] = static_cast<double>(it-> second);
            return;
        }
    }
}

void double_casts_comp_assign(std::vector<std::string> cur_line , int line_number , std::string var_type , std::string operation)
{
    double val = -10000 ;

    if(var_type != "")
    {
        if(var_type == "String")
        {
            error_double.type_incompatiblity(line_number);
            exit(-1);
        }

        if(var_type == "Int")
        {
            auto it = intmap.find(cur_line[2]);
            val = static_cast<double> (it -> second); 
        }

        if(var_type == "Double")
        {
            auto it = doublemap.find(cur_line[2]);
            val = static_cast<double> (it -> second); 
        }

        if(var_type == "Bool")
        {
            auto it = boolmap.find(cur_line[2]);
            val = static_cast<double> (it -> second); 
        } 

        if(var_type == "Float")
        {
            auto it = floatmap.find(cur_line[2]);
            val = static_cast<double> (it -> second); 
        }

        if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[2]);
            val = static_cast<double> (it -> second); 
        }
    }

    else //case1: a += 'c' ;  case2: a += 90.829 ;
    {
        //a += 'c' ;
        if(cur_line[2].size() == 3 && cur_line[2][0] == '\'' && cur_line[2][2] == '\'')
        {
            val = static_cast<double>(cur_line[2][1]);
        }

        if(val == -10000)
        {
            try
            {
                val = static_cast<double>(std::stod(cur_line[2]));
            }

            catch(const std::exception& e)
            {
                error_double.invalid_assignment(line_number);
                exit(-1);
            }
            
        }
    }

    if(operation == "+=")
    {
        compound_add_double(cur_line ,val);
    }

    if(operation == "-=")
    {
        compound_sub_double(cur_line ,val);
    }

    if(operation == "*=")
    {
        compound_mul_double(cur_line, val);
    }

    if(operation == "/=")
    {
        compound_div1_double(cur_line , val);
    }

    if(operation == "%=")
    {
        error_double.invalid_op(line_number);
        exit(-1);
    }
}

void compound_add_double(std::vector<std::string> cur_line, double operand)
{
    doublemap[cur_line[0]] += operand;
}

void compound_sub_double(std::vector<std::string> cur_line, double operand)
{
    doublemap[cur_line[0]] -= operand;
}

void compound_mul_double(std::vector<std::string> cur_line, double operand)
{
    doublemap[cur_line[0]] *= operand;
}

void compound_div1_double(std::vector<std::string> cur_line, double operand)
{
    doublemap[cur_line[0]] /= operand;
}

void add_double(std::vector<std::string> cur_line , double operand1 , double operand2)
{
    doublemap[cur_line[0]] = operand1 + operand2;
}

void sub_double(std::vector<std::string> cur_line , double operand1 , double operand2)
{
    doublemap[cur_line[0]] = operand1 - operand2;
}

void mul_double(std::vector<std::string> cur_line , double operand1 , double operand2)
{
    doublemap[cur_line[0]] = operand1 * operand2;
}

void div1_double(std::vector<std::string> cur_line , double operand1 , double operand2)
{
    doublemap[cur_line[0]] = operand1 / operand2;
}

void operation_choice_double(std::vector<std::string> cur_line ,std::string operation, double operand1 , double operand2 , int line_number)
{
    if(operation == "+")
    {
        add_double(cur_line,operand1,operand2);
        return;
    }

    if(operation == "-")
    {
        sub_double(cur_line,operand1,operand2);
        return;
    }

    if(operation == "*")
    {
        mul_double(cur_line,operand1,operand2);
        return;
    }

    if(operation == "/")
    {
        div1_double(cur_line,operand1,operand2);
        return;
    }

    if(operation == "%")
    {
        error_double.invalid_op(line_number);
        exit(-1);
    }
}

int return_double(std::string var_type , std::string name , int line_number)
{
    if(var_type == "String")
    {
        error_double.invalid_assignment(line_number);
        exit(-1);
    }

    if(var_type == "Double")
    {
        auto it = doublemap.find(name);
        return static_cast<double>(it -> second);
    }

    if(var_type == "Int")
    {
        auto it = intmap.find(name);
        return static_cast<double>(it -> second);
    }

    if(var_type == "Float")
    {
        auto it = floatmap.find(name);
        return static_cast<double>(it -> second);
    }

    if(var_type == "Bool")
    {
        auto it = boolmap.find(name);
        return static_cast<double>(it -> second);
    }

    if(var_type == "Char")
    {
        auto it = charmap.find(name);
        return static_cast<double>(it -> second);
    }
    return 0;
}