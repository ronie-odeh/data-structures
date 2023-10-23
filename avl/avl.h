#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <ostream>
#include <cassert>
#include <memory>

using namespace std;
template <typename Key, typename T>
class AVL {
    struct node_t {
        T data;
        Key key;
        int height;
        int sub_size;
        bool isRightSon, isFirstNode;
        shared_ptr<node_t> left, right, parent;
        node_t(Key key, const T& data, bool isDummy = false) : data(data), key(key), height(1), sub_size(0),
                    isRightSon(false), isFirstNode(isDummy), left(nullptr), right(nullptr), parent(nullptr) {}
        ~node_t() = default;
    };

    typedef shared_ptr<node_t> Node;
    Node root, it, first, last;
    int num_nodes{};

    int getHeight(Node r) {
        if (!r)
            return 0;
        return r->height;
    }

    int getSubSize(Node r) {
        if (!r)
            return 0;
        return r->sub_size;
    }

    int getBF(Node r) {
        return getHeight(r->left) - getHeight(r->right);
    }

    int max(int right, int left) {
        if (right > left)
            return right;
        return left;
    }

    void Rotate_LR(Node r) {
        assert(r && r->left && r->left->right);
        Node A = r, B = r->left, C = B->right, y = C->left, w = C->right, p = A->parent;
        bool A_was_right = A->isRightSon;

        //do the rotation.
        B->right = y;
        if (y) {
            y->parent = B;
            y->isRightSon = true;
        }
        A->left = w;
        if (w) {
            w->parent = A;
            w->isRightSon = false;
        }
        C->right = A;
        A->parent = C;
        A->isRightSon = true;
        C->left = B;
        B->parent = C;
        B->isRightSon = false;

        //update the parent of A to be the parent of C after C took A's place.
        if (p->isFirstNode) {
            p->right = C;
            p->left = C;
            C->parent = p;
        }
        else if (A_was_right) {
            p->right = C;
            C->parent = p;
            C->isRightSon = true;
        }
        else {
            p->left = C;
            C->parent = p;
            C->isRightSon = false;
        }

        //update the sub tree length of the rest of the affected nodes of the tree after the rotation.
        A->height = 1 + max(getHeight(A->right), getHeight(A->left));
        A->sub_size = 1 + getSubSize(A->right) + getSubSize(A->left);

        B->height = 1 + max(getHeight(B->right), getHeight(B->left));
        B->sub_size = 1 + getSubSize(B->right) + getSubSize(B->left);

        C->height = 1 + max(getHeight(C->right), getHeight(C->left));
        C->sub_size = 1 + getSubSize(C->right) + getSubSize(C->left);
    }

    void Rotate_LL(Node r) {
        assert(r && r->left);
        Node A = r, B = r->left, y = B->right, p = A->parent;
        bool A_was_right = A->isRightSon;


        //do the rotation.
        B->right = A;
        A->parent = B;
        A->isRightSon = true;
        A->left = y;
        if (y) {
            y->parent = A;
            y->isRightSon = false;
        }
        //update the parent of A to be the parent of B after B took A's place.
        if (p->isFirstNode) {
            p->right = B;
            p->left = B;
            B->parent = p;
        }
        else if (A_was_right) {
            p->right = B;
            B->parent = p;
            B->isRightSon = true;
        }
        else {
            p->left = B;
            B->parent = p;
            B->isRightSon = false;
        }
        A->height = 1 + max(getHeight(A->right), getHeight(A->left));
        A->sub_size = 1 + getSubSize(A->right) + getSubSize(A->left);

        B->height = 1 + max(getHeight(B->right), getHeight(B->left));
        B->sub_size = 1 + getSubSize(B->right) + getSubSize(B->left);
    }

