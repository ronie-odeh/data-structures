#ifndef EX3_MTMMAT_H
#define EX3_MTMMAT_H


#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmVec.h"

using std::size_t;

namespace MtmMath {

    template <typename T>
    class MtmMat {
        Dimensions dim;
        MtmVec<MtmVec<T>> matrix;
    public:


    size_t Rows()const {
        size_t tmp=this->dim.getRow();
        return tmp;
    }

    size_t Columns()const{
        size_t tmp=this->dim.getCol();
        return tmp;
    }
        /*
         * Matrix constructor, dim_t is the dimension of the matrix and val is the initial value for the matrix elements
         */
        explicit MtmMat<T>(Dimensions dim_t, const T& val=T())try:
        dim(dim_t),matrix(dim_t.getRow(),MtmVec<T>(dim_t.getCol(),val)){
            if(dim.getCol()<=0||dim.getRow()<=0)
                throw MtmExceptions::IllegalInitialization();

        }
        catch (const std::bad_alloc& e){
            throw MtmExceptions::OutOfMemory();
        }

        MtmMat(const MtmMat& mat)try:dim(mat.dim),matrix(mat.dim.getRow(),MtmVec<T>(mat.dim.getCol(),1)){
            for (size_t i = 0; i < dim.getRow(); ++i) {
                for (size_t j = 0; j < dim.getCol(); ++j) {
                    matrix[i][j]=mat.matrix[i][j];
                }
            }
        }
        catch (const std::bad_alloc& e){
            throw MtmExceptions::OutOfMemory();
        }
        ~MtmMat()= default;
        /*
         * Function that get function object f and uses it's () operator on each element in the matrix columns.
         * It outputs a vector in the size of the matrix columns where each element is the final output
         * by the function object's * operator
         */
        template <typename Func>
        MtmVec<T> matFunc(Func& f) const{
            MtmVec<T> vec(this->getDim().getCol());
            for (size_t i = 0; i < dim.getCol(); ++i) {
                Func tmp=f;
                for (size_t j = 0; j < dim.getRow(); ++j) {
                    tmp(this->matrix[j][i]);
                }
                vec[i]=*tmp;
            }
            return vec;
        }

        /*
         * resizes a matrix to dimension dim, new elements gets the value val.
         */
        virtual void resize(Dimensions dim, const T& val=T()){
            Dimensions d1(dim.getRow(),1);
            Dimensions d2(dim.getCol(),1);
            matrix.resize(d1,MtmVec<T>(dim.getCol(),val));
            for (size_t i = 0; i < dim.getRow(); ++i) {
                matrix[i].MtmVec<T>::resize(d2,val);
            }
            this->dim=dim;
        }

        /*
         * reshapes matrix so linear elements value are the same without changing num of elements.
         */

        virtual void reshape(Dimensions newDim){
            Dimensions tmp_dim(dim);
            if (dim.getRow() * dim.getCol() != newDim.getRow() * newDim.getCol()) {
                throw MtmExceptions::ChangeMatFail(dim,newDim);
            }
            MtmMat<T> tmp(*this);
            this->resize(newDim);
            size_t i = 0, j = 0;
            for (size_t col = 0; col < tmp_dim.getCol();col++) {
                for (size_t row = 0; row < tmp_dim.getRow();row++) {
                    matrix[i][j] = tmp.matrix[row][col];
                    i++;
                    if (i >= newDim.getRow()) {
                        j ++;
                        i = 0;
                    }
                }
            }
        }


        /*
         * Performs transpose operation on matrix
         */
        virtual void transpose(){
            MtmMat<T> tmp(*this);
            Dimensions tmp_dim(dim);
            tmp_dim.transpose();
            this->resize(tmp_dim);
            for (size_t i = 0; i < this->dim.getRow(); ++i) {
                for (size_t j = 0; j < this->dim.getCol(); ++j) {
                    this->matrix[i][j]=tmp.matrix[j][i];
                }
            }
        }


        virtual MtmVec<T>& operator[](const size_t& row_index){
            return matrix[row_index];
        }

        virtual const MtmVec<T>& operator[](const size_t& row_index)const{
            return matrix[row_index];
        }

