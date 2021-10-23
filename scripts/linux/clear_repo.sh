if [-d "../../project"]; then
  rm -rf ../../project
  echo "Removed project folder"
fi

rm ../../bin/*.a
echo "Removed executables from bin folder"

rm ../../bin/*.bc
echo "Removed bit code files from bin folder"

