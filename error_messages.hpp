/*Error class was made for collecting all 
the error messeges under one class*/
#pragma once
#include <iostream>
#include <string>
 
struct Error
{
    void file_isnt_open();
    void only_one_main();
    void main_is_keyword();
    void no_main();
    void invalid_op(int line);  
    void redeclaration(int line , std::string var);
    void was_not_dec(std::string var);
    void type_incompatiblity(int line);
    void invalid_assignment(int line);
    void is_keyword(std::string name);
    void missing_semicolon(int line);
};

void Error::file_isnt_open()
{
    std::cerr << "\033[1;31m Error: file isn't opened:  \033[0m" << std::endl; 
}

void Error::only_one_main()
{
    std::cerr << "\033[1;31m Error: program can have only one main function: \033[0m" << std::endl;
}

void Error::main_is_keyword()
{
    std::cerr<<"\033[1;31m Error: main is keyword: \033[0m" << std::endl;
}

void Error::no_main()
{
    std::cerr << "\033[1;31m  Error: undefined reference to main: \033[0m" << std::endl;
}

void Error::invalid_op(int line)
{
    std::cerr<< "\033[1;31m Error: invalid operation on line \033[0m" << line << " : " << std::endl;
}

void Error::redeclaration(int line , std::string var)
{
    std::cerr<< "\033[1;31m Error: redeclaration of variable \033[0m" << var << "\033[1;31m on line \033[0m" << line << "\033[1;31m : \033[0m" << std::endl; 
}

void Error::was_not_dec(std::string var)
{
    std::cerr<<"\033[1;31m Error: variable \033[0m" << var << "\033[1;31m was not declared: \033[0m" << std::endl;
}

void Error::invalid_assignment(int line)
{
    std::cerr << "\033[1;31m Error: invalid assignment on line \033[0m" << line << "\033[1;31m : \033[0m" << std::endl;
}

void Error::type_incompatiblity(int line)
{
    std::cerr << "\033[1;31m Error: type incompatibility on line \033[0m" << line << "\033[1;31m : \033[0m" << std::endl;
}

void Error::is_keyword(std::string name)
{
    std::cerr<<"\033[1;31m Error: "<< name <<"\033[1;31m is keyword: \033[0m" << std::endl;
}

void Error::missing_semicolon(int line)
{
    std::cerr << "\033[1;31m Error: missing ';' in line \033[0m" << line << "\033[1;31m : \033[0m" << std::endl;
}