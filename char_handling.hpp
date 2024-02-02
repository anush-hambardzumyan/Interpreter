#pragma once
#include <utility>
#include <algorithm>
#include "parser.hpp"
#include "types_and_keywords.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"

Error error_char;
void char_var_dec(std::vector<std::string> cur_line,int line_number);
void char_casts_init(std::vector<std::string> cur_line,int line_number ,std::string var_type);
void char_var_assign(std::vector<std::string> cur_line,int line_number);
void char_casts_assign(std::vector<std::string> cur_line, int line_number, std::string var_type);
void char_casts_comp_assign(std::vector<std::string> cur_line , int line_number , std::string var_type , std::string operation);
void add_char(std::vector<std::string> cur_line , char operand1,char operand2);
void sub_char(std::vector<std::string> cur_line , char operand1,char operand2);
void mul_char(std::vector<std::string> cur_line , char operand1,char operand2);
void div1_char(std::vector<std::string> cur_line , char operand1,char operand2);
void div2_char(std::vector<std::string> cur_line , char operand1,char operand2);
void operation_choice_char(std::vector<std::string> cur_line ,std::string operation, char operand1,char operand2 , int line_number);
void compound_add_char(std::vector<std::string>cur_line, char operand, int line_number);
void compound_sub_char(std::vector<std::string>cur_line, char operand, int line_number);
void compound_mul_char(std::vector<std::string>cur_line, char operand, int line_number);
void compound_div1_char(std::vector<std::string>cur_line, char operand, int line_number);
void compound_div2_char(std::vector<std::string> cur_line, char operand, int line_number);
char return_char(std::string var_type , std::string name , int line_number);


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

void char_var_assign(std::vector<std::string> cur_line, int line_number)
{
    if(cur_line[cur_line.size() - 1] != ";")
    {
        error_char.missing_semicolon(line_number);
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
            if(charmap[cur_line[0]] + 1 > 127)
            {
                error_char.invalid_op(line_number);
                exit(-1);
            }
            charmap[cur_line[0]] += 1;
        }

        else if(cur_line[1] == "--") //a -- ;
        {
            if(charmap[cur_line[0]] - 1 < 0)
            {
                error_char.invalid_op(line_number);
                exit(-1);
            }
            charmap[cur_line[0]] -= 1;
        }

        else
        {
            error_char.invalid_assignment(line_number);
            exit(-1);
        }
    }

    if(cur_line.size() == 4) //it should be ordinary assignment or compound assignment
    {
        auto it = std::find(allkeywords.begin() , allkeywords.end() ,cur_line[2]);
        if(it != allkeywords.end())
        {
            error_char.is_keyword(cur_line[2]);   // a = break ;
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
                    char_casts_assign(cur_line,line_number,predicted_type);
                }
            }

            if(predicted_type == "")
            {               
                try                 
                {                                           
                    int value = std::stoi(cur_line[2]);   
                    if(value >= 0 && value <= 127)  
                    {
                        charmap[cur_line[0]] = value; 
                        return;
                    }                    
                    else
                    {
                        error_char.invalid_op(line_number);
                        exit(-1);
                    }
                } 

                catch (const std::invalid_argument& e) 
                {
                    if(cur_line[2].size() == 3 && cur_line[2][0] == '\'' && cur_line[2][2] == '\'')
                    {
                        int val = static_cast<int> (cur_line[2][1]);
                        if(val >= 0 && val <= 127)
                        {
                            charmap[cur_line[0]] = (cur_line[2][1]);  //a = 'A' ;
                            return;
                        }
                        else
                        {
                            error_char.invalid_op(line_number);
                            exit(-1);
                        }
                    }

                    if(cur_line[2][0] == '\"' && cur_line[2][(cur_line[2].size()) - 1] == '\"')
                    {
                        error_char.invalid_assignment(line_number);   //a = "bckdjs" ;
                        exit(-1);
                    }
                    
                    error_char.was_not_dec(cur_line[2]);
                    exit(-1);
                }
            }
        }  


        else //this means it's compound assingnment
        {
            auto it = std::find(comp_assignments.begin(),comp_assignments.end() , cur_line[1]);
            if(it == comp_assignments.end())
            {
                error_char.invalid_assignment(line_number);
                exit(-1);
            }

            else    // a += b ;
            {
                std::string type = var_check(cur_line[2]);
                char_casts_comp_assign(cur_line,line_number,type,cur_line[1]);
            }
        } 
    }  

    if(cur_line.size() == 6) // assigns an expression
    {
        auto it1 = std::find(allkeywords.begin() , allkeywords.end() , cur_line[2]);
        auto it2 = std::find(allkeywords.begin() , allkeywords.end() , cur_line[4]);
        if(it1 != allkeywords.end())
        {
            error_char.is_keyword(cur_line[2]);
            exit(-1);
        }
        if(it2 != allkeywords.end())
        {
            error_char.is_keyword(cur_line[4]);
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
            error_char.invalid_assignment(line_number);
            exit(-1);
        }

        //case Char a,b .. ; c = a + b ; (or a and b can be another types except string)
        //flags are for checking if the operands are variables or literals
        //if in the end of checking value of any of flags stays false, assignment is invalid

        int val1 = -1000000;
        for(int i = 0; i < allvars.size() ; ++i)
        {
            if(cur_line[2] == allvars[i].second)
            {
                val1 = return_char(allvars[i].first , allvars[i].second ,line_number);
            }
        }

        int val2 = -1000000;
        for(int i = 0; i < allvars.size(); ++i)
        {
            if(cur_line[4] == allvars[i].second)
            {
                val2 = return_char(allvars[i].first , allvars[i].second , line_number);
            }
        }

        if(val1 == -1000000)
        {
            try
            {
                val1 = static_cast<char>(std::stoi(cur_line[2]));    
            }

            catch(const std::exception& e)
            {
                if(cur_line[2][0] == '\"')  // Char A ;    A = "ib" + ..
                {
                    error_char.invalid_assignment(line_number);
                    exit(-1);
                }

                // Char A ;    A = 'c' + ...
                else if(cur_line[2].size() && cur_line[2][0] == '\'' && cur_line[2][2] == '\'')
                {
                    val1 = static_cast<char>(cur_line[2][1]);
                }

                else
                {
                    error_char.invalid_assignment(line_number);
                    exit(-1);
                }
            }
        }

        if(val2 == -1000000)
        {
            try
            {
                val2 = static_cast<char>(std::stoi(cur_line[4]));    
            }

            catch(const std::exception& e)
            {
                if(cur_line[4][0] == '\"')  // Char A ;    A = "ib" + ..
                {
                    error_char.invalid_assignment(line_number);
                    exit(-1);
                }

                // Char A ;    A = 'c' + ...
                else if(cur_line[4].size() && cur_line[4][0] == '\'' && cur_line[4][2] == '\'')
                {
                    val2 = static_cast<char>(cur_line[4][1]);
                }

                else
                {
                    error_char.invalid_assignment(line_number);
                    exit(-1);
                }
            }
        }

        if(val1 != -1000000  && val2 != -1000000)
        {
            operation_choice_char(cur_line , operation, val1 , val2 , line_number);
            return;
        }

        else
        {
            error_char.invalid_assignment(line_number);
        }
    }  
}

