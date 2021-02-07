# LlamaLang
This is a language for game development heavily inspired by Jonathan Blows language, Jai.
- Annotations like Java.
- Sintax Go/TypeScript inspired.
- Reflection and function compile time excecution.
- Memory is completly managed by the programmer.
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
