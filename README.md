# TLF

A lightweight utility library.

## Features

- **Console Utilities**: ANSI color definitions, rainbow text output, and typewriter effect (`print_typed`).
- **Smart Input**: Type-safe terminal input validation (`input`) with automatic stream cleanup.
- **Container Helpers**: Clean printing, bound-safe min/max extraction, and element search.
- **Math Helpers**: Modern `<random>` wrappers for integer generation and percentage-based chance checks.
- **Timing**: High-resolution performance timer class.

## Requirements

- **Operating System**: Linux, Windows, MacOS and BSD systems supported.
- **Compiler**: Any C++ compiler supporting at least **C++17**.

## Installation

The library supports standard CMake installation. You can install it system-wide using the following commands:

```bash
# 1. Configure the build directory
cmake -B build

# 2. Install the library to your system
sudo cmake --install build
```

## Integration into your project

After installation, you can easily find and link the library in your own `CMakeLists.txt`:

```cmake
find_package(tlf REQUIRED)

add_executable(my_program main.cpp)
target_link_libraries(my_program PRIVATE tlf::tlf)
```

## Usage Example

```cpp
#include <tlf.hpp>
#include <vector>

int main() {
    // 1. Colorful and typed console output
    tlf::console::print_rainbow("Welcome to TLF Library!");
    tlf::console::print_typed("Loading utilities...", 40);

    // 2. Safe and validated input
    int age = tlf::input<int>("Enter your age: ");

    // 3. Container utilities
    std::vector<int> scores = {10, 42, 5, 99, 21};
    tlf::print_container(scores);

    if (tlf::contains(scores.begin(), scores.end(), 10)) {
        std::cout << "scores contains 10!";
    }

    // 4. Randomization helpers
    if (tlf::math::chance(75.5)) {
        int lucky_num = tlf::math::random(1, 100);
        std::cout << "Lucky number: " << lucky_num << '\n';
    }

    return 0;
}
```

## License

This project is licensed under the **GNU General Public License v3.0 (GPLv3)** — see the [LICENSE](LICENSE) file for details.
