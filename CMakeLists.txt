cmake_minimum_required(VERSION 3.14)
project(rymlWrapper)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Set vars
set(c4core_DIR /dev/vcpkg/packages/c4core_x64-windows/share/c4core)
set(ryml_DIR /dev/vcpkg/packages/ryml_x64-windows/share/ryml)
set(Boost_INCLUDE_DIR c:/Boost/boost_1_81_0)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_WARN_NEW_VERSIONS 1)

# Find dependencies
find_package(ryml CONFIG REQUIRED)
find_package(Boost COMPONENTS filesystem)

# Find yml library
add_library(ymlLib SHARED IMPORTED)
set_property(TARGET ymlLib PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/libs/yml/bin/Release/yml.lib)
target_link_libraries(ymlLib INTERFACE ryml::ryml)
target_link_libraries(ymlLib INTERFACE ${Boost_FILESYSTEM_LIBRARY})

add_executable(ymlTest test/main.cpp)
target_link_libraries(ymlTest PRIVATE ymlLib GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
target_include_directories(ymlTest PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/yml/include>
  $<INSTALL_INTERFACE:include>)

# Enable CMake’s test runner to discover the tests included in the binary, using the GoogleTest CMake module.
include(GoogleTest)
gtest_discover_tests(ymlTest)

  