    void Rotate_RL(Node r) {
        assert(r && r->right && r->right->left);
        Node A = r, B = r->right, C = B->left, y = C->left, w = C->right, p = A->parent;
        bool A_was_right = A->isRightSon;

        //do the rotation.
        B->left = w;
        if (w) {
            w->parent = B;
            w->isRightSon = false;
        }
        A->right = y;
        if (y) {
            y->parent = A;
            y->isRightSon = true;
        }
        C->left = A;
        A->parent = C;
        A->isRightSon = false;
        C->right = B;
        B->parent = C;
        B->isRightSon = true;

        //update the parent of A to be the parent of C after C took A's place.
        if (p->isFirstNode) {
            p->right = C;
            p->left = C;
            C->parent = p;
        }
        else if (A_was_right) {
            p->right = C;
            C->parent = p;
            C->isRightSon = true;
        }
        else {
            p->left = C;
            C->parent = p;
            C->isRightSon = false;
        }

        //update the sub tree length of the rest of the affected nodes of the tree after the rotation.
        A->height = 1 + max(getHeight(A->right), getHeight(A->left));
        A->sub_size = 1 + getSubSize(A->right) + getSubSize(A->left);

        B->height = 1 + max(getHeight(B->right), getHeight(B->left));
        B->sub_size = 1 + getSubSize(B->right) + getSubSize(B->left);

        C->height = 1 + max(getHeight(C->right), getHeight(C->left));
        C->sub_size = 1 + getSubSize(C->right) + getSubSize(C->left);
    }

    void Rotate_RR(Node r) {
        assert(r && r->right);
        Node A = r, B = A->right, x = B->left, p = A->parent;
        bool A_was_right = A->isRightSon;

        //do the rotation.
        B->left = A;
        A->parent = B;
        A->isRightSon = false;
        A->right = x;
        if (x) {
            x->parent = A;
            x->isRightSon = true;
        }

        //update the parent of A to be the parent of B after B took A's place.
        if (p->isFirstNode) {
            p->right = B;
            p->left = B;
            B->parent = p;
        }
        else if (A_was_right) {
            p->right = B;
            B->parent = p;
            B->isRightSon = true;
        }
        else {
            p->left = B;
            B->parent = p;
            B->isRightSon = false;
        }
        //update the sub tree length of the rest of the affected nodes of the tree after the rotation.
        A->height = 1 + max(getHeight(A->right), getHeight(A->left));
        A->sub_size = 1 + getSubSize(A->right) + getSubSize(A->left);

        B->height = 1 + max(getHeight(B->right), getHeight(B->left));
        B->sub_size = 1 + getSubSize(B->right) + getSubSize(B->left);
    }

    void Rotate(Node r) {
        if (!r)
            return;
        if (getBF(r) == 2 && getBF(r->left) == -1) {
            Rotate_LR(r);
            return;
        }
        if (getBF(r) == 2 && getBF(r->left) >= 0) {
            Rotate_LL(r);
            return;
        }
        if (getBF(r) == -2 && getBF(r->right) == 1) {
            Rotate_RL(r);
            return;
        }
        if (getBF(r) == -2 && getBF(r->right) <= 0) {
            Rotate_RR(r);
            return;
        }
    }

    void update_and_rotate(Node r) {
        if (!r)
            return;
        Node iterator = r, p = iterator->parent;
        while (!p->isFirstNode) {
            iterator->height = 1 + max(getHeight(iterator->left), getHeight(iterator->right));
            iterator->sub_size = 1 + getSubSize(iterator->right) + getSubSize(iterator->left);
            Rotate(iterator);
            iterator = p;
            p = iterator->parent;
        }
        iterator->height = 1 + max(getHeight(iterator->left), getHeight(iterator->right));
        iterator->sub_size = 1 + getSubSize(iterator->right) + getSubSize(iterator->left);
        Rotate(iterator);
    }

    bool isLeaf(Node r) {
        return !r->left && !r->right;
    }

    Node toFind(Node r, Key key_) {
        Node iterator = r;
        if(!r)
            return nullptr;
        while (!isLeaf(iterator)) {
            if (key_ == iterator->key)
                return iterator;

            if (key_ > iterator->key) {
                if (!iterator->right)
                    break;
                iterator = iterator->right;
                continue;
            }

            if (key_ < iterator->key) {
                if (!iterator->left)
                    break;
                iterator = iterator->left;
                continue;
            }
        }
        return iterator;
    }

