I implemented a vector class mimicking the std::vector class from the std library. I did this hoping that it would help me understand some of the lower-level functions
in C++, as well as the design decisions made along the way. The code is mostly based on that from the book "The C++ Programming Language"

**Move Semantics**
- An object that has been move from must be left in a 'valid' state. In other words, it must be left in a state such that its destructor can run successfully. 


TO-DO
- Implement iterators
- 
- 'copy-and-swap'

