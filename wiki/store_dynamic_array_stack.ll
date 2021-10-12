define dso_local i32 @main() #0 {
  // s32 retVal = 0
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  
  // s64 num = 6
  %2 = alloca i64, align 8
  store i64 6, i64* %2, align 8
  
  // s32 result[num]
  %3 = alloca i8*, align 8
  
  // tmpNum = num
  %5 = load i64, i64* %2, align 8

  // save stack
  %6 = call i8* @llvm.stacksave()

  // %6 into %3
  store i8* %6, i8** %3, align 8
  // get a num sized array
  %7 = alloca i32, i64 %5, align 16
  
  // tmpNum2 = tmpNum
  %4 = alloca i64, align 8
  store i64 %5, i64* %4, align 8

  // result[0] = 50
  %8 = getelementptr inbounds i32, i32* %7, i64 0
  store i32 50, i32* %8, align 16
  
  // retVal = result[0]
  %9 = getelementptr inbounds i32, i32* %7, i64 0
  %10 = load i32, i32* %9, align 16
  store i32 %10, i32* %1, align 4
   
  // restore the stack around %3
  %11 = load i8*, i8** %3, align 8
  call void @llvm.stackrestore(i8* %11)
  
  // return retVal
  %12 = load i32, i32* %1, align 4
  ret i32 %12
}
