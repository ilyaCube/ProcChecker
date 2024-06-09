project("Tests")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS *)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})