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
    std::cerr << "Error: file isn't opened: " << std::endl;
}

void Error::only_one_main()
{
    std::cerr << "Error: program can have only one main function: " << std::endl;
}

void Error::main_is_keyword()
{
    std::cerr<<"Error: main in keyword: " << std::endl;
}

void Error::no_main()
{
    std::cerr << "Error: undefined reference to main: " << std::endl;
}

void Error::invalid_op(int line)
{
    std::cerr<<"Error: invalid operation on line " << line << " : " << std::endl;
}

void Error::redeclaration(int line , std::string var)
{
    std::cerr<< "Error: redeclaration of variable " << var << " on line " << line << " :" << std::endl; 
}

void Error::was_not_dec(std::string var)
{
    std::cerr<<"Error: variable " << var << " was not declared: " << std::endl;
}

void Error::invalid_assignment(int line)
{
    std::cerr << "Error: invalid assignment on line " << line << ": " << std::endl;
}

void Error::type_incompatiblity(int line)
{
    std::cerr << "Error: type incompatibility on line " << line << ": " << std::endl;
}

void Error::is_keyword(std::string name)
{
    std::cerr<<"Error: "<< name <<" is keyword: " << std::endl;
}

void Error::missing_semicolon(int line)
{
    std::cerr << "Error: missing ';' in line " << line << ": " << std::endl;
}