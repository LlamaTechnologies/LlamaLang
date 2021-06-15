cmake_minimum_required(VERSION 3.1)

# set c++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# MSVC runtime library flags are selected by an abstraction.
cmake_policy(SET CMP0091 NEW)

# set configuration types and make them advanced option on cmake.
mark_as_advanced(CMAKE_INSTALL_PREFIX)
set(CMAKE_CONFIGURATION_TYPES Release Debug)
set(CMAKE_CXX_FLAGS_RELEASE_ASSERTS ${CMAKE_CXX_FLAGS_RELEASE})
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE_ASSERTS ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})


# set the project/solution name
project("LlamaLang"
        VERSION 1.0
        DESCRIPTION "LlamaLang compiler"
        LANGUAGES CXX)
# use folders for ZERO_CHECK and BUILD_ALL
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# remove ...
remove_definitions(/CMAKE_INTDIR)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_subdirectory(extern/googletest)
add_subdirectory(src)
add_subdirectory(tests)
