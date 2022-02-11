#include <initializer_list>
#include <memory>

//vect_base is an auxilliary class that represents the notion of memory within a vector
//It is an example of the RAII technique
template<typename T, typename A = std::allocator<T> >
struct vect_base{

    A alloc;
    std::size_t sz;
    T* elem;
    std::size_t space;

    inline vect_base(const A& a, std::size_t n) //constructor
        :alloc{a}, elem{alloc.allocate(n)}, sz{n}, space{n} {}
    
    ~vect_base() {alloc.deallocate(elem, space);} //destructor

    vect_base(const vect_base& other) = delete; // no copy operations allowed, because unique ownership.
    vect_base& operator=(const vect_base& other) = delete;

    vect_base(vect_base&& other); //move operations
    vect_base& operator=(vect_base&& other);    

};


template<typename T, typename A = std::allocator<T> >
class vect {

    vect_base<T,A> vb; //data 
    //void destroy_elements();

public:
//default?
explicit vect(std::size_t n, const T& def = T(), const A& all = A()); //construct vector of specified size
vect(std::initializer_list<T> list, const A& all = A()); //initializer-list constructor

vect(const vect& other); //copy constructor
vect& operator=(const vect& other); //copy assignment

vect(vect&& other); //move constructor
vect& operator=(vect&& other); //move assignment

~vect() ;

void reserve(std::size_t newalloc); //add capacity
void resize(std::size_t newsize,  T def = T()); //change size
void push_back(const T& val);
//void push_back(T&& tmp);

//helpers
std::size_t size() const {return vb.sz;}
std::size_t capacity() const {return vb.space;}
T operator[](std::size_t n) const {return vb.elem[n];}; //for const vect
T& operator[](std::size_t n) {return vb.elem[n];}; //for non-const


};


// using member initializer lists prevents default initialization + assignments... uses direct initialization instdead

//memcpy only works with data that doesn;t have explicit constructors (works for float, but not std::string..)
//prefer std::copy to memcpy, compiler can replace the std::copy with memcpy if necessary...

//std::move is safe because a copy will be called if a move construcor does not exist

// When new is used to allocate memory for a C++ class object, the object's constructor is called after the memory is allocated.
// allocators allow for the separation of allocating memory and object construction (the new operator does both)

//uninitialized_copy & friends guarantee thet partially constructed subobjects will be destroyed if an exception is thrown


/* Design RULES

- default values go in .h file

- the main principle of RAII is to give ownership of any heap-allocated resource (e.g. dynamically allocated memory)
to a stack-allocated object, whose destructor contains the code to delete or free the resource and also any associated cleanup code.

- Using RAII, we define a vect_base class that handles the memory for a vector. Each vector then contains a vector_base as a member.

*/


/*TO-DO

- what happens to allocator in constructors?

- Can we use move semantics in the initizlier-list constructor? (apparently not!)

-range-checking?

- Implement emplace_back (variadic templates)
- pop_back
*/