#pragma once

#include <tlf/macro.hpp>

#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <type_traits>
#include <thread>
#include <chrono>
#include <filesystem>
#include <string_view>
#include <ctime>
#include <iomanip>
#include <fstream>

#ifdef TLF_UNIX
#include <unistd.h>
#include <termios.h>
#elif defined(TLF_WINDOWS)
#include <windows.h>
#endif

namespace tlf {
    template<typename Container>
    void print_container(const Container &container) {
        bool first = true;

        std::cout << '[';
        for (const auto &item: container) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << item;
            first = false;
        }

        std::cout << "]\n";
    }

    template<typename Iterator, typename ValueType>
    bool contains(Iterator begin, Iterator end, const ValueType &value) {
        return std::find(begin, end, value) != end;
    }

    class timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> time;

    public:
        timer() : time(std::chrono::high_resolution_clock::now()) {
        }

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

    class logger {
    private:
        std::ofstream log_file;

        [[nodiscard]] static std::string get_current_time() {
            const auto now = std::chrono::system_clock::now();
            std::time_t time_now = std::chrono::system_clock::to_time_t(now);
            std::tm time_info {};

#           ifdef TLF_WINDOWS
                localtime_s(&time_info, &time_now);
#           else
                localtime_r(&time_now, &time_info); // Linux, Mac and BSD systems
#           endif

            std::stringstream ss;
            ss << std::put_time(&time_info, "[%Y-%m-%d %H:%M:%S]");
            return ss.str();
        }

        void write_log(std::string_view level, std::string_view message) {
            if (log_file.is_open()) {
                log_file << get_current_time() << " " << level << ": " << message << '\n';
                log_file.flush();
            }
        }

    public:
        explicit logger(std::string_view filename = "app.log") {
            log_file.open(std::string(filename), std::ios::app);
        }

        ~logger() {
            if (log_file.is_open()) log_file.close();
        }

        void info(std::string_view message) { write_log("[INFO]", message); }
        void warning(std::string_view message) { write_log("[WARNING]", message); }
        void error(std::string_view message) { write_log("[ERROR]", message); }
    };

    inline void sleep(const std::size_t ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    template<typename DataType = std::string>
    DataType input(std::string_view text) {
#       ifdef TLF_UNIX
            tcflush(STDIN_FILENO, TCIFLUSH);
#       elif defined(TLF_WINDOWS)
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#       endif

        std::cout << text;

        DataType result;
        if constexpr (std::is_same_v<DataType, std::string>) {
            std::getline(std::cin, result);
        } else {
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
