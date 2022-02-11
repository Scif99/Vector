I implemented a vector class mimicking the std::vector class from the std library. I did this hoping that it would help me understand some of the lower-level functions
in C++, as well as the design decisions made along the way. The code is mostly based on that from the book "The C++ Programming Language"

**Move Semantics**
- An object that has been move from must be left in a 'valid' state. In other words, it must be left in a state such that its destructor can run successfully. 
- Since C++11, the std::swap function makes use of move semantics. If the swapped objects are non-moveable, swap will revert to copies instead.

**Copy-and-Swap**
- using swap can be used as an alternative to implement assignment operators
- However, it cannot be used in constructors (e.g. move constructor) as the other object will not be in a valid state after the swap. 


**RAII**
- The auxilliary class vect_base is an example of using RAII. It handles memory for the vector.
- For example, a swap is used to implement the copy assignment operator for vect.
- The other vector is then destroyed as it goes out of scope. This then calls the destructors of vect's members, namely vect_base, which safely deallocates memory.


TO-DO
- Implement iterators
- 
- 'copy-and-swap'

