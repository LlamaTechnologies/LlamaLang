# LlamaLang
This language is defined with game development in mind. Also tries to standardize some implementation details so many other implementations are compatible between them in contrast to c++. Its features are heavily inspired by Jonathan Blows language, Jai.
- Annotations like Java.
- Sintax Go/TypeScript inspired.
- Reflection
- compile time funcitons excecution.
- Memory management fully up to the programmer.
- Strongly typed.
- Compiled.

See the [wiki](https://github.com/Pablo96/LlamaLang/wiki) for language definition  

## Implementation details
The compiler is made using ANTLR v4 with C# target and it targets LLVM IR so it can be compiled for every hardware supported by LLVM and have debuging capabilities.

## Example
An example of the gramar can be found at the files test.def and test.decl.

## Suported IDE's
As to date none is supported but these are the future targets in order of priority:
- Visual Studio Code
- Visual Studio

Started 23/12/2020
