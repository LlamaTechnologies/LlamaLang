define dso_local i32 @main() #0 {
  // u64 num = 6
  %2 = alloca i64, align 8
  store i64 6, i64* %2, align 8
  
  // u64 byteSize = num * sizeof(s32)
  %4 = load i64, i64* %2, align 8
  %5 = call { i64, i1 } @llvm.umul.with.overflow.i64(i64 %4, i64 4)

  // struct {
  //   u64 mulResult
  //   bool overflown
  // }
  %6 = extractvalue { i64, i1 } %5, 1
  %7 = extractvalue { i64, i1 } %5, 0
  
  // mulResult = overflown ? -1 : mulResult
  %8 = select i1 %6, i64 -1, i64 %7

  // s32* newMem = mulResult
  %9 = call noalias nonnull i8* @_Znam(i64 %8) #3
  
  // s32* result = newMem
  %3 = alloca i32*, align 8
  %10 = bitcast i8* %9 to i32*
  store i32* %10, i32** %3, align 8

  // result[0] = 50
  %11 = load i32*, i32** %3, align 8
  %12 = getelementptr inbounds i32, i32* %11, i64 0
  store i32 50, i32* %12, align 4
  
  // return result[0]
  ret i32 %12
}