    Node getMinNode(Node r) {
        if (!r)
            return NULL;
        Node iterator = r;
        while (iterator->left) {
            iterator = iterator->left;
        }
        return iterator;
    }

    Node copyTree(Node& r, Node& old_r, Node& p) {
        if (!old_r)
            return nullptr;
        r = Node(new node_t(Key(old_r->key), T(old_r->data)));
        r->isRightSon = old_r->isRightSon;
        r->height = old_r->height;
        r->sub_size = old_r->sub_size;
        r->parent = p;
        r->left = copyTree(r->left, old_r->left, r);
        r->right = copyTree(r->right, old_r->right, r);
        return r;
    }

    void avl2arr_aux(Node r, Key** keys, T** vals, int* index){
        if(r == nullptr)
            return;
        avl2arr_aux(r->left,keys,vals,index);

        (*keys)[*index] = Key(r->key);
        (*vals)[*index] = T(r->data);
        (*index)++;

        avl2arr_aux(r->right,keys,vals,index);
    }

    //Array to AVL.
    Node arr2avl(Key* arr1, T* arr2, int n, Node parent_, bool right_son) {
        if (n == 1) {
            Node to_ret = Node(new node_t(*arr1, *arr2));
            to_ret->isRightSon = right_son;
            to_ret->parent = parent_;
            to_ret->sub_size = 1;
            return to_ret;
        }
        if (n == 2) {
            Node p = Node(new node_t(*arr1, *arr2));
            Node s = Node(new node_t(arr1[1], arr2[1]));
            p->isRightSon = right_son;
            p->parent = parent_;
            p->height = 2;

            p->right = s;
            s->parent = p;
            s->isRightSon = true;

            s->sub_size = 1;
            p->sub_size = 2;
            return p;
        }
        int mid = n / 2;
        if (mid % 2 == 0) {
            if (n % 2 == 1)
                mid++;
            else mid--;
        }

        Node r = Node(new node_t(arr1[mid], arr2[mid]));
        r->parent = parent_;
        r->isRightSon = right_son;
        r->left = arr2avl(arr1, arr2, mid, r, false);
        r->right = arr2avl(arr1 + mid + 1,arr2 + mid + 1, n - mid - 1, r, true);
        r->height = 1 + max(getHeight(r->left), getHeight(r->right));
        r->sub_size = 1 + getSubSize(r->right) + getSubSize(r->left);
        return r;
    }

    void update_sub_size(Node r){
        if(!r)
            return;
        update_sub_size(r->left);
        update_sub_size(r->right);
        r->sub_size = 1 + getSubSize(r->right) + getSubSize(r->left);
    }

public:
    //create root dummy node, and set height to zero.
    AVL() {
        Node n = Node(new node_t(Key(), T(), true));
        root = n;
        num_nodes = 0;
        it = nullptr;
        first = nullptr;
        last = nullptr;
    }

    AVL(const AVL& old) :AVL() {
        copyTree(root->right, old.root->right, root);
        root->left = root->right;
        first = toFind(root->right, old.first->key);
        last = toFind(root->right, old.last->key);
        this->num_nodes = old.num_nodes;
    }

    AVL& operator=(const AVL& old) {
        if (this == &old)
            return *this;
        this->clear();
        if(old.isEmpty())
            return *this;
        copyTree(root->right, old.root->right, root);
        root->left = root->right;
        this->num_nodes = old.num_nodes;
        this->first = toFind(root->right, old.first->key);
        this->last = toFind(root->right, old.last->key);
        return *this;
    }

    ~AVL() {
        this->it = nullptr;
        this->first = nullptr;
        this->last = nullptr;
        this->clear();
        root = nullptr;
    }

    bool isEmpty() const{
        return  num_nodes == 0;
    }

