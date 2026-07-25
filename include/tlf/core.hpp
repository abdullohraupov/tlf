#pragma once

#if defined(__linux__) || defined (__unix__) || defined(__APPLE__)
    #define UNIX 1
#endif

#if defined(_WIN32) || defined (_WIN64)
    #define WINDOWS 1
#endif

#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <type_traits>
#include <thread>
#include <chrono>
#include <filesystem>
#include <string_view>

#ifdef UNIX
#include <unistd.h>
#include <termios.h>
#endif

namespace tlf {

    template<typename Container>
    void print_container(const Container& container) {

        bool first = true;

        for (const auto& item : container)  {
            if (!first) {
                std::cout << " ";
            }
            std::cout << item;
            first = false;
        }

        std::cout << '\n';
    }

    template<typename Iterator, typename ValueType>
    bool contains(Iterator begin, Iterator end, const ValueType& value) {
        return std::find(begin, end, value) != end;
    }

    class timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> time;

    public:
        timer() : time(std::chrono::high_resolution_clock::now()) {}

        void reset() {
            time = std::chrono::high_resolution_clock::now();
        }

        [[nodiscard]] auto elapsed_ms() const {
            const auto now = std::chrono::high_resolution_clock::now();

            return std::chrono::duration_cast<std::chrono::milliseconds>(now - time).count();
        }

        [[nodiscard]] auto elapsed_s() const {
            const auto now = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double> elapsed = now - time;

            return elapsed.count();
        }
    };

    inline void sleep(std::size_t ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    template<typename DataType = std::string>
    DataType input(std::string_view text) {

        #ifdef UNIX
        tcflush(STDIN_FILENO, TCIFLUSH);
        #elif defined(WINDOWS)
        std::cin.sync();
        #endif

        std::cout << text;

        DataType result;
        if constexpr (std::is_same_v<DataType, std::string>) {
            std::getline(std::cin, result);
        }
        else {
            std::cin >> result;

            while (!std::cin) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "\nInvalid input!\n";
                std::cout << text;
                std::cin >> result;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        return result;
    }

} // namespace tlf