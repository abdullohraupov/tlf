#pragma once

#include <tlf/macro.hpp>

#include <random>

namespace tlf::math {
    template<typename DataType = int>
    DataType random(DataType min, DataType max) {
        static_assert(std::is_integral_v<DataType>, "type must be integral");

        thread_local std::random_device rd;
        thread_local std::mt19937 gen(rd());

        using dist_type = std::conditional_t<
            sizeof(DataType) < sizeof(int),
            int,
            DataType
        >;

        std::uniform_int_distribution<dist_type> distribution(
            static_cast<dist_type>(min),
            static_cast<dist_type>(max)
        );

        return static_cast<DataType>(distribution(gen));
    }

    inline bool chance(const double chance) {
        thread_local std::random_device rd;
        thread_local std::mt19937 gen(rd());

        std::uniform_real_distribution distribution(0.0, 100.0);

        return distribution(gen) <= chance;
    }
} // namespace math
