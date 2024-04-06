cmake_minimum_required(VERSION 3.22.0)

project("ProcChecker" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Cmake")

set(PROJECT_VERSION_MAJOR "0")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "0")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "./Src/*.cpp" "./Lib/*.cpp")

include("Qt")

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND LINKED_LIBRARIES rt pthread)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LINKED_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE "./Lib")