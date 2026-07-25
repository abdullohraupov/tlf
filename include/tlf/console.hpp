#pragma once

#if defined(__linux__) || defined (__unix__) || defined(__APPLE__)
    #define UNIX 1
#endif

#if defined(_WIN32) || defined (_WIN64)
    #define WINDOWS 1
#endif

#include <iostream>
#include <string_view>

#ifdef WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif


inline void init() {
    #ifdef WINDOWS
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
    }
    #endif
}

namespace tlf::console {

    inline void clear() {
        init();
        std::cout << "\033[2J\033[1;1H";
    }

    // *---colors---* //
    inline constexpr std::string_view red {"\033[31m"};
    inline constexpr std::string_view green {"\033[32m"};
    inline constexpr std::string_view yellow {"\033[33m"};
    inline constexpr std::string_view cyan {"\033[36m"};
    inline constexpr std::string_view blue {"\033[34m"};
    inline constexpr std::string_view magenta {"\033[35m"};
    inline constexpr std::string_view reset {"\033[0m"};
    // *---colors---* //

} // namespace console