        MtmMat<T> operator-()const{
            MtmMat<T> new_matrix(*this);
            for (size_t i = 0; i < dim.getRow(); ++i) {
                for (size_t j = 0; j < dim.getCol(); ++j) {
                    new_matrix.matrix[i][j]*=-1;
                }
            }
            return new_matrix;

        }

        MtmMat<T>& operator+=(const MtmMat<T>& mat){
            if(dim!=mat.dim){
                Dimensions tmp_dim(mat.dim);
                throw MtmExceptions::DimensionMismatch(dim,tmp_dim);
            }
            for (size_t i = 0; i < dim.getRow(); ++i) {
                for (size_t j = 0; j < dim.getCol(); ++j) {
                    this->matrix[i][j]+=mat.matrix[i][j];
                }
            }
            return *this;
        }

        MtmMat<T>& operator-=(const MtmMat<T>& mat){
            if(dim!=mat.dim){
                Dimensions tmp_dim(mat.dim);
                throw MtmExceptions::DimensionMismatch(dim,tmp_dim);
            }
            *this+=(-mat);
            return *this;
        }

        MtmMat<T>& operator*=(const T& t){
            for (size_t i = 0; i < dim.getRow(); ++i) {
                for (size_t j = 0; j < dim.getCol(); ++j) {
                    matrix[i][j]*=t;
                }
            }
            return *this;

        }
        class iterator{
        protected:
            friend class MtmMat<T>;
            friend class nonzero_iterator;
            MtmMat<T>* mtmMat;
            size_t row_index;
            size_t col_index;
        public:
            iterator(MtmMat<T>* mtmMat, size_t row_index, size_t col_index)
                    : mtmMat(mtmMat),row_index(row_index),col_index(col_index){}
            T& operator*(){
                return mtmMat->matrix[row_index][col_index];
            }

            virtual iterator& operator++(){
                if(++row_index==mtmMat->dim.getRow()){
                    if(++col_index==mtmMat->dim.getCol()){
                        return *this;}
                    else{
                        row_index=0;
                        return *this;
                    }
                }
                return *this;
            }

            bool operator==(const iterator& iterator1) const {
                return row_index == iterator1.row_index && col_index == iterator1.col_index;
            }

            bool operator!=(const iterator& iterator1) const {
                return !(*this == iterator1);
            }
        };

        class nonzero_iterator:public iterator {
        public:

            nonzero_iterator(MtmMat<T>* mtmMat, size_t row_index, size_t col_index)
                    :iterator(mtmMat,row_index,col_index){}

            virtual nonzero_iterator& operator++() override{
                ++this->row_index;
                if(this->row_index==this->mtmMat->dim.getRow()){
                        if(++this->col_index==this->mtmMat->dim.getCol()){
                            return *this;
                        }
                    this->row_index=0;
                }
                while(this->col_index!=this->mtmMat->dim.getCol()&&
                this->mtmMat->matrix[this->row_index][this->col_index]==T(0)){
                    if(++this->row_index==this->mtmMat->dim.getRow()){
                        if(++this->col_index==this->mtmMat->dim.getCol()){
                            return *this;
                        }
                        this->row_index=0;
                    }
                }
                return *this;
            }
        };

        iterator begin() {
            return iterator(this,0,0);
        }

        iterator end() {
            return iterator(this,this->dim.getRow(),this->dim.getCol());
        }

        virtual nonzero_iterator nzbegin(){
            size_t row=0,col=0;
            while(matrix[row][col]==T(0)&&col!=dim.getCol()&&row!=dim.getRow()){
                if(++row==dim.getRow()){
                    row=0;
                    ++col;
                }
            }
            return nonzero_iterator(this,row,col);

        }

        nonzero_iterator nzend(){
            return nonzero_iterator(this,this->dim.getRow(),this->dim.getCol());
        }


        virtual void print_mat(){
            for (size_t i = 0; i < dim.getRow(); ++i) {
                for (size_t j = 0; j < dim.getCol(); ++j) {
                    std::cout<<matrix[i][j] <<" ";
                }
                std::cout<<std::endl;
            }
            std::cout<<std::endl;
        }

        Dimensions getDim()const{
            Dimensions tmp=this->dim;
            return tmp;
        }

    };


