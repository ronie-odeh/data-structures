#ifndef EX3_MTMVEC_H
#define EX3_MTMVEC_H

#include <vector>
#include <unordered_map>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "Complex.h"

using std::size_t;
namespace MtmMath {
    template <typename T>
    class MtmVec {
        std::vector<T> vec;
        size_t vec_size;
        bool vertical;
        size_t row;
        bool is_triag=false;
        bool is_upper_triag;
    public:
        size_t& get_row_index(){
            return this->row;
        }

        bool& get_is_triag(){
            return this->is_triag;
        }

        bool& get_is_upper(){
            return this->is_upper_triag;
        }



        /*
         * Vector constructor, m is the number of elements in it and val is the initial value for the matrix elements
         */
        explicit MtmVec(size_t m=1, const T& val=T()) try : vec(m,val) , vec_size(m),vertical(true),is_triag(false){
            if(m<=0)
                throw MtmExceptions::IllegalInitialization();
        }
            catch(const std::bad_alloc&){
                throw MtmExceptions::OutOfMemory();
            }

        ~MtmVec()= default;

        MtmVec(const MtmVec& vec1)try:vec(vec1.vec_size),vec_size(vec1.vec_size),vertical(vec1.vertical){
            for(size_t i = 0; i < vec1.vec_size; ++i) {
                vec.at(i)=vec1.vec.at(i);
            }
        }
        catch(const std::bad_alloc& e){
            throw MtmExceptions::OutOfMemory();
        }


        /*
         * Function that get function object f and uses it's () operator on each element in the vectors.
         * It outputs the function object's * operator after iterating on all the vector's elements
         */
        template <typename Func>
        T vecFunc(Func& f) const{
            Func tmp=f;
            for (size_t i = 0; i < vec_size; ++i) {
                tmp(vec.at(i));
            }
            return *tmp;
        }

        /*
         * Resizes a vector to dimension dim, new elements gets the value val.
         * Notice vector cannot transpose through this method.
         */
        void resize(Dimensions dim, const T& val=T()){
            if(vertical) {
                MtmMath::Dimensions dimension(vec_size,1);
                if (dim.getCol() != 1)
                    throw MtmExceptions::ChangeMatFail(dimension, dim);
            }
            else {
                MtmMath::Dimensions dimension(1,vec_size);
                if (dim.getRow() != 1)
                    throw MtmExceptions::ChangeMatFail(dimension, dim);
            }
            if(vertical){
                try { vec.resize(dim.getRow(),val); }
                catch(std::bad_alloc& e){ throw MtmExceptions::OutOfMemory(); }
                vec_size=dim.getRow();

            }
            else{
                try{vec.resize(dim.getCol(),val); }
                catch(std::bad_alloc& e){ throw MtmExceptions::OutOfMemory(); }
                vec_size=dim.getCol();
            }
        }

        /*
         * Performs transpose operation on matrix
         */
        void transpose(){
            vertical = !vertical;
        }

        MtmVec& operator=(const MtmVec& vec1){
            if(this==&vec1)
            {
                return *this;
            }
            MtmVec<T> tmp_vec(vec1);
            vertical=vec1.vertical;
            Dimensions dim(tmp_vec.vec_size);
            if(!vertical)
                dim.transpose();
            this->resize(dim);
            for(size_t i =0; i<vec1.vec_size; ++i)
            {
                this->vec.at(i)=tmp_vec[i];
            }
            return *this;
        }

        /*
         * makes an [] operator to the MtmVec object, which returns the element in the MtmVec that's found in
         * the place x :MtmVec[x]
         */
        T& operator[](size_t index){
            if(index>=vec_size||index<0)
                throw MtmExceptions::AccessIllegalElement();
            if(is_triag){
                if(is_upper_triag&&index<row){
                    throw MtmExceptions::AccessIllegalElement();
                }
                else if(!is_upper_triag&&index>row){
                    throw MtmExceptions::AccessIllegalElement();
                }
            }
            return vec.at(index);
        }
        /*
         * makes an [] operator for the MtmVec which is identical to the other one but the difference is that this one
         * is made for a const MtmVec.
         */
        const T& operator[](size_t index) const{
            if(index>=vec_size||index<0)
                throw MtmExceptions::AccessIllegalElement();
            return vec.at(index);
        }

        /*
         * makes a += operator for MtmVec which is used to add two MtmVec variables which adds the value of the
         * second MtmVec variable instantly to the MtmVec on the right side.
         */
        MtmVec& operator+=(const MtmVec& vec1){

            if(vertical!=vec1.vertical||vec_size!=vec1.vec_size)
            {
                if(vertical&&!vec1.vertical)
                {
                    MtmMath::Dimensions dim1(vec_size,1);
                    MtmMath::Dimensions dim2(1,vec1.vec_size);
                    throw MtmExceptions::DimensionMismatch(dim1,dim2);
                }
                else if(!vertical&&vec1.vertical)
                {

                    MtmMath::Dimensions dim1(1,vec_size);
                    MtmMath::Dimensions dim2(vec1.vec_size,1);
                    throw MtmExceptions::DimensionMismatch(dim1,dim2);
                }
                else
                {
                    if(vertical){

                        MtmMath::Dimensions dim1(vec_size,1);
                        MtmMath::Dimensions dim2(vec1.vec_size,1);
                        throw MtmExceptions::DimensionMismatch(dim1,dim2);
                    }
                    else{
                        MtmMath::Dimensions dim1(1,vec_size);
                        MtmMath::Dimensions dim2(1,vec1.vec_size);
                        throw MtmExceptions::DimensionMismatch(dim1,dim2);
                    }
                }
            }
            for(size_t i=0; i < vec_size; ++i)
                vec.at(i)+=vec1.vec.at(i);
            return *this;
        }

