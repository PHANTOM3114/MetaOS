set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick DBus)

add_executable(MetaHub
        src/main.cpp
        src/cicd_view_model/include/cicd_view_model.hpp
        src/cicd_view_model/src/cicd_view_model.cpp
        qml/qml.qrc
)

target_include_directories(MetaHub PRIVATE
    src/cicd_view_model/include
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