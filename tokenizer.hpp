#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "error_messages.hpp"

Error error_tok;
bool iostream_usage = true;

std::vector<std::string> splitString(const std::string& input, char delimiter) 
{
    std::vector<std::string> parts;
    std::istringstream stream(input);
    std::string part;

    while (std::getline(stream, part, delimiter)) 
    {
        if (!part.empty()) 
        {
            parts.push_back(part);
        }
    }
    return parts;
}


std::vector<std::vector<std::string>> check()
{
    std::ifstream code_file;
    code_file.open("code.txt");

    if(!code_file.is_open())
    {
        error_tok.file_isnt_open();
        return {{}};
    }

    std::vector<std::string> tokens;
    std::string line;
    std::vector<std::vector<std::string>> allLines;
    bool main_appearance = false;
    int line_number = 1;

    while(!code_file.eof())
    {
        std::getline(code_file,line);
        tokens = splitString(line,' ');
        if(line_number == 1 && (tokens.size() != 2 || tokens[0] != "#include" || tokens[1] != "<iostream>"))
        {
            iostream_usage = false;
        }
        
        if(tokens.size() == 3 && tokens[0] == "int" && tokens[1] == "main()" && tokens[2] == "{")
        {
            if(main_appearance == true)                     
            {
                error_tok.only_one_main();
                exit(-1);    
            }
            main_appearance = true;
        }

        line_number++;
        allLines.push_back(tokens);
    }

    if(main_appearance == true)
    {
        if(allLines[allLines.size() - 1][0] != "}" || allLines[allLines.size() - 1].size() != 1)
        {
            error_tok.no_main();
            return {{}};
        }
        return allLines;
    }

    else
    {
        error_tok.no_main();
        return {{}};
    }
}