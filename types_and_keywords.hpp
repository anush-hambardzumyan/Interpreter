#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

std::vector<std::string> types = {"Bool","Char","Int","Float","Double","String"};
std::map<std::string,int> intmap;
std::map<std::string,double> doublemap;
std::map<std::string,float> floatmap;
std::map<std::string,bool> boolmap;
std::map<std::string,char> charmap;
std::map<std::string,std::string> stringmap;

std::vector<std::string> arithmetical_ops_unary = {"++" , "--" , "+=" , "-=" , "*=" , "/="};
std::vector<std::string> arithmetical_ops_binary = {"+" , "-" , "*" , "/" };
std::vector<std::string> comparison_ops = {"<" , "<=" , ">" , ">=" , "==" , "!="};

std::vector<std::pair<std::string,std::string>> allvars; //type and var name
std::vector<std::string> allkeywords
{"Bool", "Char", "Int", "Float", "Double", "String", "if", "while", "std::cout<<", "std::cin>>" , "return", "break" , "continue" , "main"};