void operation_choice_char(std::vector<std::string> cur_line ,std::string operation, char operand1,char operand2 , int line_number)
{
    if(operation == "+")
    {
        if((int(operand1) + int(operand2)) > 127 || (int(operand1) + int(operand2)) < 0)
        {
            error_char.invalid_op(line_number);
            exit(-1);
        }

        add_char(cur_line,operand1,operand2);
        return;
    }

    if(operation == "-")
    {
        if((int(operand1) - int(operand2)) > 127 || (int(operand1) - int(operand2)) < 0)
        {
            error_char.invalid_op(line_number);
            exit(-1);
        }
        sub_char(cur_line,operand1,operand2);
        return;
    }

    if(operation == "*")
    {
        if((int(operand1) * int(operand2)) > 127 || (int(operand1) * int(operand2)) < 0)
        {
            error_char.invalid_op(line_number);
            exit(-1);
        }
        mul_char(cur_line,operand1,operand2);
        return;
    }

    if(operation == "/")
    {
        if((int(operand1) / int(operand2)) > 127 || (int(operand1) / int(operand2)) < 0)
        {
            error_char.invalid_op(line_number);
            exit(-1);
        }
        div1_char(cur_line,operand1,operand2);
        return;
    }

    if(operation == "%")
    {
        if((int(operand1) % int(operand2)) > 127 || (int(operand1) % int(operand2)) < 0)
        {
            error_char.invalid_op(line_number);
            exit(-1);
        }

        div2_char(cur_line,operand1,operand2);
        return;
    }
}

void add_char(std::vector<std::string> cur_line , char operand1,char operand2)
{
    charmap[cur_line[0]] = operand1 + operand2;
}

void sub_char(std::vector<std::string> cur_line , char operand1,char operand2)
{
    charmap[cur_line[0]] = operand1 - operand2;
}

void mul_char(std::vector<std::string> cur_line , char operand1,char operand2)
{
    charmap[cur_line[0]] = operand1 * operand2;
}

void div1_char(std::vector<std::string> cur_line , char operand1,char operand2)
{
    charmap[cur_line[0]] = operand1 / operand2;
}

