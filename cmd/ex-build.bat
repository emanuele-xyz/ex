@echo off

rem ///////////////////////////////////////////////////////////////////////////
rem crash if no arguments are specified
rem ///////////////////////////////////////////////////////////////////////////

if "%~1"=="" (goto :crash) else (goto :continue)

:crash
echo Pass either 'debug' or 'release' as arguments
goto :exit

:continue

rem ///////////////////////////////////////////////////////////////////////////
rem source directories
rem ///////////////////////////////////////////////////////////////////////////

set SRC= ..\src

rem ///////////////////////////////////////////////////////////////////////////
rem source files
rem ///////////////////////////////////////////////////////////////////////////

rem ---------------------------------------------------------------------------
rem project source files
rem ---------------------------------------------------------------------------

set SOURCES=
set SOURCES= %SOURCES% %SRC%\Main.cpp
set SOURCES= %SOURCES% %SRC%\App\App.cpp
set SOURCES= %SOURCES% %SRC%\App\Input.cpp
set SOURCES= %SOURCES% %SRC%\App\Settings.cpp
set SOURCES= %SOURCES% %SRC%\App\State.cpp
set SOURCES= %SOURCES% %SRC%\App\Window.cpp
set SOURCES= %SOURCES% %SRC%\Game\Game.cpp
set SOURCES= %SOURCES% %SRC%\Game\Graphics.cpp
set SOURCES= %SOURCES% %SRC%\Systems\Logger.cpp

rem ///////////////////////////////////////////////////////////////////////////
rem compiler flags
rem ///////////////////////////////////////////////////////////////////////////

set FLAGS=

rem ---------------------------------------------------------------------------
rem include directories (/I dir)
rem ---------------------------------------------------------------------------

set FLAGS= %FLAGS% /I ..\include
set FLAGS= %FLAGS% /I ..\vendor\glfw\include
set FLAGS= %FLAGS% /I ..\vendor\glad\include
set FLAGS= %FLAGS% /I ..\vendor\cglm\include
set FLAGS= %FLAGS% /I ..\vendor\stb_image\include

rem ---------------------------------------------------------------------------
rem defines preprocessor symbol (/Dsymbol)
rem ---------------------------------------------------------------------------

rem define either EX_DEBUG or EX_RELEASE
if "%1" == "debug"   (set FLAGS= %FLAGS% /DEX_DEBUG)
if "%1" == "release" (set FLAGS= %FLAGS% /DEX_RELEASE)

rem disbale standard library deprecation
rem set FLAGS= %FLAGS% /D_CRT_SECURE_NO_WARNINGS

rem TODO: do i need them aligned or not?
rem disable cglm alignment requirement
rem set FLAGS= %FLAGS% /DCGLM_ALL_UNALIGNED

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
rem disable some c++ RTTI and exceptions
rem ---------------------------------------------------------------------------

rem disable RTTI
set FLAGS= %FLAGS% /GR-
rem disbales C++ exceptions
set FLAGS= %FLAGS% /EHa-

rem ---------------------------------------------------------------------------
rem .pdb file name
rem ---------------------------------------------------------------------------

set FLAGS= %FLAGS% /Fdex.pdb

rem ---------------------------------------------------------------------------
rem .exe file name
rem ---------------------------------------------------------------------------

set FLAGS= %FLAGS% /Feex.exe

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
rem ignore structure was padded due to __declspec(align())
set FLAGS= %FLAGS% /wd4324
rem ignore 'noexcept' used with no exception handling mode specified
set FLAGS= %FLAGS% /wd4577
rem class has virtual functions, but its trivial destructor is not virtual
set FLAGS= %FLAGS% /wd5204

rem ///////////////////////////////////////////////////////////////////////////
rem linker flags
rem ///////////////////////////////////////////////////////////////////////////

set LINKER= /link
set LINKER= %LINKER% /release

set LINKER= %LINKER% user32.lib
set LINKER= %LINKER% gdi32.lib
set LINKER= %LINKER% shell32.lib

rem ---------------------------------------------------------------------------
rem link glfw
rem ---------------------------------------------------------------------------

if "%1" == "debug"   (set LINKER= %LINKER% .\glfw.dbg.lib)
if "%1" == "release" (set LINKER= %LINKER% .\glfw.rls.lib)

rem ---------------------------------------------------------------------------
rem link glad
rem ---------------------------------------------------------------------------

if "%1" == "debug"   (set LINKER= %LINKER% .\glad.dbg.lib)
if "%1" == "release" (set LINKER= %LINKER% .\glad.rls.lib)

rem ---------------------------------------------------------------------------
rem link stb_image
rem ---------------------------------------------------------------------------

if "%1" == "debug"   (set LINKER= %LINKER% .\stb_image.dbg.lib)
if "%1" == "release" (set LINKER= %LINKER% .\stb_image.rls.lib)

rem ///////////////////////////////////////////////////////////////////////////
rem compile
rem ///////////////////////////////////////////////////////////////////////////

rem compile glfw
setlocal
call glfw-build.bat %1
endlocal

rem compile glad
setlocal
call glad-build.bat %1
endlocal

rem compile stb_image
setlocal
call stb-image-build.bat %1
endlocal

if not exist .\build mkdir .\build
pushd .\build
cl %FLAGS% %SOURCES% %LINKER%
popd

rem ///////////////////////////////////////////////////////////////////////////
rem exit
rem ///////////////////////////////////////////////////////////////////////////

:exit
exit /b
