@echo off
REM Compile the C file using MinGW
gcc giveup.c -o giveup.exe

REM Run the compiled C executable
giveup.exe

REM Run the Python script
python gui.py
