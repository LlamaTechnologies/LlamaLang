set test_file=%1

if not exist "..\\..\\bin\\antlr_test\\" mkdir "..\\..\\bin\\antlr_test\\"
java -jar ../antlr-4.9.1-complete.jar -listener -no-visitor ../../ModuleRetriever.g4 -o ../../bin/antlr_test

cd ../../bin/antlr_test
javac ModuleRetriever*.java

java org.antlr.v4.gui.TestRig ModuleRetriever validSource .%test_file% -gui
