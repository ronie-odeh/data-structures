#ifndef EX3_MTMEXCEPTIONS_H
#define EX3_MTMEXCEPTIONS_H

#include <exception>
#include <string>
#include <iostream>
#include "Auxilaries.h"

namespace MtmMath {
    namespace MtmExceptions {
        class MtmExceptions : public std::exception {
        public:
            virtual ~MtmExceptions() throw() {}
        };

        /*
         * Exception for illegal initialization of an object, needs to output
         * "MtmError: Illegal initialization values" in what() class function
         */
        class IllegalInitialization : public MtmExceptions {
        public:
                const char* what() const throw() override;
        };

        /*
         * Exception for Memory allocation failure for an object, needs to output
         * "MtmError: Out of memory" in what() class function
         */
        class OutOfMemory : public MtmExceptions {
        public:
            const char* what() const throw() override;
        };

        /*
         * Exception for dimension mismatch during a mathematical function, needs to output
         * "MtmError: Dimension mismatch: (<mat 1 row>,<mat 1 col>) (<mat 2 row>,<mat 2 col>)"
         * in what() class function
         */
        class DimensionMismatch : public MtmExceptions {
        Dimensions dimension1;
        Dimensions dimension2;
        std::string str;
        public:
            DimensionMismatch(Dimensions& first_dimension,Dimensions& second_dimension):
            dimension1(first_dimension),dimension2(second_dimension),
            str("MtmError: Dimension mismatch: "+dimension1.to_string()+" "+dimension2.to_string()){}
            const char* what() const throw() override;
        };

        /*
         * Exception for error for changing matrix/vector shape in reshape and resize, needs to output
         * "MtmError: Change matrix shape failed from: (<mat row>,<mat col>) (<new mat row>,<new mat col>)"
         * in what() class function
         */
        class ChangeMatFail : public MtmExceptions {
        Dimensions current_dimension;
        Dimensions new_dimension;
        std::string str;
        public:
            ChangeMatFail(Dimensions& current_D,Dimensions& new_D):
            current_dimension(current_D),new_dimension(new_D),
            str("MtmError: Change matrix shape failed from: "+
                current_dimension.to_string()+" "+new_dimension.to_string()){}
            const char* what() const throw() override;
        };

        /*
         * Exception for accessing an illegal element in matrix or vector, needs to output
         * "MtmError: Attempt access to illegal element" in what() class function
         */
        class AccessIllegalElement : public MtmExceptions {
        public:
            const char* what() const throw() override;

        };
    }
}


#endif //EX3_MTMEXCEPTIONS_H
