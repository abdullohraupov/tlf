/*
 * TLF - A lightweight utility library.
 * Copyright (C) 2026 Abdulloh Raupov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://gnu.org>.
 */

#pragma once

#ifdef __linux__ // LINUX

#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <limits>
#include <type_traits>
#include <optional>
#include <thread>
#include <chrono>
#include <filesystem>
#include <random>
#include <unistd.h>
#include <termios.h>
#include <string_view>
#include <cstdint>

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

    template<typename Iterator>
    auto get_max(Iterator begin, Iterator end) -> std::optional<typename std::iterator_traits<Iterator>::value_type> {

        if (begin == end) return std::nullopt;

        return *std::max_element(begin, end);
    }

    template<typename Iterator>
    auto get_min(Iterator begin, Iterator end) -> std::optional<typename std::iterator_traits<Iterator>::value_type> {

        if (begin == end) return std::nullopt;

        return *std::min_element(begin, end);
    }

    class timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> time;

    public:
        timer() : time(std::chrono::high_resolution_clock::now()) {}

        void reset() {
            time = std::chrono::high_resolution_clock::now();
        }

        auto elapsed_ms() const {
            const auto now = std::chrono::high_resolution_clock::now();

            return std::chrono::duration_cast<std::chrono::milliseconds>(now - time).count();
        }

        auto elapsed_s() const {
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = now - time;

            return elapsed.count();
        }
    };

    inline void sleep(size_t ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));

        tcflush(STDIN_FILENO, TCIFLUSH);
    }

    template<typename DataType = std::string>
    DataType input(const std::string &text) {

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

    namespace console {

        inline void clear() {
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

        inline void print_rainbow(std::string_view text) {

            static constexpr std::array<std::string_view, 6> rainbow_colors {
                tlf::console::red,
                tlf::console::green,
                tlf::console::yellow,
                tlf::console::cyan,
                tlf::console::blue,
                tlf::console::magenta
            };

            for (size_t i {0}; i < text.size(); ++i) {
                char c {text[i]};
                std::cout << rainbow_colors[i % 6] << c;
            }
            std::cout << tlf::console::reset << '\n';
        }

        inline void print_typed(std::string_view text, int delay_ms = 50) {
            for (size_t i {0}; i < text.size(); ++i) {
                std::cout << text[i] << std::flush;
                tlf::sleep(delay_ms);
            }
            std::cout << '\n';
        }

    } // namespace console

    namespace math {

        template<typename DataType = int>
        DataType random(DataType min, DataType max) {

            static_assert(std::is_integral_v<DataType>, "type must be integral");

            static std::random_device rd;
            static std::mt19937 gen(rd());

            std::uniform_int_distribution<DataType> distr(min, max);

            return distr(gen);
        }

        inline bool chance(double chance) {

            static std::random_device rd;
            static std::mt19937 gen(rd());

            std::uniform_real_distribution<double> distr(0.0, 100.0);

            return distr(gen) <= chance;
        }

    } // namespace math

} // namespace tlf

#endif // LINUX
