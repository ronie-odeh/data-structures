#ifndef EX3_MTMMATTRIAG_H
#define EX3_MTMMATTRIAG_H


#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMatSq.h"

using std::size_t;

namespace MtmMath {


    template <typename T>
    class MtmMatTriag : public MtmMatSq<T>{
        bool isUpper;
    public:

        /*
         * Triangular Matrix constructor, m is the number of rows and columns in the matrix,
         * val is the initial value for the matrix elements and isUpper_ is whether it is upper
         * Rectangular matrix (true means it is)
         */
        MtmMatTriag (size_t m, const T& val=T(), bool isUpper_t=true):MtmMatSq<T>(m,T(0)),isUpper(isUpper_t){
            for (size_t i = 0; i < m; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    if (isUpper_t){
                        if(j>=i)
                            this->operator[](i)[j]=val;
                    } else{
                        if(i>=j)
                            this->operator[](i)[j]=val;
                    }
                }
            }
        }


        MtmMatTriag(const MtmMat<T>& mat1):MtmMatTriag(mat1.getDim().getRow(),T(0)){
            bool isUpper=true,isLower=true;
            for (size_t i = 0; i < mat1.getDim().getRow(); ++i) {
                for (size_t j = 0; j < mat1.getDim().getCol(); ++j) {
                    if(j>i){
                        if(mat1[i][j]!=T(0))
                            isLower=false;
                    }
                    if(i>j){
                        if(mat1[i][j]!=T(0))
                            isUpper=false;
                    }
                }
            }
            if(!isLower&&!isUpper)
                throw MtmExceptions::IllegalInitialization();
            this->isUpper=isUpper;
            if(isUpper) {
                for (size_t i = 0; i < mat1.getDim().getRow(); ++i) {
                    for (size_t j = 0; j < mat1.getDim().getCol(); ++j){
                        if(j>=i)
                            this->operator[](i)[j]=mat1[i][j];
                    }
                }
            }
            if(isLower) {
                for (size_t i = 0; i < mat1.getDim().getRow(); ++i) {
                    for (size_t j = 0; j < mat1.getDim().getCol(); ++j){
                        if(i>=j)
                            this->operator[](i)[j]=mat1[i][j];
                    }
                }
            }
        }

        void transpose(){
            MtmMat<T> tmp(*this);
            Dimensions tmp_dim(this->getDim());
            for (size_t k = 0; k < this->getDim().getRow(); ++k) {
                for (size_t i = 0; i < this->getDim().getCol(); ++i) {
                    if(isUpper)
                    {
                        if(i>=k)
                            this->operator[](k)[i]=0;
                    }
                    else{
                        if(k>=i)
                            this->operator[](k)[i]=0;
                    }
                }
            }
            isUpper=!isUpper;
            for (size_t i = 0; i < this->getDim().getRow(); ++i) {
                for (size_t j = 0; j < this->getDim().getCol(); ++j) {
                    if(isUpper){
                        if(j>=i)
                            this->operator[](i)[j]=tmp[j][i];
                    }
                    else{
                        if(i>=j)
                            this->operator[](i)[j]=tmp[j][i];
                    }

                }
            }
        }

        virtual void resize(Dimensions dim, const T& val=T()) override{
            if(dim.getRow()!=dim.getCol()){
                Dimensions curr_dim(this->getDim());
                throw MtmExceptions::ChangeMatFail(curr_dim,dim);
            }
            bool check=false;
            Dimensions old_dim(this->getDim());
            if(!isUpper){
                this->transpose();
                check=true;
            }
            MtmMath::MtmMatSq<T>::resize(dim,T(0));
            for (size_t i = 0; i < dim.getRow(); ++i) {
                for (size_t j = old_dim.getCol(); j < dim.getRow(); ++j) {
                        if(j>=i)
                            this->operator[](i)[j]=val;

                }
            }
            if(check){
                this->transpose();
            }
        }

        MtmVec<T>& operator[](size_t row_index){
            MtmMatSq<T>::operator[](row_index).get_row_index()=row_index;
            MtmMatSq<T>::operator[](row_index).get_is_triag()=true;
            MtmMatSq<T>::operator[](row_index).get_is_upper()=this->isUpper;

            return MtmMatSq<T>::operator[](row_index);
        }

        const MtmVec<T>& operator[](const size_t& row_index)const{
            return this->MtmMatSq<T>::operator[](row_index);
        }

        void print_mat(){
            for (size_t i = 0; i < this->getDim().getRow(); ++i) {
                for (size_t j = 0; j < this->getDim().getCol(); ++j) {
                    if(isUpper){
                        if(j>=i)
                            std::cout<<this->operator[](i)[j] <<" ";
                    }
                    else{
                        if(i>=j)
                            std::cout<<this->operator[](i)[j] <<" ";
                    }
                }
                std::cout<<std::endl;
            }
            std::cout<<std::endl;
        }



    };
}

#endif //EX3_MTMMATTRIAG_H
