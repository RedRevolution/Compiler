cd C:\Users\Red\Desktop\X6-122\compiler\Red
copy testfile1.txt C:\Users\Red\Desktop\Compiler\testfile.txt
copy output1.txt C:\Users\Red\Desktop\Compiler\answer.txt
cd C:\Users\Red\Desktop\Compiler
Compiler.exe

fc /N output.txt answer.txt
if errorlevel 1 pause