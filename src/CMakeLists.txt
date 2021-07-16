set(EXEC_NAME ${CMAKE_PROJECT_NAME})
set(LIB_NAME ${EXEC_NAME}_lib)

# set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# set llang sources
set(LLAMALANG_SRC
ast_nodes.hpp
ast_nodes.cpp

bigfloat.hpp

bigint.hpp
bigint.cpp

common_defs.hpp
common_defs.cpp

compiler.hpp
compiler.cpp

console.hpp

error.hpp
error.cpp

ir.hpp
ir.cpp

lexer.hpp
lexer.cpp

main.cpp

parse_error_msgs.hpp

parser.hpp
parser.cpp

semantic_analyzer.hpp
semantic_analyzer.cpp

semantic_error_msgs.hpp

types.hpp
types.cpp
)

# Engine executable name
add_executable(${EXEC_NAME} ${LLAMALANG_SRC})

# Add code to unit_test
add_library(${LIB_NAME} STATIC ${LLAMALANG_SRC})

# target name label
set_target_properties(${EXEC_NAME} PROPERTIES PROJECT_LABEL ${EXEC_NAME})

# target name label
set_target_properties(${LIB_NAME} PROPERTIES PROJECT_LABEL ${LIB_NAME})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core binaryformat bitwriter)

# Link against LLVM libraries
target_link_libraries(${EXEC_NAME} ${llvm_libs})

# Link against LLVM libraries
target_link_libraries(${LIB_NAME} ${llvm_libs})

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
        set_property(TARGET ${LIB_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Release>>DLL")
        set_property(TARGET ${LIB_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
        set_property(TARGET ${EXEC_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Release>>DLL")
        set_property(TARGET ${EXEC_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
        set_property(TARGET ${EXEC_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/resources")
        set_property(TARGET ${EXEC_NAME} PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "-s MyProgram_Main.llang")
endif()
