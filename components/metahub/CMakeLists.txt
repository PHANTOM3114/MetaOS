cmake_minimum_required(VERSION 3.20)
project(MetaHub CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick DBus)

add_executable(MetaHub
        src/main.cpp
        qml/qml.qrc
)

target_link_libraries(MetaHub PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::DBus
)

install(TARGETS MetaHub
    DESTINATION bin
)