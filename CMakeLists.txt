cmake_minimum_required(VERSION 3.15)

project(
    ptl
    VERSION 0.1
    DESCRIPTION "Persistent Template Library for C++"
    LANGUAGES CXX
)

file(GLOB SOURCE_FILES
    "src/*.cxx"
    "src/*.hxx"
)

option(BUILD_STATIC "Build static version of ${PROJECT_NAME}" ON)
option(BUILD_SHARED "Build static version of ${PROJECT_NAME}" ON)
option(BUILD_TESTS "Build tests" ON)

set(INCLUDE_PLT "include/plt")

if(BUILD_TESTS)
    enable_testing()
    file(GLOB TEST_SOURCES
        "src/test/*.cxx"
    )
endif()

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(NOT STATIC_PTL)
    set(STATIC_PTL "${PROJECT_NAME}-static")
endif()
if(NOT SHARED_PTL)
    set(SHARED_PTL "${PROJECT_NAME}-shared")
endif()

set(TARGETS "")
if(BUILD_STATIC)
    list(APPEND TARGETS ${STATIC_PTL})
endif()
if(BUILD_SHARED)
    list(APPEND TARGETS ${SHARED_PTL})
endif()

set(TARGETS "${STATIC_PTL};${SHARED_PTL}")

add_library(${STATIC_PTL} STATIC ${SOURCE_FILES})
add_library(${SHARED_PTL} SHARED ${SOURCE_FILES})

foreach(TARGET IN LISTS TARGETS)
    set_target_properties(
        ${TARGET} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
    target_include_directories(
        ${TARGET} PUBLIC
        ${INCLUDE_PLT}
    )
endforeach()

foreach(TEST_SOURCE IN LISTS TEST_SOURCES)
    get_filename_component(TEST_TARGET ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_TARGET} ${TEST_SOURCE})
    set_target_properties(
        ${TEST_TARGET} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
    target_link_libraries(${TEST_TARGET} PRIVATE ${SHARED_PTL})
    target_include_directories(${TEST_TARGET} PRIVATE ${INCLUDE_PLT})
    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
endforeach()

