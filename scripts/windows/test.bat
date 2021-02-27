set test_rule=%1
set test_file=%2

if not exist "..\\..\\bin\\antlr_test\\" mkdir "..\\..\\bin\\antlr_test\\"
java -jar ../antlr-4.9.1-complete.jar -no-listener -visitor ../../LlamaLang.g4 -o ../../bin/antlr_test

cd ../../bin/antlr_test
javac LlamaLang*.java

java org.antlr.v4.gui.TestRig LlamaLang %test_rule% .%test_file% -gui
