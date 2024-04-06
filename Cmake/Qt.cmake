set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Quick Core Widgets Core5Compat REQUIRED)
list(APPEND LINKED_LIBRARIES Qt6::Quick Qt6::Core Qt6::Widgets Qt6::Core5Compat)