    template <typename T>
    MtmMat<T> operator+(const MtmMat<T>& mat1,const MtmMat<T>& mat2){
        return MtmMat<T>(mat1)+=mat2;
    }

    template <typename T>
    MtmMat<T> operator+(const MtmMat<T>& mat1,const T& t){
        return MtmMat<T>(mat1.getDim(),t)+mat1;
    }

    template <typename T>
    MtmMat<T> operator+(const T& t,const MtmMat<T>& mat1){
        return mat1+t;
    }

    template <typename T>
    MtmMat<T> operator+(const MtmVec<T>& vec1,const MtmMat<T>& mat1){
        if(vec1.getVertical()) {
            if(vec1.getSize()!=mat1.getDim().getRow()||mat1.getDim().getCol()!=1){
                Dimensions d1(vec1.getSize(),1);
                Dimensions d2(mat1.getDim());
                throw MtmExceptions::DimensionMismatch(d2,d1);
            }
        }
        else {
                if(vec1.getSize()!=mat1.getDim().getCol()||mat1.getDim().getRow()!=1){
                    Dimensions d1(1,vec1.getSize());
                    Dimensions d2(mat1.getDim());
                    throw MtmExceptions::DimensionMismatch(d2,d1);
                }
            }
        MtmMat<T> tmp(mat1);
        typename MtmMat<T>::iterator iterator1 = tmp.begin();
        typename MtmMat<T>::iterator end_iterator = tmp.end();
        size_t i=0;
        while(iterator1!=end_iterator){
            *iterator1=vec1[i++];
            ++iterator1;
        }
        return mat1+tmp;
    }

    template <typename T>
    MtmMat<T> operator+(const MtmMat<T>& mat1,const MtmVec<T>& vec1){
        return vec1+mat1;
    }

    template <typename T>
    MtmMat<T> operator-(const MtmVec<T>& vec1,const MtmMat<T>& mat1){
        return vec1+(-mat1);
    }

    template <typename T>
    MtmMat<T> operator-(const MtmMat<T>& mat1,const MtmVec<T>& vec1){
        return mat1+(-vec1);
    }

    template <typename T>
    MtmMat<T> operator-(const MtmMat<T>& mat1,const MtmMat<T>& mat2){
        return MtmMat<T>(mat1)-=mat2;
    }

    template <typename T>
    MtmMat<T> operator-(const MtmMat<T>& mat1,const T& t){
        return mat1-MtmMat<T>(mat1.getDim(),t);
    }

    template <typename T>
    MtmMat<T> operator-(const T& t,const MtmMat<T>& mat1){
        MtmMat<T> tmp(mat1.getDim(),t);
        return tmp-mat1;
    }

    template <typename T>
    MtmMat<T> operator*(const MtmMat<T>& mat1,const T& t){
        return MtmMat<T>(mat1)*=t;
    }

    template <typename T>
    MtmMat<T> operator*(const T& t,const MtmMat<T>& mat1){
        return mat1*t;
    }

    template <typename T>
    MtmMat<T> operator*(const MtmVec<T>& vec1,const MtmVec<T>& vec2){
        if(vec1.getSize()!=vec2.getSize() || vec1.getVertical()==vec2.getVertical()){
            if(vec1.getVertical()) {
                Dimensions d1(vec1.getSize(),1);
                Dimensions d2(1,vec2.getSize());
                throw MtmExceptions::DimensionMismatch(d1,d2);
            } else{
                Dimensions d1(1,vec1.getSize());
                Dimensions d2(vec2.getSize(),1);
                throw MtmExceptions::DimensionMismatch(d1,d2);
            }
        }
        if(!vec1.getVertical()) {
            Dimensions dimension(1,1);
            MtmMat<T> result(dimension);
            T t(0);
            for (size_t i = 0; i < vec1.getSize(); ++i) {
                t+=(vec1[i]*vec2[i]);
            }
            result[0][0]=t;
            return result;
        } else {
            Dimensions dimension(vec1.getSize(),vec1.getSize());
            MtmMat<T> result(dimension);
            for (size_t row = 0; row < vec1.getSize(); ++row) {
                for (size_t col = 0; col < vec2.getSize(); ++col) {
                    result[row][col]=(vec1[row]*vec2[col]);
                }
            }
            return result;
        }
    }

