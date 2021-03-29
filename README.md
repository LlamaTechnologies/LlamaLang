# LlamaLang
This language is defined with `game development` in mind.  
Also tries to `standardize implementation` details so many other implementations are compatible between them in contrast to `c++`.  
I heavily inspired by Jonathan Blows language, `Jai`.

Features:
- Annotations like Java.
- Syntax `Go` inspired.
- `Reflection`
- Compile time functions execution.
- Memory management fully up to the programmer.
- Strongly typed.
- Compiled.

See the [wiki](https://github.com/Pablo96/LlamaLangGo/wiki) (OUTDATED) for language definition.  
For an up-to-date yet informal language definition see the `wiki` folder.

## Implementation details
The compiler is made using `ANTLR v4` with `C++` target.  
It targets `LLVM IR`, so it can be compiled for every hardware supported by LLVM and have debugging capabilities.

## Example
An example of the last supported syntax can be found in ´tests´ folder.

## Supported IDE's
As to date is not yet supported:
- `Visual Studio Code`

Started 23/12/2020

# Building
## Compile
### Requisites
- antlr 4.9.1 c++ runtime source code
- llvm source code
- cmake 3.1+
- vs2019(it should work on gcc and clang though is not tested)
### Steps
- Compile antlr c++ runtime static lib, put them in the lib folder

- Compile llvm (you only need to compile llvm and clang projects)
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
- On the LlamaLang repo folder run:
    ```
    mkdir project
    ```
    ```
    cd project
    ```
    ```
    cmake -DLLVM_DIR='llvm/installation/folder' ..
    ```
- Now you can open the sln file in the project folder and build the solution from vs
## Antlr tree viewer
just run these commands
```
cd ./scripts/windows
```
```
test.bat sourceFile ./../tests/test.llang
```
