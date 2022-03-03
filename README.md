@[TOC](Bilinear Pair Accumulator)

# Introduction
The code of bilinear pair accumulator used by the paper “A Blockchain-based Arbitrable Data Auditing Scheme for IoT-Cloud Services”。

# Configuration 
In order to perform the polynomial operations in bilinear pair accumulators, we introduce the PBC library in this scheme.And the PBC library is an open source C library built on the GMP library that performs the mathematical operations underlying pairing-based cryptosystems. 

# Development Environment 
The code's development environment is JetBrains CLion. And Client’s server proxy was collocated with CSP server on 8 cores of a machine with 2.60GHz Intel i7-6700HQ processors and 12GB of RAM, running Ubuntu Linux.

# Usage
Firstly, compiling and executing the file "main.cpp" to generate random file data and encrypt it.
Then running the file "PBC.cpp" to test the algorithm's correctness and efficiency.


# Contact 
The code is written by Yifang Zhang. He is currently pursuing the master's degree in Department of Artificial Intelligence, Beijing Normal University of China. 
Mail: zyfyydgq@mail.bnu.edu.cn
