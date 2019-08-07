# CppLinq
A simple LINQ library for C++.
This is forked from https://github.com/berrysoft/CppLinq
The old README file is renamed to OldREADME.md
## Changes from Berrysoft
The old test files can be found in the 'test' folder.  The new tests use [catch2](https://github.com/catchorg/Catch2/releases/download/v2.9.1/catch.hpp) and can be found in the catch2 folder. This fork also fixes an isue regarding "reverse" raised at the original repo.
Notice that there is no `make` file. Each `cpp` file  in `catch2` folder is a separate test and can be complied in g++ as in 
    
    g++ -std=c++17 -I<your_include_folder> <filename>.cpp

Your include folder must the `linq` and `catch2` sub-folders


