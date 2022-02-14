#include "vect.h"
#include <iostream>
#include <algorithm>

/*VECT_BASE*/




template<typename T, typename A> //move constructor
vect_base<T,A>::vect_base(vect_base<T,A>&& other)
    : alloc{other.alloc}, sz{other.sz}, elem{other.elem}, space{other.space}
{

    other.elem = nullptr;
    other.sz = 0;
    other.space =0;

    //std::cout<<"Base Move-Constructed!\n";
}




/*Use swap?*/
template<typename T, typename A> //move assignment
vect_base<T,A>& vect_base<T,A>::operator=(vect_base<T,A>&& other)
{
    swap(*this, other);
    //std::cout<<"Base Move-Assigned!\n";
    return *this;
}



/* VECTOR*/


template<typename T, typename A> 
void vect<T,A>::destroy_elements() //calls destructor for each element
{
    for(std::size_t i = 0; i< vb.sz; i++) {~vb.elem[i];}
}

template<typename T, typename A> 
vect<T,A>::vect(std::size_t n, const T& def, const A& all) //contructs a vector of size n with default initialized values.
    : vb{vect_base<T,A>(all,n)}
{
    std::uninitialized_fill(vb.elem,vb.elem + vb.sz,def);
    std::cout<<"Constructed!\n";
}


template<typename T, typename A> 
vect<T,A>::vect(std::initializer_list<T> list, const A& all) //initializer-list constructor
    : vb{vect_base<T,A>(all, list.size())}
{
    std::uninitialized_copy(list.begin(), list.end(),vb.elem); //copy over to elem 
    std::cout<<"list-constructed!\n"; 
}


template<typename T, typename A>  
vect<T,A>::vect(const vect<T,A>& other) //copy constructor
    : vb{vect_base<T,A>(other.vb.alloc, other.vb.sz)} 
{
    std::uninitialized_copy(other.vb.elem, other.vb.elem + other.vb.sz, vb.elem); //copies into the uninitialized memory allocated by alloc
    std::cout<<"Copy Constructed!\n";
}



template<typename T, typename A>  
vect<T,A>& vect<T,A>::operator=(const vect<T,A>& other) //copy assignment
{
    //see 'copy-and-swap'
    vect<T,A> tmp{other};
    swap(*this,tmp);
    std::cout<<"Copied!\n";
    return *this;
}


//move constructor
template<typename T, typename A>
vect<T,A>::vect(vect<T,A>&& other) 
    : vb{std::move(other.vb)} //transfer ownership
{
    std::cout<<"Move Constructed!\n";
}

//move assignment
template<typename T, typename A>
vect<T,A>& vect<T,A>::operator=(vect<T,A>&& other) {
    
    swap(*this, other);
    return *this;
}


//destructor
template<typename T, typename A>
vect<T,A>::~vect() 
{
    destroy_elements();
    std::cout<<"Destroyed!\n";
}


//STORAGE

//adds free space 
template<typename T,typename A>
void vect<T,A>::reserve(std::size_t new_capacity) 
{
   
    if(new_capacity<=vb.space) return; //don't allow downsizing

    vect_base<T,A> b{vb.alloc, new_capacity}; 
    b.sz = vb.sz; //size should not change?

    //these two lines can be optimised with a move...
    std::uninitialized_copy(vb.elem, vb.elem + vb.sz, b.elem); 
    destroy_elements();

    std::swap(vb,b); 
}

//change the size of the vector
template<typename T, typename A>
void vect<T,A>::resize(std::size_t newsize, T def) 
{
    reserve(newsize);
    if(vb.sz < newsize) std::uninitialized_fill(vb.elem + vb.sz, vb.elem + newsize, def); //size grows
    else for(std::size_t i = newsize; i < vb.sz; i++) ~vb.elem[i]; //size shrinks
    
    vb.sz = newsize;
}

//increase vector size by one
template<typename T, typename A> 
void vect<T,A>::push_back(const T& val) 
{

    std::cout<<"size before:\t"<<this->vb.sz<<'\n';
    std::cout<<"space before:\t"<<this->vb.space<<'\n';


    if(vb.space==0) {reserve(8);} //case where vector is initially empty
    else if(vb.sz == vb.space)  reserve(2* vb.space); //case where vector has nor free space left

    std::cout<<"size after:\t"<<this->vb.sz<<'\n';
    std::cout<<"space after:\t"<<this->vb.space<<'\n';

    vb.alloc.construct(&vb.elem[vb.sz], val);

    ++vb.sz;    


}



int main() {


    
    //fill(vect<int>{1,2});

    vect<char> v{'a','b'};
    vect<char> w{'x','y'};

    //vect<char> w{std::move(v)}; //should call move constructor

    w = std::move(v); //should call move assignment operator

    char tmp{'x'};
    w.push_back(tmp);

    
    //w.push_back('v');

    for(int i = 0;i<w.size();i++) {std::cout<<w[i]<<'\n';}

}





