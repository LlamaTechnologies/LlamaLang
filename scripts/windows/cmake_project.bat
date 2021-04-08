if not exist "../../project" mkdir "../../project"

cd ../../project

REM change the llvm directory to your llvm intall directory
cmake .. -DLLVM_DIR="C:/Program Files/LLVM/lib/cmake/llvm"