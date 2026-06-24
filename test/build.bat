@ECHO OFF
REM -------------------------------------------------------------------------------
REM BUILD.BAT - Build script for Cubic Polynomial Curve Fitting Library
REM Compiler:  Borland Turbo C++ 3.1 (BCC)
REM Usage:     Run from DOSBox command line in the test\ directory
REM -------------------------------------------------------------------------------

REM --- Configuration -------------------------------------------------------------
REM Set TC to the Turbo C++ installation directory.
SET TC=C:\PROGRAMS\BCPP31
REM -------------------------------------------------------------------------------

ECHO ------------------------------------------------------------------------------ >> build.log
ECHO  Build Log                                                                     >> build.log
ECHO ------------------------------------------------------------------------------ >> build.log
ECHO.                                                                               >> build.log

ECHO [1/11] Compiling curve.cpp...                              >> build.log
BCC -ml -I%TC%\INCLUDE -I..\src -L%TC%\LIB -c ..\src\curve.cpp    >> build.log
IF ERRORLEVEL 1 GOTO FAIL
ECHO.                                                          >> build.log

ECHO [2/11] Compiling ini.cpp...                                >> build.log
BCC -ml -I%TC%\INCLUDE -I..\src -L%TC%\LIB -c ..\src\ini.cpp      >> build.log
IF ERRORLEVEL 1 GOTO FAIL
ECHO.                                                          >> build.log

ECHO [3/11] Compiling list.cpp...                               >> build.log
BCC -ml -I%TC%\INCLUDE -I..\src -L%TC%\LIB -c ..\src\list.cpp     >> build.log
IF ERRORLEVEL 1 GOTO FAIL
ECHO.                                                          >> build.log

ECHO [4/11] Compiling queue.cpp...                              >> build.log
BCC -ml -I%TC%\INCLUDE -I..\src -L%TC%\LIB -c ..\src\queue.cpp    >> build.log
IF ERRORLEVEL 1 GOTO FAIL
ECHO.                                                          >> build.log

ECHO [5/11] Compiling stack.cpp...                              >> build.log
BCC -ml -I%TC%\INCLUDE -I..\src -L%TC%\LIB -c ..\src\stack.cpp    >> build.log
IF ERRORLEVEL 1 GOTO FAIL
ECHO.                                                          >> build.log

ECHO [6/11] Compiling csv.cpp...                               >> build.log
BCC -ml -I%TC%\INCLUDE -I..\src -L%TC%\LIB -c ..\src\csv.cpp      >> build.log
IF ERRORLEVEL 1 GOTO FAIL
ECHO.                                                          >> build.log

ECHO [7/11] Compiling graph.cpp...                             >> build.log
BCC -ml -I%TC%\INCLUDE -I..\src -L%TC%\LIB -c ..\src\graph.cpp    >> build.log
IF ERRORLEVEL 1 GOTO FAIL
ECHO.                                                          >> build.log

ECHO [8/11] Compiling test-1.cpp...                              >> build.log
BCC -ml -I%TC%\INCLUDE -I..\src -L%TC%\LIB -c test-1.cpp          >> build.log
IF ERRORLEVEL 1 GOTO FAIL
ECHO.                                                          >> build.log

ECHO [9/11] Linking test-1.exe...                               >> build.log
BCC -ml -L%TC%\LIB test-1.obj curve.obj ini.obj list.obj csv.obj graph.obj GRAPHICS.LIB >> build.log
IF ERRORLEVEL 1 GOTO FAIL
ECHO.                                                          >> build.log

ECHO [10/11] Compiling test-2.cpp...                             >> build.log
BCC -ml -I%TC%\INCLUDE -I..\src -L%TC%\LIB -c test-2.cpp          >> build.log
IF ERRORLEVEL 1 GOTO FAIL
ECHO.                                                          >> build.log

ECHO [11/11] Linking test-2.exe...                               >> build.log
BCC -ml -L%TC%\LIB test-2.obj curve.obj ini.obj list.obj csv.obj graph.obj GRAPHICS.LIB >> build.log
IF ERRORLEVEL 1 GOTO FAIL
ECHO.                                                          >> build.log

ECHO ------------------------------------------------------------------------------ >> build.log
ECHO  Build complete.                                                               >> build.log
ECHO ------------------------------------------------------------------------------ >> build.log

ECHO Build succeeded.
GOTO END

:FAIL
ECHO Build FAILED. See build.log for details.

:END
