cmake_minimum_required(VERSION 3.20)
project(tlf VERSION 1.0.0 LANGUAGES CXX)

add_library(tlf INTERFACE
        include/tlf.hpp
        include/tlf/core.hpp
        include/tlf/console.hpp
        include/tlf/math.hpp
        tests/main.cpp)

target_include_directories(tlf INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(tlf INTERFACE cxx_std_17)

include(GNUInstallDirs)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS tlf
        EXPORT tlfTargets
)

install(EXPORT tlfTargets
        FILE tlfTargets.cmake
        NAMESPACE tlf::
        DESTINATION lib/cmake/tlf
)