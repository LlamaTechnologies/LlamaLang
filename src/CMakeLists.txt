# find llvm
#find_package(LLVM REQUIRED CONFIG)

#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

#include_directories(${LLVM_INCLUDE_DIRS})
#add_definitions(${LLVM_DEFINITIONS})

set(EXEC_NAME ${CMAKE_PROJECT_NAME})

# set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/engine")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/engine")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/engine")

# set llang sources
set(LLAMALANG_SRC
ast_nodes.hpp
ast_nodes.cpp

bigint.hpp
bigint.cpp

common_defs.hpp
common_defs.cpp

config.hpp
console.hpp

error.hpp
error.cpp

main.cpp

parse_error_msgs.hpp

parser.hpp
parser.cpp

supportedcompilers.hpp

lexer.hpp
lexer.cpp
)

#link_directories("${CMAKE_SOURCE_DIR}/extern" "${LLVM_DIR}/lib")

# Engine executable name
add_executable(${EXEC_NAME} ${LLAMALANG_SRC})

# Add code to unit_test
add_library(${EXEC_NAME}_lib STATIC ${LLAMALANG_SRC})

# target name label
set_target_properties(${EXEC_NAME} PROPERTIES PROJECT_LABEL ${EXEC_NAME})

# Find the libraries that correspond to the LLVM components
# that we wish to use
#llvm_map_components_to_libnames(llvm_libs support core binaryformat bitwriter)

# Link against LLVM libraries
#target_link_libraries(${EXEC_NAME} ${llvm_libs})

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
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")


#####################################
# VISUAL STUDIO COMPILER OPTIONS
#####################################

if (MSVC)
        set_property(TARGET ${EXEC_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Release>>")
        set_property(TARGET ${EXEC_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        set_property(TARGET ${EXEC_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/MyProgram")
endif()
