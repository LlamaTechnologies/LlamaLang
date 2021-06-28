# Llama Lang
This language is defined with `game development` in mind.  
Also tries to `standardize implementation` details so many other implementations are compatible between them in contrast to `c++`.  
I heavily inspired by Jonathan Blows language, `Jai`.

## Features
- Annotations like Java.
- Syntax `Go` inspired.
- `Reflection`
- Compile time functions execution.
- Memory management fully up to the programmer.
- Strongly typed.
- Compiled.

## Build
### Requisites
- llvm source code
- cmake 3.1+
- vs2019(it should work on gcc and clang though is not tested)
### Steps
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
- Go to the `scripts` folder and run the `cmake_project.bat`.  
  `NOTE`: if it cant find llvm make sure the llvm install dir is ok in the script file.
- Now you can open the sln file in the project folder and build the solution from vs

## Usage/Examples
```llamalang
#main
fn app() i32 {
  print("Hello world!")
  return 0
}
```
An example of the last supported syntax can be found in ´tests´ folder.

## Documentation
See the [wiki](https://github.com/Pablo96/LlamaLangGo/wiki) (OUTDATED) for language definition.  
For an up-to-date yet informal language definition see the `wiki` folder.


## License

[GNU AGPLv3](https://choosealicense.com/licenses/agpl-3.0/)

## Roadmap
- basic language
- language specific features
- standard library
- support `Visual Studio Code`

development Started 23/12/2020
