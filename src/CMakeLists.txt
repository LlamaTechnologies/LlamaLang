set(EXEC_NAME ${CMAKE_PROJECT_NAME})
set(LIB_NAME ${EXEC_NAME}_lib)

# set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# set llang sources
set(LLAMALANG_SRC
common_defs.hpp
common_defs.cpp

compiler.hpp
compiler.cpp

driver.hpp
driver.cpp

error.hpp
error.cpp

main.cpp

commons/defer_stmnt.hpp

utils/console.hpp
utils/file_utils.hpp
utils/file_utils.cpp

ast/ast_fwd.hpp
ast/ast_nodes.hpp
ast/ast_nodes.cpp

tokenizer/token.hpp
tokenizer/tokenizer.hpp
tokenizer/tokenizer_fwd.hpp
tokenizer/tokenizer.cpp

parser/parse_error_msgs.hpp
parser/parser.hpp
parser/parser.cpp

code_gen/ir.hpp
code_gen/ir.cpp

semantics/builtins.hpp
semantics/builtins.cpp
semantics/scope.hpp
semantics/scope.cpp
semantics/semantic_analyzer.hpp
semantics/semantic_analyzer.cpp
semantics/semantic_error_msgs.hpp
semantics/src_code_repository.hpp
semantics/src_code_repository.cpp
semantics/types.hpp
semantics/types.cpp

platform.hpp
platform/linux.cpp
platform/windows.cpp
)

# Engine executable name
add_executable(${EXEC_NAME} ${LLAMALANG_SRC})

# Add code to unit_test
add_library(${LIB_NAME} STATIC ${LLAMALANG_SRC})

# target name label
set_target_properties(${EXEC_NAME} PROPERTIES PROJECT_LABEL ${EXEC_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "llama")

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
        set_property(TARGET ${EXEC_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Debug")
        set_property(TARGET ${EXEC_NAME} PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "-s ../examples/TestSimple.llama")
endif()
