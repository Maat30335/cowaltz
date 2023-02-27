#ifndef USEFUL_H
#define USEFUL_H

#include <stdexcept>
#include <string>

void Error(std::string s){
    throw std::runtime_error(s);
}

#endif
