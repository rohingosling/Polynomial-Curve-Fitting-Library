@ECHO OFF
REM -------------------------------------------------------------------------------
REM CLEAN.BAT - Remove all build artifacts
REM Usage:     Run from DOSBox command line in the test\ directory
REM -------------------------------------------------------------------------------

IF EXIST curve.obj     DEL curve.obj
IF EXIST ini.obj       DEL ini.obj
IF EXIST list.obj      DEL list.obj
IF EXIST queue.obj     DEL queue.obj
IF EXIST stack.obj     DEL stack.obj
IF EXIST csv.obj       DEL csv.obj
IF EXIST graph.obj     DEL graph.obj
IF EXIST test-1.obj    DEL test-1.obj
IF EXIST test-1.exe    DEL test-1.exe
IF EXIST test-2.obj    DEL test-2.obj
IF EXIST test-2.exe    DEL test-2.exe
IF EXIST build.log     DEL build.log