    template <typename T>
    MtmMat<T> operator*(const MtmMat<T>& mat1,const MtmMat<T>& mat2){
        if(mat1.getDim().getCol()!=mat2.getDim().getRow())
        {
            Dimensions d1(mat1.getDim());
            Dimensions d2(mat2.getDim());
            throw MtmExceptions::DimensionMismatch(d1,d2);
        }
        Dimensions result_dim(mat1.getDim().getRow(),mat2.getDim().getCol()),mat2_transpose(mat2.getDim());
        mat2_transpose.transpose();
        MtmMat<T> result(result_dim);
        MtmMat<T> mat2_tmp(mat2_transpose);
        for (size_t i = 0; i < mat2.getDim().getRow(); ++i) {
            for (size_t j = 0; j < mat2.getDim().getCol(); ++j) {
                mat2_tmp[j][i]=mat2[i][j];
            }
        }

        for (size_t k = 0; k < mat2_transpose.getRow(); ++k) {
            mat2_tmp[k].transpose();
        }
        for (size_t l = 0; l < result_dim.getRow(); ++l) {
            for (size_t i = 0; i < result_dim.getCol(); ++i) {
                result[l][i]=(mat2_tmp[i]*mat1[l])[0][0];
            }
        }
        return result;
    }

    template <typename T>
    MtmMat<T> operator*(const MtmVec<T>& vec1,const MtmMat<T>& mat1){
        Dimensions vec_dim;
        MtmMat<T> vec2mat(vec_dim);
        if(vec1.getVertical()) {
            if(mat1.getDim().getRow()!=1)
            {
                Dimensions d1(vec1.getSize(),1);
                Dimensions d2(mat1.getDim());
                throw MtmExceptions::DimensionMismatch(d1,d2);
            }
            MtmMat<T> tmp_vec2mat(Dimensions(vec1.getSize(),1));
            vec2mat=tmp_vec2mat;
        } else {
            if(mat1.getDim().getRow()!=vec1.getSize()) {
                Dimensions d1(1,vec1.getSize());
               Dimensions d2(mat1.getDim());
                throw MtmExceptions::DimensionMismatch(d1,d2);
            }
            MtmMat<T> tmp_vec2mat(Dimensions(1,vec1.getSize()));
            vec2mat=tmp_vec2mat;
        }
        MtmVec<T> tmp(vec1);
        typename MtmVec<T>::iterator iterator1 = tmp.begin();
        typename MtmMat<T>::iterator iterator2 = vec2mat.begin();
        typename MtmVec<T>::iterator end_iterator = tmp.end();
        while(iterator1!=end_iterator){
            *iterator2=*iterator1;
            ++iterator1;
            ++iterator2;
        }
        return vec2mat*mat1;
    }


    template <typename T>
    MtmMat<T> operator*(const MtmMat<T>& mat1,const MtmVec<T>& vec1){
        MtmMat<T> vec2mat(Dimensions(1,1));
        if(vec1.getVertical()) {
            if(mat1.getDim().getCol()!=vec1.getSize())
            {
                Dimensions d1(vec1.getSize(),1);
                Dimensions d2(mat1.getDim());
                throw MtmExceptions::DimensionMismatch(d1,d2);
            }
            MtmMat<T> tmp_vec2mat(Dimensions(vec1.getSize(),1));
            vec2mat=tmp_vec2mat;
        } else {
            if(mat1.getDim().getCol()!=1)
            {
                Dimensions d1(1,vec1.getSize());
                Dimensions d2(mat1.getDim());
                throw MtmExceptions::DimensionMismatch(d1,d2);
            }
            MtmMat<T> tmp_vec2mat(Dimensions(1,vec1.getSize()));
            vec2mat=tmp_vec2mat;
        }
        MtmVec<T> tmp(vec1);
        typename MtmVec<T>::iterator iterator1 = tmp.begin();
        typename MtmMat<T>::iterator iterator2 = vec2mat.begin();
        typename MtmVec<T>::iterator end_iterator = tmp.end();
        while(iterator1!=end_iterator){
            *iterator2=*iterator1;
            ++iterator1;
            ++iterator2;
        }
        return operator*(mat1,vec2mat);
    }


}


#endif //EX3_MTMMAT_H
