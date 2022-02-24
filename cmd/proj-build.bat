@echo off

rem ///////////////////////////////////////////////////////////////////////////
rem source directories
rem ///////////////////////////////////////////////////////////////////////////

set SRC= ..\src
set INC= ..\include

rem ///////////////////////////////////////////////////////////////////////////
rem compiler flags
rem ///////////////////////////////////////////////////////////////////////////

set FLAGS=

rem ---------------------------------------------------------------------------
rem misc
rem ---------------------------------------------------------------------------

rem disable msvc logo
set FLAGS= %FLAGS% /nologo
rem displays the full path of source code files passed
set FLAGS= %FLAGS% /FC
rem builds multiple source files concurrently
set FLAGS= %FLAGS% /MP

rem ---------------------------------------------------------------------------
rem debug build compiler flags
rem ---------------------------------------------------------------------------

rem generate debugging information
if "%1" == "debug" (set FLAGS= %FLAGS% /Zi)
rem compiles to create a debug multithreaded executable file using LIBCMTD.lib
if "%1" == "debug" (set FLAGS= %FLAGS% /MTd)
rem disables optimization and speeds compilation
if "%1" == "debug" (set FLAGS= %FLAGS% /Od)

rem ---------------------------------------------------------------------------
rem release build compiler flags
rem ---------------------------------------------------------------------------

rem compiles to create a multithreaded executable file, by using LIBCMT.lib
if "%1" == "release" (set FLAGS= %FLAGS% /MT)
rem creates fast code
if "%1" == "release" (set FLAGS= %FLAGS% /O2)
rem generates intrinsic functions
if "%1" == "release" (set FLAGS= %FLAGS% /Oi)

rem ---------------------------------------------------------------------------
rem warnings
rem ---------------------------------------------------------------------------

rem enable all warnings
set FLAGS= %FLAGS% /Wall
rem treat warnings as erros
set FLAGS= %FLAGS% /WX
rem ignore spectre mitigation insertion warning
set FLAGS= %FLAGS% /wd5045
rem ignore function not inlined warning
set FLAGS= %FLAGS% /wd4710

rem ---------------------------------------------------------------------------
rem includes
rem ---------------------------------------------------------------------------

set FLAGS= %FLAGS% /I %INC%

rem ///////////////////////////////////////////////////////////////////////////
rem linker flags
rem ///////////////////////////////////////////////////////////////////////////

set LINKER= /link
set LINKER= %LINKER% /release

rem ///////////////////////////////////////////////////////////////////////////
rem source files
rem ///////////////////////////////////////////////////////////////////////////

set SOURCES=
set SOURCES= %SOURCES% %SRC%\main.c

rem ///////////////////////////////////////////////////////////////////////////

if not exist .\build mkdir .\build
pushd .\build
cl %FLAGS% %SOURCES% %LINKER%
popd
