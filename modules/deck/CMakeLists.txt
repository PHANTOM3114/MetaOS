set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick DBus NetworkAuth)

add_executable(deck
        main.cpp
        sonar/sonar_view_model.hpp
        sonar/sonar_view_model.cpp
        sonar/auth_manager.hpp
        sonar/auth_manager.cpp
        qml/qml.qrc
)

target_link_libraries(deck PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::DBus
    Qt6::NetworkAuth
)

install(TARGETS deck
    DESTINATION bin
)