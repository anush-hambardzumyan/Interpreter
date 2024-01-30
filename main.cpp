#include <iostream>
#include <stack>
#include <map>
#include <utility>
#include <algorithm>
//#include "types.hpp"
#include "tokenizer.hpp"
#include "error_messages.hpp"
#include "interpreter.hpp"


int main()
{   
    std::vector<std::vector<std::string>> TokStream = check();
    if(TokStream.size() == 0)
    {
        return 0;
    }

    else
    {
        interpreter(TokStream);
    }
}
