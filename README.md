# FST-Cplusplus
A universal C++ library for FST inverted index structures

Introduction
FST is a commonly used inverted index data structure in Lucene. It not only ensures the compression efficiency O(K) of prefix tree (K is the length of keywords), but also adds suffix index compression to reduce memory consumption. Using mixed Chinese and English short text test, compression rate is 3.4%.But Lucene is implemented in Java, where a version of Cplusplus is implemented with some minor improvements.


This project consists of two parts, reader and writer. 

Research Platform
linux

Programming Language
C/C++.

Research Topic
1 C++ library for FST inverted index structures

Build and Run
Build FST:

$ make and run descheduler:

you can put this project into a C++ program, then use CMAKE to build this project 

