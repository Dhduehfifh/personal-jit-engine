@echo off
set SRC=main.c
set OUT=bin\jit_engine.exe

echo [🔨] Checking for compiler...
where cl >nul 2>nul
if %errorlevel%==0 (
    set CC=cl
    echo [🎯] Using MSVC: %CC%
    if not exist bin mkdir bin
    %CC% /nologo /O2 %SRC% /Fe:bin\jit_engine.exe
    goto done
)

where gcc >nul 2>nul
if %errorlevel%==0 (
    set CC=gcc
    echo [🎯] Using MinGW GCC: %CC%
    if not exist bin mkdir bin
    %CC% -std=c11 -O2 %SRC% -o bin\jit_engine.exe
    goto done
)

echo [❌] No suitable compiler found (MSVC or MinGW)
exit /b 1

:done
echo [✅] Build complete -> %OUT%