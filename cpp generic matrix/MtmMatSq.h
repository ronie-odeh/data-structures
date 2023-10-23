#ifndef EX3_MTMMATREC_H
#define EX3_MTMMATREC_H

#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMat.h"

using std::size_t;

namespace MtmMath {

    template <typename T>
    class MtmMatSq : public MtmMat<T> {
    public:
        /*
         * Rectangular Matrix constructor, m is the number of rows and columns in the matrix
         * and val is the initial value for the matrix elements
         */
        explicit MtmMatSq (size_t m, const T& val=T()):MtmMat<T>(Dimensions(m,m),val){}

        /*
         * an override for the reshape function in MtmMat that always throws an exception, because you cannot
         * reshape a square matrix.
         */

        void reshape(Dimensions dim){
            Dimensions curr_dim(this->getDim());
            throw MtmExceptions::ChangeMatFail(curr_dim,dim);
        }

        /*
         *an override for the resize function in MtmMat, checks the given dimension if its not nxn it throws an exception
         * if its gonna resize the matrix to a new square matrix it uses the resize function in MtmMat and resizes the
         * matrix.
         */
        virtual void resize(Dimensions dim, const T& val=T()){
            if(dim.getRow()!=dim.getCol()){
                Dimensions curr_dim(this->getDim());
                throw MtmExceptions::ChangeMatFail(curr_dim,dim);
            }
            MtmMat<T>::resize(dim,val);
        }



        MtmMatSq(const MtmMat<T>& mat):MtmMatSq(mat.getDim().getCol()) {
            if (mat.getDim().getCol() != mat.getDim().getRow())
                throw MtmExceptions::IllegalInitialization();
            for (size_t i = 0; i < mat.getDim().getRow(); ++i) {
                for (size_t j = 0; j < mat.getDim().getCol(); ++j) {
                    this->operator[](i)[j]=mat[i][j];
                }
            }
        }






    };

}

#endif //EX3_MTMMATREC_H
