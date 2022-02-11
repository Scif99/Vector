#include "vect.h"
#include <iostream>
#include <algorithm>

/*VECT_BASE*/

template<typename T, typename A> //move constructor
vect_base<T,A>::vect_base(vect_base<T,A>&& other)
    : alloc{other.alloc}, elem{other.elem}, sz{other.sz}, space{other.space}
{

    other.elem = nullptr;
    other.sz = 0;
    other.space =0;
}


/*OPTIMISE*/
template<typename T, typename A> //move assignment
vect_base<T,A>& vect_base<T,A>::operator=(vect_base<T,A>&& other)
{
    delete[] elem;

    alloc = other.alloc; 
    elem = other.elem;
    sz = other.sz;
    space = other.space;

    other.elem = nullptr; 
    other.sz = other.space = 0;

    //could use swap?

    return *this;
}



/* VECTOR*/

template<typename T, typename A> 
void vect<T,A>::destroy_elements() 
{
    for(std::size_t i = 0; i< vb.sz; i++) {~vb.elem[i];} //destroy s each element in the base
}

template<typename T, typename A> 
vect<T,A>::vect(std::size_t n, const T& def, const A& all) //contructs a vector of size n with default initialized values.
    : vb{vect_base<T,A>(all,n)}
{
    std::uninitialized_fill(vb.elem,vb.elem + vb.sz,def);
    std::cout<<"Constructed!"<<'\n';
}


template<typename T, typename A> 
vect<T,A>::vect(std::initializer_list<T> list, const A& all) //initializer-list constructor
    : vb{vect_base<T,A>(all, list.size())}
{
    std::uninitialized_copy(list.begin(), list.end(),vb.elem); //copy over to elem 
    std::cout<<"list-constructed!"<<std::endl; //for debugging
}


template<typename T, typename A>  
vect<T,A>::vect(const vect<T,A>& other) //copy constructor
    : vb{vect_base<T,A>(other.vb.alloc, other.vb.sz)} 
{
    std::uninitialized_copy(other.vb.elem, other.vb.elem + other.vb.sz, vb.elem); //copy from other
    std::cout<<"Copy constructor!"<<std::endl;
}

template<typename T, typename A>  
vect<T,A>& vect<T,A>::operator=(const vect<T,A>& other) //copy assignment
{
    if(&other==this) return *this; //check for self-assignment

    vect_base<T,A> p = vect_base<T,A>(other.vb.alloc, other.vb.sz); // these 2 lines are just the copy constructor!
    std::uninitialized_copy(other.vb.elem, other.vb.elem + other.vb.sz, p.elem);
    //vect temp{other};

    destroy_elements();
    std::swap(vb,p);

    std::cout<<"Copy Assignment!"<<std::endl;
    return *this;
}


//move constructor
template<typename T, typename A>
vect<T,A>::vect(vect<T,A>&& other) 
    : vb{std::move(other.vb)} //transfer ownership
{
    std::cout<<"Move constructor!"<<std::endl;
}

//move assignment
template<typename T, typename A>
vect<T,A>& vect<T,A>::operator=(vect<T,A>&& other) {

    if(&other == this)  return *this;

    std::swap(*this, other); //safe to swap here

    std::cout<<"Move assignment!"<<std::endl;
    return *this;
}


//destructor
template<typename T, typename A>
vect<T,A>::~vect() 
{
    //destroy elements in vb
    destroy_elements();
    std::cout<<"Destroyed!"<<std::endl;
}


//STORAGE

//adds free space 
template<typename T,typename A>
void vect<T,A>::reserve(std::size_t newalloc) 
{
   
    if(newalloc<=vb.space) return; //don't allow downsizing

    vect_base<T,A> b{vb.alloc, newalloc}; //allocate memory

    std::uninitialized_copy(vb.elem, vb.elem + size(), b.elem); //can be optimised...
    destroy_elements();

    std::swap(vb,b);

    //old value released on return
}

//change the size of the vector
template<typename T, typename A>
void vect<T,A>::resize(std::size_t newsize, T def) 
{
    reserve(newsize);
    if(vb.sz < newsize) {
        std::uninitialized_fill(vb.elem + vb.size, vb.elem + vb.newsize, def);
    }
    else{

        for(std::size_t i = newsize; i < vb.sz; i++) {~vb.elem[i];} //destroy excess
    }
    
    vb.sz = newsize;
}

//increase vector size by one
template<typename T, typename A> 
void vect<T,A>::push_back(const T& val) 
{
    if(vb.space==0) {reserve(8);} //case where vector is initially empty

    else if(vb.sz == vb.space)  reserve(2* vb.space); //case where vector has nor free space left
    vb.alloc.construct(&vb.elem[vb.sz], val);
    ++vb.space;
}

// template<typename T, typename A> void vect<T,A>::push_back( T&& tmp) {

//     if(space==0) {reserve(8);} //case where vector is initially empty

//     else if(sz == space) { //case where vector has nor free space left
//         reserve(2* space);
//     }

//     //use a move here as tmp is no longer of user
//     elem[sz] = std::move(tmp); //note that std::move is necessary as an rvalue reference is actually an lvalue!
//     ++sz;
// }

template<typename T, typename A> vect<T,A> fill(vect<T,A> v) {
    //v = vect(10);
    std::cout<<"Filled"<<std::endl;
    return v;
}



int main() {


    
    //fill(vect<int>{1,2});

    vect<char> v(5,'a');

    vect<char> w{'a','b','c'};


    w = v;

    w.push_back('h');

}





