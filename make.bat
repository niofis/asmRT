@echo off

 
if exist "asmRT.obj" del "asmRT.obj"
REM if exist "asmRT.exe" del "asmRT.exe"

c:\fasm\fasm "asmRT.asm"
REM c:\fasm\fasm -s "debug.fas" "asmRT.asm"
REM listing debug.fas listing.txt
REM symbols debug.fas symbols.txt

goto TheEnd

:errlink
 echo _
echo Link error
goto TheEnd

:errasm
 echo _
echo Assembly Error
goto TheEnd

:TheEnd
