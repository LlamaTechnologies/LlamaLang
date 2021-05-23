######################################
# DIRECTORIES AND NAMES
######################################

set(TEST_NAME  ${CMAKE_PROJECT_NAME}_tests)

# set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/test")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/test")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/test")

# set llang sources
set(LLAMATEST_SRC
lexer/lexer_happy_paths.cpp
"test.cpp"
)

link_directories("${CMAKE_SOURCE_DIR}/lib" "${LLVM_DIR}/lib")

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

if (MSVC)
        set_property(TARGET ${EXEC_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Release>>")
        set_property(TARGET ${EXEC_NAME} PROPERTY MSVC_RUNTIME_LIBRARY  "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        set_property(TARGET ${EXEC_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/MyProgram")
endif()