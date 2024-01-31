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
