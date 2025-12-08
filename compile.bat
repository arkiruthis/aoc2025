@echo off
setlocal

if "%~1"=="" (
    echo Usage: %~nx0 ^<source_file.cpp^>
    exit /b 1
)

set SOURCE_FILE=%~1

if not exist "%SOURCE_FILE%" (
    echo Error: File '%SOURCE_FILE%' not found
    exit /b 1
)

rem Call vcvars64.bat to set up MSVC environment
rem Adjust the path below to match your Visual Studio installation
rem Common locations:
rem   Visual Studio 2022: "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
rem   Visual Studio 2019: "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
) else (
    echo Error: Could not find vcvars64.bat. Please adjust the path in this script.
    exit /b 1
)

rem Compile with cl using C++20 standard and optimizations
cl /std:c++20 /O2 /EHsc /W4 "%SOURCE_FILE%" /Fe:main.exe

if %ERRORLEVEL% equ 0 (
    echo Successfully compiled %SOURCE_FILE% to main.exe
    rem Clean up intermediate files
    del *.obj >nul 2>&1
) else (
    echo Compilation failed
    exit /b 1
)

endlocal
