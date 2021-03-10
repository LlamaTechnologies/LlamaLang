# LlamaLang
This language is defined with game development in mind.  
Also tries to standardize some implementation details so many other implementations are compatible between them in contrast to c++.  
I heavily inspired by Jonathan Blows language, Jai.

Features:
- Annotations like Java.
- Syntax Go/TypeScript inspired.
- Reflection
- compile time functions execution.
- Memory management fully up to the programmer.
- Strongly typed.
- Compiled.

See the [wiki](https://github.com/Pablo96/LlamaLangGo/wiki) for language definition

## Implementation details
The compiler is made using ANTLR v4 with Go target.  
It targets LLVM IR, so it can be compiled for every hardware supported by LLVM and have debugging capabilities.

## Example
An example of the grammar can be found in ´examples´ folder.

## Supported IDE's
As to date none is supported, but these are the future targets in order of priority:
- Visual Studio Code
- Visual Studio

Started 23/12/2020
