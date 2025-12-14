@echo off
REM Batch file to compile Minesweeper game
REM This script compiles all source files into minesweeper.exe

cd /d "%~dp0"
echo ========================================
echo    Minesweeper Game Compiler
echo ========================================
echo.

REM Check if main.c exists
if not exist "main.c" (
    echo ERROR: main.c not found in current directory!
    echo Please make sure you're running this from the project directory.
    echo Current directory: %CD%
    pause
    exit /b 1
)

REM Check for GCC
where gcc >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: GCC compiler not found!
    echo.
    echo Please install MinGW-w64 or add GCC to your system PATH.
    echo Download MinGW from: https://www.mingw-w64.org/downloads/
    echo.
    pause
    exit /b 1
)

echo Compiling Minesweeper...
echo Current directory: %CD%
echo.

REM Compile all source files
gcc main.c common.c display.c input.c menu.c game.c settings.c store.c statistics.c achievements.c abilities.c theme.c -o minesweeper.exe -Wall -Wextra -std=c99 -O2

REM Check compilation result
if %errorlevel% == 0 (
    echo.
    echo ========================================
    echo    Compilation successful!
    echo ========================================
    echo.
    echo The game has been compiled successfully.
    echo.
    echo To run the game, type:
    echo   minesweeper.exe
    echo.
    echo Or simply double-click minesweeper.exe
    echo.
) else (
    echo.
    echo ========================================
    echo    Compilation failed!
    echo ========================================
    echo.
    echo Please check the error messages above.
    echo Make sure:
    echo   - All source files (.c and .h) are present
    echo   - GCC is properly installed and in your PATH
    echo   - You have write permissions in this directory
    echo.
)

pause
