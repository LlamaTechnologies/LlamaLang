set test_rule=%1
set test_file=%2

java org.antlr.v4.Tool LlamaLang.g4  -o ./Java

cd ./Java/
javac LlamaLang*.java

java org.antlr.v4.gui.TestRig LlamaLang %test_rule% .%test_file% -gui
