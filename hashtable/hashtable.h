#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "avl.h"
#include <cmath>

#define FIRST_ARRAY_SIZE 4
#define MAX_ELEMS_RATIO 1 //means need a new array of double the size of the current one
#define INC_SIZE 2
#define MIN_ELEMS_RATIO 0.25 //means need a new array of half the size of the current one
#define DEC_SIZE 0.5

template<class T>
class HashTable{
    AVL<int,T>* *elements; //an array of pointers to avl trees //key: PlayerID (int), data: player
    int arr_size;
    int num_elems;
    double alpha;

public:
    HashTable() : arr_size(FIRST_ARRAY_SIZE), num_elems(0){
        alpha = ((sqrt(5) - 1) / 2.0); //lecture 7 slide 23
        elements = new AVL<int,T>*[FIRST_ARRAY_SIZE];
        initializeElements();
    }

    HashTable& operator=(const HashTable& other){
        for (int i = 0; i < arr_size; ++i) {
            if (elements[i] != nullptr) {
                delete elements[i];
            }
        }
        delete[] elements;
        //change elements size to match other.elements size
        arr_size = other.arr_size;
        AVL<int,T>** new_arr = new AVL<int,T>*[arr_size];
        for (int i = 0; i < arr_size; i++){
            new_arr[i] = new AVL<int,T>();
        }
        this->elements = new_arr;
        num_elems = other.num_elems;
        alpha = other.alpha;
        for(int i=0; i<arr_size; i++){
            if(!other.elements[i]->isEmpty()){
                *elements[i] = *(other.elements[i]);
            }
        }
        return *this;
    }

    ~HashTable(){
        for (int i = 0; i < arr_size; ++i) {
            if (elements[i] != nullptr) {
                delete elements[i];
            }
        }
        delete[] elements;
    }

    void initializeElements() {
        for (int i = 0; i < arr_size; i++){
            elements[i] = new AVL<int,T>();
        }
    }

    int getHashIndex(int key){
        double x = (key*alpha - floor(key*alpha)); //this is the fraction part of key*alpha
        return floor(arr_size * x);
    }

    bool insert(int key_, T& data_){
        bool res = insertToTable(key_, data_);
        checkAndRehashIfNeeded();
        return res;
    }

    bool insertToTable(int key, T& data){
        int index = getHashIndex(key);
        if(elements[index] == nullptr){
            elements[index] = new AVL<int,T>();
            if(!elements[index]){
                return false;
            }
        }
        elements[index]->insert(key, data);
        num_elems++;
        return true;
    }

    void remove(int key_){
        int index = getHashIndex(key_);
        elements[index]->remove(key_);
        num_elems--;
        checkAndRehashIfNeeded();
    }

    void checkAndRehashIfNeeded(){
        if(num_elems >= (arr_size * MAX_ELEMS_RATIO)){
            rehash(arr_size * INC_SIZE);
        }
        else if(num_elems <= (arr_size * MIN_ELEMS_RATIO)){ //TODO else if (items_amount * MIN_ITEMS_RATIO <= array_size)
            rehash(floor(arr_size * DEC_SIZE));
        }
    }

    void rehash(int new_size){
        //AVL<int,T>** old_arr = elements;
        int old_size = arr_size;
        AVL<int,T>** old_arr = new AVL<int,T>*[old_size];
        for (int i = 0; i < old_size; i++){
            old_arr[i] = new AVL<int,T>();
        }
        for(int i=0; i<old_size; i++){
            if(!elements[i]->isEmpty()){
                *old_arr[i] = *elements[i];
                elements[i]->clear();
            }
            delete elements[i];
        }
        delete[] elements;
        this->elements = new AVL<int,T>*[new_size];
        this->arr_size = new_size;
        this->num_elems = 0;
        initializeElements();
        for (int i = 0; i < old_size; i++) {
            if (!old_arr[i]->isEmpty() || old_arr != nullptr) {
                int size = old_arr[i]->nodeNum();
                int *key_arr = new int[size];
                T *data_arr = new T[size];
                old_arr[i]->avl2arr(&key_arr, &data_arr); //TODO check
                for(int j = 0; j < size; j++){
                    insertToTable(key_arr[j], data_arr[j]);
                }
                delete[] key_arr;
                delete[] data_arr;
            }
            old_arr[i]->clear();
            delete old_arr[i]; //destructor of the avl tree clears it //TODO does this delete the ptr to the tree or the tree itself?
        }
        delete[] old_arr;
    }

    T* find(int key) {
        int index = getHashIndex(key);
        if (elements[index]->isEmpty()){
            return nullptr;
        }
        if(*(elements[index]->getLastKey()) == key){
            return elements[index]->getLast();
        }
        elements[index]->getFirst(); //TODO do we need this to set the iterator to the first node or no?
        while(elements[index]->getCurrentKey() != elements[index]->getLastKey()){
            if(*(elements[index]->getCurrentKey()) == key){
                return elements[index]->getCurrent();
            }
            elements[index]->getNext();
        }
        return nullptr;
    }

    //merge hash tables function
    void mergeWith(HashTable<T>& other){
        for(int i = 0; i < other.arr_size; i++){
            int size = other.elements[i]->nodeNum();
            if(size != 0) {
                int *keys = new int[size];
                T *data = new T[size];
                other.elements[i]->avl2arr(&keys, &data);
                for (int j = 0; j < size; j++) {
                    this->insert(keys[j], data[j]);
                }
                delete[] keys;
                delete[] data;
                other.elements[i]->clear();
            }
        }
    }

    template<typename T_>
    friend HashTable<T_> mergeHash(HashTable<T_>& hash1, HashTable<T_>& hash2);
};

template<typename T_>
HashTable<T_> mergeHash(HashTable<T_>& hash1, HashTable<T_>& hash2){
    HashTable<T_> merged_hash = HashTable<T_>();
    merged_hash.mergeWith(hash1);
    merged_hash.mergeWith(hash2);
    return merged_hash;
}

#endif //HASHTABLE_H