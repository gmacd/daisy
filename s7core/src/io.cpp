#include "io.h"

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>

namespace s7 {

    std::string ReadAll(const std::string& filepath)
    {
        std::ifstream file(filepath);
        if (file.is_open())
        {
            // construct string from iterator range
            std::string content(
                (std::istreambuf_iterator<char>(file)),
                (std::istreambuf_iterator<char>()));
            
            return content;
        }
        else 
        {
            return "";
        }
    }
}
