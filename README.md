# ArrayBlockingQueue

Implementation of ArrayBlockingQueue in C++ conforming to implementation standard of Java.
use cmake to build the code and examples.
1. cmake CMakeLists.txt
2. make clean
3. make 

# The only Feature that is not inline with Java Implementation is the iterator method thread safety.
I looked at the Java 8 Implementation code for Iterator and there is hell lot of work behind to keep track of iterator pointers using weak references to the items in queue and sweeping them and using locks for consistency. I felt that
a queue does not need iteration in most cases except a few where debugging may be needed. 

Hence the current iterator method should generally suffice. If reader has interest he is free to implement a 
thread safe version.
