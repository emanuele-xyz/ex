@echo off

rem setup msvc
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 > nul

rem get this file's path and add it to the global PATH
rem this file is supposed to be inside a folder containing all the scripts
rem that will be used for development
set PATH=%PATH%;%~dp0%

set PROJ=%CD%
