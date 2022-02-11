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
    other.sz = 0;
    other.space =0;

    //could use swap?

    return *this;
}



/* VECTOR*/


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

/*OPTIMISE*/

template<typename T, typename A>  
vect<T,A>& vect<T,A>::operator=(const vect<T,A>& other) //copy assignment
{
    if(&other==this) return *this; //check for self-assignment

    vect_base<T,A> p = vect_base<T,A>(other.vb.alloc, other.vb.sz);

    std::uninitialized_copy(other.vb.elem, other.vb.elem + other.vb.sz, p.elem);

    //destroy elements in vb?
    for(std::size_t i = 0; i< vb.sz; i++) {
        ~vb.elem[i];
    }

    //alternatively, copy & swap?

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
    
    //might want to destroy elements of other explicitly...
    //but the state of other doesn't matter after move, so can just use a swap to transfer ownership
    std::swap(*this, other); 

    std::cout<<"Move assignment!"<<std::endl;
    return *this;
}


//destructor
template<typename T, typename A>
vect<T,A>::~vect() 
{
    //destroy elements in vb
    for(std::size_t i = 0; i< vb.sz; i++) {
        ~vb.elem[i];
    }
    std::cout<<"Destroyed!"<<std::endl;
}


//STORAGE


//adds free space 
template<typename T,typename A>
void vect<T,A>::reserve(std::size_t newalloc) 
{
   
    if(newalloc<=vb.space) return; //don't allow downsizing

    vect_base<T,A> b{vb.alloc, newalloc}; //allocate memory
    std::uninitialized_copy(vb.elem, vb.elem + size(), b.elem); /*OPTIMISE*/
 
    //destroy elements in vb
    for(std::size_t i = 0; i< vb.sz; i++) {
        ~vb.elem[i];
    }

    std::swap<vect_base<T,A>>(*this, b); //swap (uses move semantics?)

    //old memory freed as vect_base gets destroyed (RAII)
}

// //change the size of the vector
// template<typename T, typename A>
// void vect<T,A>::resize(std::size_t newsize, T def) 
// {
//     reserve(newsize);
//     for(std::size_t i = sz ;i< newsize;i++) alloc.construct(&elem[i], def);
//     for(std::size_t i = newsize ;i< sz;i++) alloc.destroy(&elem[i]); //destroy 'excess' memory in case we downsize
    
//     sz = newsize;
// }

// //increase vector size by one
// template<typename T, typename A> 
// void vect<T,A>::push_back(const T& val) 
// {
//     if(space==0) {reserve(8);} //case where vector is initially empty

//     else if(sz == space)  reserve(2* space); //case where vector has nor free space left
//     alloc.construct(&elem[sz], val);
//     sz++;
// }

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


}