void div2_char(std::vector<std::string> cur_line , char operand1,char operand2)
{
    charmap[cur_line[0]] = operand1 % operand2;
}

char return_char(std::string var_type , std::string name , int line_number)
{
    if(var_type == "String" || var_type == "Double" || var_type == "Float" || var_type == "Bool")
    {
        error_char.invalid_assignment(line_number);
        exit(-1);
    }

    if(var_type == "Char")
    {
        auto it = charmap.find(name);
        return it -> second;
    }

    if(var_type == "Int")
    {
        auto it = intmap.find(name);
        return it -> second;
    }

    return 0;
}

void char_casts_assign(std::vector<std::string> cur_line , int line_number , std::string var_type)
{
    if(var_type == "")
    {
        return ;
    }

    if(var_type == "String" || var_type == "Double" || var_type == "Double" || var_type == "Bool")            
    {
        error_char.type_incompatiblity(line_number);
        exit(-1);
    }

    else
    {
        if(var_type == "Int")
        {
            auto it = intmap.find(cur_line[2]);
            if(it -> second < 0 || it -> second > 127)
            {
                error_char.invalid_op(line_number);
            }
            charmap[cur_line[0]] = it -> second;
            return;
        }

        else if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[2]);
            charmap[cur_line[0]] = it -> second;
            return;
        }
    }
}

void compound_add_char(std::vector<std::string> cur_line,char operand, int line_number)
{
    if(int(charmap[cur_line[0]]) + int(operand) < 0 || int(charmap[cur_line[0]]) + int(operand) > 127)
    {
        error_char.invalid_op(line_number);
        exit(-1);
    }
    charmap[cur_line[0]] += operand;
}

void compound_sub_char(std::vector<std::string> cur_line,char operand, int line_number)
{
    if(int(charmap[cur_line[0]]) - int(operand) < 0 || int(charmap[cur_line[0]]) - int(operand) > 127)
    {
        error_char.invalid_op(line_number);
        exit(-1);
    }
    charmap[cur_line[0]] -= operand;
}

void compound_mul_char(std::vector<std::string> cur_line,char operand,int line_number)
{
    if(int(charmap[cur_line[0]]) * int(operand) < 0 || int(charmap[cur_line[0]]) * int(operand) > 127)
    {
        error_char.invalid_op(line_number);
        exit(-1);
    }
    charmap[cur_line[0]] *= operand;
}

void compound_div1_char(std::vector<std::string> cur_line,char operand, int line_number)
{
    if(int(charmap[cur_line[0]]) / int(operand) < 0 || int(charmap[cur_line[0]]) / int(operand) > 127)
    {
        error_char.invalid_op(line_number);
        exit(-1);
    }
    charmap[cur_line[0]] /= operand;
}

void compound_div2_char(std::vector<std::string> cur_line,char operand, int line_number)
{
    if(int(charmap[cur_line[0]]) % int(operand) < 0 || int(charmap[cur_line[0]]) % int(operand) > 127)
    {
        error_char.invalid_op(line_number);
        exit(-1);
    }
    charmap[cur_line[0]] %= operand;
}

void char_casts_comp_assign(std::vector<std::string> cur_line , int line_number , std::string var_type , std::string operation)
{
    int val = -10000 ;

    if(var_type != "")
    {
        if(var_type == "String" || var_type == "Bool" ||var_type == "Double" || var_type == "Float")
        {
            error_char.type_incompatiblity(line_number);
            exit(-1);
        }

        if(var_type == "Int")
        {
            auto it = intmap.find(cur_line[2]);
            val = static_cast<int> (it -> second); 
        }

        if(var_type == "Char")
        {
            auto it = charmap.find(cur_line[2]);
            val = static_cast<char> (it -> second); 
        }
    }

    else //case1: a += 'c' ;  case2: a += 90 ;
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
                val = static_cast<int>(std::stoi(cur_line[2]));
                if(val < 0 || val > 127)
                {
                    error_char.invalid_op(line_number);
                    exit(-1);
                }
            }

            catch(const std::exception& e)
            {
                error_char.invalid_assignment(line_number);
                exit(-1);
            }
            
        }
    }

    if(operation == "+=")
    {
        compound_add_char(cur_line ,val , line_number);
    }

    if(operation == "-=")
    {
        compound_sub_char(cur_line ,val,line_number);
    }

    if(operation == "*=")
    {
        compound_mul_char(cur_line, val , line_number);
    }

    if(operation == "/=")
    {
        compound_div1_char(cur_line , val, line_number);
    }

    if(operation == "%=")
    {
        compound_div2_char(cur_line , val,line_number);
    }
}