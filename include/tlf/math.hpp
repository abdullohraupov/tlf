#pragma once

#if defined(__linux__) || defined (__unix__) || defined(__APPLE__)
    #define UNIX 1
#endif

#if defined(_WIN32) || defined (_WIN64)
    #define WINDOWS 1
#endif

#include <random>

namespace tlf::math {

    template<typename DataType = int>
    DataType random(DataType min, DataType max) {

        static_assert(std::is_integral_v<DataType>, "type must be integral");

        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_int_distribution<DataType> distribution(min, max);

        return distribution(gen);
    }

    inline bool chance(const double chance) {

        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_real_distribution<double> distribution(0.0, 100.0);

        return distribution(gen) <= chance;
    }

} // namespace math