    bool insert(Key key_, const T& data_) {
        Node to_add = Node(new node_t(Key(key_), T(data_)));
        if (!to_add)
            return false;
        Node iterator = root->right;

        //Adds the new node as the root.
        if (isEmpty()) {
            root->left = to_add;
            root->right = to_add;
            to_add->parent = root;
            num_nodes++;
            first = to_add;
            last = to_add;
            return true;
        }

        //Add the new node in the right place.
        iterator = toFind(iterator, key_);
        if (iterator->key == to_add->key)
            return false;

        if (iterator->key < key_) {
            iterator->right = to_add;
            to_add->parent = iterator;
            to_add->isRightSon = true;
        }

        else if (iterator->key > key_) {
            iterator->left = to_add;
            to_add->parent = iterator;
            to_add->isRightSon = false;
        }

        update_and_rotate(to_add);
        num_nodes++;
        if (first->key > key_) {
            first = to_add;
        }
        if (last->key < key_) {
            last = to_add;
        }
        return true;
    }

    T* find(Key key_) {
        if (isEmpty())
            return nullptr;
        Node n = toFind(root->right, key_);
        if (n && key_ == n->key)
            return &n->data;
        return nullptr;
    }

//    int Rank(Key key_){
//        if (isEmpty())
//            return -1;
//        Node iter = toFind(root->right, key_);
//        if (!iter || key_ != iter->key)
//            return -1;
//        int r = 0;
//        iter = root->right;
//        while(iter->key != key_){
//            if(iter->key > key_){
//                iter = iter->left;
//                continue;
//            }
//            if(iter->key < key_){
//                r = r + getSubSize(iter->left) + 1;
//                iter = iter->right;
//                continue;
//            }
//        }
//        r = r + getSubSize(iter->left) + 1;
//        return r;
//    }
//
//    T* Select(int index){
//        if(num_nodes < index)
//            return nullptr;
//        if(isEmpty())
//            return nullptr;
//        int k = index;
//        Node iter = root->right;
//        while(getSubSize(iter->left) != k-1){
//            if(getSubSize(iter->left) > k-1){
//                iter = iter->left;
//                continue;
//            }
//            if(getSubSize(iter->left) < k-1){
//                k = k - getSubSize(iter->left) - 1;
//                iter = iter->right;
//                continue;
//            }
//        }
//        return &(iter->data);
//    }

    void clear() {
        if(num_nodes == 0)
            return;
        Node parent = root;
        Node iterator = root->right;
        bool wasRight;
        while (!this->isEmpty()) {
            if (isLeaf(iterator)) {
                if (iterator == root->right) {
                    parent->right = nullptr;
                    parent->left = nullptr;
                    num_nodes--;
                    break;
                }
                wasRight = iterator->isRightSon;
                if (wasRight){
                    parent->right = nullptr;
                    num_nodes--;
                }
                else{
                    parent->left = nullptr;
                    num_nodes--;
                }
                iterator = parent;
                parent = iterator->parent;
                continue;
            }
            if (iterator->left) {
                parent = iterator;
                iterator = iterator->left;
                continue;
            }
            if (iterator->right) {
                parent = iterator;
                iterator = iterator->right;
                continue;
            }
        }
        this->it = nullptr;
        this->first = nullptr;
        this->last = nullptr;
        num_nodes = 0;
    }

