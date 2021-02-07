test_rule=$1;
test_file=$2;

java -Xmx500M -cp "/usr/local/lib/java/antlr-4.9-complete.jar" org.antlr.v4.Tool LlamaLangParser.g4 -o ./Java;
cd ./Java/;
javac LlamaLang*.java;
java org.antlr.v4.gui.TestRig LlamaLang $test_rule .$test_file -gui;
