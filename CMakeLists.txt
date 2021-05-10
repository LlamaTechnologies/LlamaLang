cmake_minimum_required(VERSION 3.1)

# set c++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# MSVC runtime library flags are selected by an abstraction.
cmake_policy(SET CMP0091 NEW)

# find llvm
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

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

######################################
# DIRECTORIES AND NAMES
######################################

set(EXEC_NAME "llang")

# set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

# set llang sources
set(LLAMALANG_SRC
src/bigint.hpp
src/bigint.cpp

src/common_defs.h
src/common_defs.cpp

src/config.hpp
src/console.hpp


src/error.hpp
src/error.cpp

src/main.cpp

src/tokenize.hpp
src/tokenize.cpp

src/supportedcompilers.hpp
)

link_directories("${CMAKE_SOURCE_DIR}/lib" "${LLVM_DIR}/lib")

# Engine executable name
add_executable(${EXEC_NAME} ${LLAMALANG_SRC})

# target name label
set_target_properties(${EXEC_NAME} PROPERTIES PROJECT_LABEL ${EXEC_NAME})

# set include dirs
target_include_directories(${EXEC_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include/antlr4-runtime")

# Link to antlr4
target_link_libraries(${EXEC_NAME} optimized antlr4-runtime debug antlr4-runtime-d)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core binaryformat bitwriter)

# Link against LLVM libraries
target_link_libraries(${EXEC_NAME} ${llvm_libs})

# set filters
foreach(_source IN ITEMS ${LLAMALANG_SRC})
# Get the directory of the source file
get_filename_component(_source_path "${_source}" PATH)

# Make sure we are using windows slashes
string(REPLACE "/" "\\" _group_path "${_source_path}")

source_group("${_group_path}" FILES "${_source}")
endforeach()

#####################################
# SET BUILD TARGETS
#####################################
set_property(TARGET ${EXEC_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ANTLR4CPP_STATIC)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")


#####################################
# VISUAL STUDIO COMPILER OPTIONS
#####################################

if (MSVC)
        set_property(TARGET ${EXEC_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Release>>")
        set_property(TARGET ${EXEC_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        set_property(TARGET ${EXEC_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/MyProgram")
endif()
