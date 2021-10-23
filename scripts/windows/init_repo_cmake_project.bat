if not exist "../../extern/googletest" (
  git submodule init
  git submodule update
  echo "Cloned submodules"
)

if not exist "../../project" (
  mkdir "../../project"
  echo "Created 'project' folder"
)

cd ../../project

cmake .. -DLLVM_DIR="E:\LLVM_Lib\lib\cmake\llvm"

