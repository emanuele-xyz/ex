@echo off

rem ///////////////////////////////////////////////////////////////////////////
rem crash if no arguments are specified
rem ///////////////////////////////////////////////////////////////////////////

if "%~1"=="" (goto :crash) else (goto :continue)

:crash
echo Pass either 'debug' or 'release' as arguments
goto :exit

:continue

set GLFW= ..\vendor\glfw

rem ///////////////////////////////////////////////////////////////////////////
rem source directories
rem ///////////////////////////////////////////////////////////////////////////

set SRC= %GLFW%\src

rem ///////////////////////////////////////////////////////////////////////////
rem source files
rem ///////////////////////////////////////////////////////////////////////////

set SOURCES=
set SOURCES= %SOURCES% %SRC%\context.c
set SOURCES= %SOURCES% %SRC%\init.c
set SOURCES= %SOURCES% %SRC%\input.c
set SOURCES= %SOURCES% %SRC%\monitor.c
set SOURCES= %SOURCES% %SRC%\wgl_context.c
set SOURCES= %SOURCES% %SRC%\egl_context.c
set SOURCES= %SOURCES% %SRC%\osmesa_context.c
set SOURCES= %SOURCES% %SRC%\win32_init.c
set SOURCES= %SOURCES% %SRC%\win32_joystick.c
set SOURCES= %SOURCES% %SRC%\win32_monitor.c
set SOURCES= %SOURCES% %SRC%\win32_thread.c
set SOURCES= %SOURCES% %SRC%\win32_time.c
set SOURCES= %SOURCES% %SRC%\win32_window.c
set SOURCES= %SOURCES% %SRC%\window.c
set SOURCES= %SOURCES% %SRC%\vulkan.c

rem ///////////////////////////////////////////////////////////////////////////
rem compiler flags
rem ///////////////////////////////////////////////////////////////////////////

set FLAGS=

rem ---------------------------------------------------------------------------
rem include directories (/I dir)
rem ---------------------------------------------------------------------------

set FLAGS= %FLAGS% /I %GLFW%\include

rem ---------------------------------------------------------------------------
rem defines preprocessor symbol (/Dsymbol)
rem ---------------------------------------------------------------------------

set FLAGS= %FLAGS% /D_CRT_SECURE_NO_WARNINGS
set FLAGS= %FLAGS% /D_GLFW_WIN32

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
rem .pdb file name
rem ---------------------------------------------------------------------------

set FLAGS= %FLAGS% /Fdglfw.pdb

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
rem ignore relative include path warning
set FLAGS= %FLAGS% /wd4464
rem ignore bytes padding added warning
set FLAGS= %FLAGS% /wd4820
rem ignore not defined preprocessor symbol
set FLAGS= %FLAGS% /wd4668
rem ignore no function prototype given: converting '()' to '(void)'
set FLAGS= %FLAGS% /wd4255
rem ignore nonstandard extension used : nameless struct/union warning
set FLAGS= %FLAGS% /wd4201
rem ignore unreferenced formal parameter warning
set FLAGS= %FLAGS% /wd4100
rem ignore function/data pointer conversion in expression warning
set FLAGS= %FLAGS% /wd4152
rem ignore possible loss of data warning (they are the same warning twice)
set FLAGS= %FLAGS% /wd4244 /wd4242
rem ignore enumerator 'identifier' in switch of enum 'enumeration' is not
rem explicitly handled by a case label warning
set FLAGS= %FLAGS% /wd4061
rem function selected for inline expansion
set FLAGS= %FLAGS% /wd4711

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

if "%1" == "debug"   (set FLAGS= %FLAGS% /OUT:glfw.dbg.lib)
if "%1" == "release" (set FLAGS= %FLAGS% /OUT:glfw.rls.lib)

rem ///////////////////////////////////////////////////////////////////////////
rem Build static library
rem ///////////////////////////////////////////////////////////////////////////

if not exist .\build mkdir .\build
pushd .\build
lib *.obj %FLAGS%
popd

rem ///////////////////////////////////////////////////////////////////////////
rem exit
rem ///////////////////////////////////////////////////////////////////////////

:exit
exit /b
