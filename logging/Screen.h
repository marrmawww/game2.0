#pragma once
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

inline void enableAnsi() {
#ifdef _WIN32
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(console, &mode);
    SetConsoleMode(console, mode | 0x0004);   
#endif
}

inline void clearScreen() {
    std::cout << "\033[2J\033[H";
}


inline void moveCursor(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H";
}

inline void saveCursor() {
    std::cout << "\033" << "7";
}

inline void restoreCursor() {
    std::cout << "\033" << "8";
}
