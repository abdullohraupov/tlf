#pragma once

#if defined(__linux__) || defined (__unix__) || defined(__APPLE__)
#   define TLF_UNIX 1
#endif

#if defined(_WIN32) || defined (_WIN64)
#   define TLF_WINDOWS 1
#endif
