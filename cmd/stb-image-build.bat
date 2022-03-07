@echo off

rem ///////////////////////////////////////////////////////////////////////////
rem crash if no arguments are specified
rem ///////////////////////////////////////////////////////////////////////////

if "%~1"=="" (goto :crash) else (goto :continue)

:crash
echo Pass either 'debug' or 'release' as arguments
goto :exit

:continue

set STB_IMAGE= ..\vendor\stb_image

rem ///////////////////////////////////////////////////////////////////////////
rem source directories
rem ///////////////////////////////////////////////////////////////////////////

set SRC= %STB_IMAGE%\src

rem ///////////////////////////////////////////////////////////////////////////
rem source files
rem ///////////////////////////////////////////////////////////////////////////

set SOURCES=
set SOURCES= %SOURCES% %SRC%\stb_image.c

rem ///////////////////////////////////////////////////////////////////////////
rem compiler flags
rem ///////////////////////////////////////////////////////////////////////////

set FLAGS=

rem ---------------------------------------------------------------------------
rem include directories (/I dir)
rem ---------------------------------------------------------------------------

set FLAGS= %FLAGS% /I %STB_IMAGE%\include

rem ---------------------------------------------------------------------------
rem defines preprocessor symbol (/Dsymbol)
rem ---------------------------------------------------------------------------

rem set FLAGS= %FLAGS% /D_CRT_SECURE_NO_WARNINGS
rem set FLAGS= %FLAGS% /D_GLFW_WIN32

rem ---------------------------------------------------------------------------
rem misc
rem ---------------------------------------------------------------------------

rem disable msvc logo
set FLAGS= %FLAGS% /nologo
rem displays the full path of source code files passed
set FLAGS= %FLAGS% /FC
rem builds multiple source files concurrently
set FLAGS= %FLAGS% /MP
rem compiles without linking
set FLAGS= %FLAGS% /c

rem ---------------------------------------------------------------------------
rem disable some c++ RTTI and exceptions
rem ---------------------------------------------------------------------------

rem disable RTTI
set FLAGS= %FLAGS% /GR-
rem disbales C++ exceptions
set FLAGS= %FLAGS% /EHa-

rem ---------------------------------------------------------------------------
rem .pdb file name
rem ---------------------------------------------------------------------------

set FLAGS= %FLAGS% /Fdstb_image.pdb

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
rem rem ignore spectre mitigation insertion warning
set FLAGS= %FLAGS% /wd5045
rem ignore bytes padding added warning
set FLAGS= %FLAGS% /wd4820

rem ///////////////////////////////////////////////////////////////////////////
rem Compile
rem ///////////////////////////////////////////////////////////////////////////

if not exist .\build mkdir .\build
pushd .\build
cl %FLAGS% %SOURCES%
popd

rem ///////////////////////////////////////////////////////////////////////////
rem Lib
rem ///////////////////////////////////////////////////////////////////////////

rem reset flags
set FLAGS=

rem ///////////////////////////////////////////////////////////////////////////
rem Object files
rem ///////////////////////////////////////////////////////////////////////////

set OBJS=
set OBJS= %OBJS% stb_image.obj

rem ---------------------------------------------------------------------------
rem misc
rem ---------------------------------------------------------------------------

rem suppresses display of the copyright message
set FLAGS= %FLAGS% /NOLOGO
rem display infos
set FLAGS= %FLAGS% /VERBOSE

rem ---------------------------------------------------------------------------
rem .lib file name
rem ---------------------------------------------------------------------------

if "%1" == "debug"   (set FLAGS= %FLAGS% /OUT:stb_image.dbg.lib)
if "%1" == "release" (set FLAGS= %FLAGS% /OUT:stb_image.rls.lib)

rem ///////////////////////////////////////////////////////////////////////////
rem Build static library
rem ///////////////////////////////////////////////////////////////////////////

if not exist .\build mkdir .\build
pushd .\build
lib %OBJS% %FLAGS%
popd

rem ///////////////////////////////////////////////////////////////////////////
rem exit
rem ///////////////////////////////////////////////////////////////////////////

:exit
exit /b
