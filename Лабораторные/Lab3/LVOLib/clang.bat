clang -target x86_64-pc-win32 -I .\include\ -c -o .\bin\LVO.o .\src\LVO.c &&
llvm-ar cr .\bin\lvolib.lib .\bin\LVO.o &&
llvm-ranlib .\bin\lvolib.lib