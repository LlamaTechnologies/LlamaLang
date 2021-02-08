cmake_minimum_required(VERSION 3.1)

# set c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
src/antlr/LlamaLangBaseVisitor.cpp
src/antlr/LlamaLangBaseVisitor.h
src/antlr/LlamaLangLexer.cpp
src/antlr/LlamaLangLexer.h
src/antlr/LlamaLangParser.cpp
src/antlr/LlamaLangParser.h
src/antlr/LlamaLangVisitor.cpp
src/antlr/LlamaLangVisitor.h

src/ast/AST_Types.hpp
src/ast/BinaryStatementNode.hpp
src/ast/ConstantNode.hpp
src/ast/FunctionNode.hpp
src/ast/Node.hpp
src/ast/ProgramNode.hpp
src/ast/RightValueNode.hpp
src/ast/StatementNode.hpp
src/ast/UnaryStatementNode.hpp
src/ast/VariableDeclNode.hpp

src/built-in/Built-inTypes.hpp

src/error_handling/ErrorDescription.hpp
src/error_handling/SyntaxErrorListener.hpp

src/semantic_analyzer/SemanticAnalyzer.cpp
src/semantic_analyzer/SemanticAnalyzer.hpp

src/symbol_table/ComplexTypeSymbol.hpp
src/symbol_table/FunctionSymbol.hpp
src/symbol_table/Symbol.hpp
src/symbol_table/TypeSymbol.hpp
src/symbol_table/VarSymbol.hpp
src/symbol_table/SymbolTableScope.hpp

src/Compiler.cpp
src/Primitives.cpp
src/Primitives.hpp
src/CppReflection.hpp
src/SupportedCompilers.hpp
)

link_directories("${CMAKE_SOURCE_DIR}/lib")

# Engine executable name
add_executable(${EXEC_NAME} ${LLAMALANG_SRC})

# target name label
set_target_properties(${EXEC_NAME} PROPERTIES PROJECT_LABEL ${EXEC_NAME})

# set include dirs
target_include_directories(${EXEC_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include/antlr4-runtime")

# Link to antlr4
target_link_libraries(${EXEC_NAME} antlr4-runtime)

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

# DEBUG
set_property(TARGET ${EXEC_NAME} APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:LT_DEBUG>)
#RELEASE
set_property(TARGET ${EXEC_NAME} APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>:LT_RELEASE>)