    bool remove(Key key_) {
        if (isEmpty())
            return false;
        Node to_del = toFind(root->right, key_), p = to_del->parent;

        //the node to delete isn't found in the tree.
        if (to_del->key != key_)
            return false;

        //the node to delete is a leaf; has no sons.
        if (isLeaf(to_del)) {
            if (p == root) {
                p->right = nullptr;
                p->left = nullptr;
                to_del->parent = nullptr;
                first = nullptr;
                last = nullptr;
                num_nodes--;
                return true;
            }
            if (to_del == first) {
                first = p;
            }
            if (to_del == last) {
                last = p;
            }
            if (to_del->isRightSon) {
                p->right = nullptr;
            }
            if (!to_del->isRightSon) {
                p->left = nullptr;
            }
            num_nodes--;
            update_and_rotate(p);
            return true;
        }

        //the node to_delete has 2 sons (left & right)
        if (to_del->right && to_del->left) {
            Node to_swap = getMinNode(to_del->right), p_ = to_swap->parent, update_from = to_swap;
            bool swap_is_son = p_ == to_del, update_from_swap = swap_is_son;
            if (to_swap->isRightSon) {
                p_->right = nullptr;
            }
            else p_->left = nullptr;

            //attach the minimum key node in the right sub tree in place of the node to_delete.
            if (swap_is_son) {
                to_swap->left = to_del->left;
                to_swap->parent = to_del->parent;
                to_swap->isRightSon = to_del->isRightSon;
            }
            else {
                if (!to_swap->right) {
                    to_swap->right = to_del->right;
                }
                else {
                    Node tmp = to_swap->right;
                    p_->left = tmp;
                    tmp->isRightSon = false;
                    //tmp->right->parent = tmp;
                    tmp->parent = p_;
                    //to_swap->right = nullptr;
                    to_swap->right = to_del->right;
                    update_from_swap = true;
                    update_from = tmp;
                }
                to_swap->left = to_del->left;
                to_swap->parent = to_del->parent;
                to_swap->isRightSon = to_del->isRightSon;
            }
            //attach the parent to have the new node as son in place of the node to_delete.
            if (p == root) {
                p->right = to_swap;
                p->left = to_swap;
            }
            else if (to_del->isRightSon) {
                p->right = to_swap;
            }
            else if (!to_del->isRightSon) {
                p->left = to_swap;
            }

            //detach the right, left and parent of the node to_delete.
            to_del->right = nullptr;
            to_del->left = nullptr;
            to_del->parent = nullptr;

            //update the sons to point at new parent.
            if (to_swap->right)
                to_swap->right->parent = to_swap;
            if (to_swap->left)
                to_swap->left->parent = to_swap;

            //update the tree, starting from the parent of the minimum node before we swapped.
            if (!swap_is_son && !update_from_swap)
                update_and_rotate(p_);
            else update_and_rotate(update_from);
            num_nodes--;
            return true;
        }

        //the node to_delete has one son (left / right)
        else {
            if (to_del == first) {
                first = to_del->right;
                while (first->left) {
                    first = first->left;
                }
            }
            if (to_del == last) {
                last = to_del->left;
                while (last->right) {
                    last = last->right;
                }
            }
            Node sub;
            if (to_del->left)
                sub = to_del->left;
            else if (to_del->right)
                sub = to_del->right;
            sub->parent = p;
            if (p == root) {
                p->right = sub;
                p->left = sub;
            }
            else {
                if (to_del->isRightSon) {
                    p->right = sub;
                    sub->isRightSon = true;
                }
                else {
                    p->left = sub;
                    sub->isRightSon = false;
                }
            }
            to_del->right = nullptr;
            to_del->left = nullptr;
            to_del->parent = nullptr;
            update_and_rotate(sub);
            num_nodes--;
            return true;
        }
    }

    int nodeNum() {
        return num_nodes;
    }


    T* getFirst() {
        if (this->isEmpty())
            return nullptr;
        if (first) {
            it = first;
            return &first->data;
        }
        return nullptr;
    }

    T* getNext() {
        assert(first && it); //TODO
        if (num_nodes == 1 || it == last) {
            return nullptr;
        }
        if (it->parent == root) {
            if (it->right) {
                it = it->right;
                while (it->left) {
                    it = it->left;
                }
                return &it->data;
            }
            else {
                return nullptr;
            }
        }

        if (it->right) {
            it = it->right;
            while (it->left) {
                it = it->left;
            }
            return &it->data;
        }

        if (!it->isRightSon && it->parent != root && !it->right) {
            it = it->parent;
            return &it->data;
        }
        if (it->isRightSon) {
            while (it->key > it->parent->key) {
                it = it->parent;
            }
            it = it->parent;
            return &it->data;
        }
        return nullptr;
    }

