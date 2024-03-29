#pragma once
#include <string>
#include <vector>
#include <map>
#include <list>

std::vector<std::string> types = {"Bool","Char","Int","Float","Double","String"};

//name of variable and value
std::map<std::string,int> intmap;
std::map<std::string,double> doublemap;
std::map<std::string,float> floatmap;
std::map<std::string,bool> boolmap;
std::map<std::string,char> charmap;
std::map<std::string,std::string> stringmap;

//name of array and the contents of array
std::vector<std::pair<std::string, std::list<int>>> intarrmap; 
std::vector<std::pair<std::string, std::list<double>>> doublearrmap;
std::vector<std::pair<std::string, std::list<float>>> floatarrmap;
std::vector<std::pair<std::string, std::list<bool>>> boolarrmap;
std::vector<std::pair<std::string, std::list<char>>> chararrmap;


std::vector<std::string> inc_dec = {"++" , "--" };
std::vector<std::string> comp_assignments = {"+=" , "-=" , "*=" , "/=" , "%="};
std::vector<std::string> arithmetical_ops_binary = {"+" , "-" , "*" , "/" , "%"};
std::vector<std::string> comparison_ops = {"<" , "<=" , ">" , ">=" , "==" , "!="};

std::vector<std::pair<std::string,std::string>> allvars; //type and var name
std::vector<std::pair<std::string,std::string>> allarrays; //type and arr name
std::vector<std::string> allkeywords
{"Bool", "bool", "Char", "char" , "Int", "int" , "Float", "float" ,"Double", "double", "String", "string", 
"If", "if" , "While", "while ", "std::cout", "std::cin" ,"std::endl", "return", "break" , "continue" , "main"};



