# Llama Lang
This language is defined with `game development` in mind.  
Also tries to `standardize implementation` details so many other implementations are compatible between them in contrast to `c++`.  
I heavily inspired by Jonathan Blows language, `Jai`.

## Features
- Syntax `Go` and `Rust` inspired.
- Optional full runtime `Reflection`
- Compile time execution.
- Clearer `manual memory management`.
- Strongly `static typed`.
- `Pre-compiled`.
- integrated `build system` and `test framework`.

# Example (hello world)
```llamalang
#main
fn MyHelloWorldApp() S32 {
  print("Hello world!")
  ret 0
}
```
An example of the last supported syntax can be found in the `bin/examples` folder.

# Build
## Prerequisites
### Windows
- llvm 12 source code
- cmake 3.1+
- vs2019(it should work on gcc and clang though is not tested)  

### Linux
- llvm 12
- clang 12  

## Steps
### Windows
- Compile llvm (you only need to compile llvm project)
    #### On the llvm folder run:
     ```
     mkdir projectFiles
     ```
     ```
    cd projectFiles
    ```
    ```
    cmake -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON --Thost=x64 --build ..
    ```
    ```
    cmake --build . --target install
    ```
- Go to the `scripts` folder and run the `cmake_project.bat`.  
  `NOTE`: if it cant find llvm make sure the llvm install dir is ok in the script file.
- Now you can open the sln file in the project folder and build the solution from vs  

### Linux
- Go to the `scripts` folder and run the `init_repo_and_cmake_project.sh`.  
- TODO

# Documentation
See the [wiki](https://github.com/Pablo96/LlamaLangGo/wiki) (OUTDATED) for language definition.  
For an up-to-date yet informal language definition see the `wiki` folder.

# License
[GNU AGPLv3](https://choosealicense.com/licenses/agpl-3.0/)

# Roadmap
- basic language
- - functions
- - globals and variables
- - bool|numbers constants
- - if/elif/else
- - for loops
- - pointers
- - arrays constants
- - structs|unions
- - build system
- - memory management
- debugger
- nice to have features
- - generics (maybe?)
- - some OOP concepts ?
- - lambdas
- - named capture-based inner scopes in functions
- - data oriented programming features
- - dynamic libraries
- - threading system (actors?)
- - reflection
- - complete build system
- - test framework
- standard library
- some other libraries
- support `Visual Studio Code` and `NeoVim`
- - windowing library
- - UI library
- - async logger library
- - regex library
- - vector math library
- - cmd line library
- - space partition structures library
- Tools
- - C headers to llamalang extern functions
- - Package manager ?

# Cool fact ?
development Started 23/12/2020