    T* getLast() {
        if (this->isEmpty())
            return nullptr;
        if (last)
            return &last->data;
        return nullptr;
    }

    T* getCurrent() {
        if (!it)
            return nullptr;
        return &it->data;
    }

    Key* getLastKey() {
        if (this->isEmpty())
            return nullptr;
        if (last)
            return &last->key;
        return nullptr;
    }

    Key* getCurrentKey() {
        if (!it)
            return nullptr;
        return &it->key;
    }

    void avl2arr(Key** keys,T** data){
        int index = 0;
        avl2arr_aux(root->right,keys,data,&index);
    }

    //takes in 4 arrays, their sizes, and 2 more for output
    void merge(Key** keys1, T** data1, int n1, Key** keys2, T** data2, int n2, Key** keys, T** data){
        int i1=0, i2=0, index=0;
        while(i1 < n1 && i2 < n2){
            if((*keys1)[i1] < (*keys2)[i2]){
                (*keys)[index] = (*keys1)[i1];
                (*data)[index] = (*data1)[i1];
                i1++;
                index++;
            }
            else if((*keys1)[i1] > (*keys2)[i2]){
                (*keys)[index] = (*keys2)[i2];
                (*data)[index] = (*data2)[i2];
                i2++;
                index++;
            }
        }
        if(i1 < n1 && i2 == n2){
            while(i1 < n1){
                (*keys)[index] = (*keys1)[i1];
                (*data)[index] = (*data1)[i1];
                i1++;
                index++;
            }
        }
        else if(i1 == n1 && i2 < n2){
            while(i2 < n2){
                (*keys)[index] = (*keys2)[i2];
                (*data)[index] = (*data2)[i2];
                i2++;
                index++;
            }
        }
    }

    template <typename Key_,typename T_>
    friend AVL<Key_,T_> ArrToAVL(Key_* keys, T_* vals, int n);

    template <typename Key_,typename T_>
    friend AVL<Key_,T_> mergeTrees(AVL<Key_,T_>* tree1, AVL<Key_,T_>* tree2); //TODO
};

template <typename Key_, typename T_>
AVL<Key_,T_> ArrToAVL(Key_* keys, T_* vals, int n) {
    AVL<Key_,T_> to_ret = AVL<Key_,T_>();
    to_ret.root->right = to_ret.arr2avl(keys, vals, n, to_ret.root, true);
    to_ret.root->left = to_ret.root->right;
    to_ret.it = nullptr;
    to_ret.first = to_ret.toFind(to_ret.root->right, keys[0]);
    to_ret.last = to_ret.toFind(to_ret.root->right, keys[n - 1]);
    to_ret.num_nodes = n;
    to_ret.update_sub_size(to_ret.root->right);
    return to_ret;
}

template <typename Key_, typename T_>
AVL<Key_,T_> mergeTrees(AVL<Key_,T_>* tree1, AVL<Key_,T_>* tree2){
    //merge this and tree
    int size1 = tree1->nodeNum();
    Key_ *key_arr1 = new Key_[size1];
    T_ *data_arr1 = new T_[size1];
    tree1->avl2arr(&key_arr1, &data_arr1);
    tree1->clear();

    int size2 = tree2->nodeNum();
    Key_ *key_arr2 = new Key_[size2];
    T_ *data_arr2 = new T_[size2];
    tree2->avl2arr(&key_arr2, &data_arr2);
    tree2->clear();

    int size = size1 + size2;
    Key_ *key_arr = new Key_[size];
    T_ *data_arr = new T_[size];

    tree1->merge(&key_arr1, &data_arr1, size1, &key_arr2, &data_arr2, size2, &key_arr, &data_arr);
    AVL<Key_,T_> merged_tree = ArrToAVL(key_arr, data_arr, size);

    delete[](key_arr1);
    delete[](data_arr1);
    delete[](key_arr2);
    delete[](data_arr2);
    delete[](key_arr);
    delete[](data_arr);

    return merged_tree;
}

#endif //AVL_H
