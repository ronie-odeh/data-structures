#include "MtmExceptions.h"

#include <iostream>
using namespace MtmMath::MtmExceptions;

const char* IllegalInitialization::what() const throw(){
    return "MtmError: Illegal initialization values";
}

const char* OutOfMemory::what() const throw(){
    return "MtmError: Out of memory";
}

const char* DimensionMismatch::what() const throw(){
    return str.c_str();
}

const char* ChangeMatFail::what() const throw(){
    return str.c_str();
}


const char* AccessIllegalElement::what() const throw(){
    return "MtmError: Attempt access to illegal element";
}