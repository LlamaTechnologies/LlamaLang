set(TEST_NAME  ${CMAKE_PROJECT_NAME}_tests)

# set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# set llang sources
set(LLAMATEST_SRC
lexer/lexer_happy.cpp
lexer/lexer_sad.cpp
parser/parser_happy_expr.cpp
parser/parser_sad_expr.cpp
parser/parser_happy_stmnts.cpp
semantic/semantic_happy.cpp
semantic/semantic_sad.cpp
integration/integrations.cpp

test.cpp
)

# Engine executable name
add_executable(${TEST_NAME} ${LLAMATEST_SRC})

# Add code to unit_test
add_test(NAME ${TEST_NAME} COMMAND ${LLAMATEST_SRC})

target_link_libraries(${TEST_NAME} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

# set filters
foreach(_source IN ITEMS ${LLAMATEST_SRC})
# Get the directory of the source file
get_filename_component(_source_path "${_source}" PATH)

# Make sure we are using windows slashes
string(REPLACE "/" "\\" _group_path "${_source_path}")

source_group("${_group_path}" FILES "${_source}")
endforeach()

#####################################
# VISUAL STUDIO COMPILER OPTIONS
#####################################

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DLL_TESTS")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DLL_TESTS")

if (MSVC)
        set_property(TARGET ${EXEC_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Release>>")
        set_property(TARGET ${EXEC_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        set_property(TARGET ${EXEC_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/MyProgram")
endif()