        /*
        * makes a -= operator for MtmVec which is used to minus two MtmVec variables which minus the value of the
        * second MtmVec variable instantly from the MtmVec on the right side.
        */
        MtmVec& operator-=(const MtmVec& vec1){
            return this->operator+=(-vec1);
        }

        /*
         * creates a new copy of the MtmVec that this operator is used on (this) which all the elements in the
         * MtmVec(this) are multiplied by -1.
         * (creates -(this)).
         */
        MtmVec operator-() const{
            MtmVec<T> new_vec(*this);
            for (size_t i = 0; i < vec_size; ++i) {
                new_vec.vec.at(i)=-vec.at(i);
            }
            return new_vec;
        }

        /*
         * operator += for a MtmVec and scalar t from type T, which adds t to the elements of MtmVec.
         */
        MtmVec& operator+=(const T& t){
            MtmVec<T> vec2(vec_size,t);
            if(!vertical)
                vec2.transpose();
            return *this+=vec2;
        }

        /*
         * operator -= for a MtmVec and scalar t from type T, which minus t from the elements of MtmVec.
         */
        MtmVec& operator-=(const T& t){
            MtmVec<T> vec2(vec_size,t);
            if(!vertical)
                vec2.transpose();
            return *this-=vec2;
        }

        /*
         * operator *= for a MtmVec and scalar t from type T, which multiplies all the elements of MtmVec with t.
         */
        MtmVec& operator*=(const T& t){
            for (size_t i = 0; i < vec_size; ++i) {
                vec.at(i)*=t;
            }
            return *this;
        }



        size_t getSize() const{
            size_t tmp=this->vec_size;
            return tmp;
        }



        bool getVertical()const{
            bool tmp=this->vertical;
            return tmp;
        }

        /*
         * prints vector.
         */
        void print_vec(){
            for (size_t i = 0; i < vec_size; ++i) {
                std::cout<<vec.at(i)<<" ";
            }
            std::cout<<std::endl;
        }

        class iterator{
        protected:
            friend class MtmVec<T>;
            iterator(MtmVec<T>* mtmVec, size_t index):mtmVec(mtmVec),index(index){}
            MtmVec<T>* mtmVec;
            size_t index;

        public:
            T& operator*() {
                if(index<0||index>=mtmVec->vec_size)
                    throw MtmExceptions::AccessIllegalElement();
                return mtmVec->vec.at(index);
            }

            virtual iterator& operator++(){
                ++index;
                return *this;
            }

            bool operator==(const iterator& iterator1) const {
                return index == iterator1.index;
            }

            bool operator!=(const iterator& iterator1) const {
                return !(*this == iterator1);
            }

        };

    class nonzero_iterator: public iterator{
        friend class MtmVec<T>;
        nonzero_iterator(MtmVec<T>* mtmVec, size_t index):iterator(mtmVec,index){}
        public:
            nonzero_iterator& operator++() override{
                while(++this->index<this->mtmVec->vec_size&&this->mtmVec->vec.at(this->index)==T(0)){}
                return *this;
            }
        };

        iterator begin() {
            return iterator(this,0);
        }

        iterator end() {
            return iterator(this,this->vec_size);
        }

        nonzero_iterator nzbegin() {
            size_t i=0;
            while(i<vec_size&&vec.at(i)==T(0)){
                i++;
            }
            return nonzero_iterator(this,i);
        }

        nonzero_iterator nzend() {
            return nonzero_iterator(this,vec_size);
        }
    };

    /*
     * returns a MtmVec which is a vec1+vec2.
     */
    template <typename T>
    MtmVec<T> operator+(const MtmVec<T>& vec1,const MtmVec<T>& vec2){
        return MtmVec<T>(vec1)+=vec2;
    }

    /*
     * returns a MtmVec which is a vec1-vec2;
     */
    template <typename T>
    MtmVec<T> operator-(const MtmVec<T>& vec1,const MtmVec<T>& vec2){
        return MtmVec<T>(vec1)-=vec2;
    }

    /*
     * an + operator for MtmVec and element T from the same type as the elements in MtmVec
     * which allows : MtmVec+T
     */

    template <typename T>
    MtmVec<T> operator+(const MtmVec<T>& vec1,const T& t){
        return MtmVec<T>(vec1)+=t;
    }

    /*
     * an + operator for MtmVec and element T from the same type as the elements in MtmVec
     * which allows : T+MtmVec
     */
    template <typename T>
    MtmVec<T> operator+(const T& t,const MtmVec<T>& vec1){
        return vec1+t;
    }

    /*
     * an - operator for MtmVec and element T from the same type as the elements in MtmVec
     * which allows : MtmVec-T
     */
    template <typename T>
    MtmVec<T> operator-(const MtmVec<T>& vec1,const T& t){
        return MtmVec<T>(vec1)-=t;
    }

    /*
     * an * multiply operator for MtmVec with T, MtmVec*T.
     * which multiplies all the elements of the vec1 by t.
     */
    template <typename T>
    MtmVec<T> operator*(const MtmVec<T>& vec1,const T& t){

        return MtmVec<T>(vec1)*=t;
    }

    /*
     * an * multiply operator for MtmVec with T, T*MtmVec.
     * which multiplies all the elements of the vec1 by t.
     */
    template <typename T>
    MtmVec<T> operator*(const T& t,const MtmVec<T>& vec1){
        return MtmVec<T>(vec1)*t;
    }

    template <typename T>
    MtmVec<T> operator-(const T& t,const MtmVec<T>& vec1){
        MtmVec<T> tmp(vec1);
        for (size_t i = 0; i < vec1.getSize(); ++i) {
            tmp[i]=t-tmp[i];
        }
        return tmp;
    }
}

#endif //EX3_MTMVEC_H
