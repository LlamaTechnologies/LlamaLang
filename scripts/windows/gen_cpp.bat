java -jar ../antlr-4.9.1-complete.jar -listener -no-visitor -Dlanguage=Cpp -package "llang" ../../ModuleRetriever.g4 -o ../../src/antlr
java -jar ../antlr-4.9.1-complete.jar -no-listener -visitor -Dlanguage=Cpp -package "llang" ../../LlamaLang.g4 -o ../../src/antlr
