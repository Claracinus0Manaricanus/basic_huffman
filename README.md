# What

A basic implementation of huffman coding, tests has an example usage of the implementation. This is a library so you can use it in your own projects for huffman compression.

# Build

Make sure you are in the projects root directory.  

Simply do:  
`cmake -S . -B build`  
`cmake --build build`  
to build the library  
  
If you want to build the test you can do:  
`cmake -S . -B build -DCM_TEST=1`  
`cmake --build build`  
and run it with:  
`./build/test`
