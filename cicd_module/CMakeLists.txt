find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(sdbus-c++ REQUIRED)
find_package(sdbus-c++-tools REQUIRED)

FetchContent_Declare(httplib SYSTEM
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
        GIT_TAG a609330e4c6374f741d3b369269f7848255e1954
        GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(httplib)

FetchContent_Declare(libenvpp
        GIT_REPOSITORY https://github.com/ph3at/libenvpp.git
        GIT_TAG v1.5.1
)
FetchContent_MakeAvailable(libenvpp)

set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

set(DBUS_INTERFACE_XML "${CMAKE_SOURCE_DIR}/protocols/dbus/cicd/cicdDataFetch.xml")
set(DBUS_ADAPTOR_H "${CMAKE_CURRENT_BINARY_DIR}/generated/CiCdAdaptor.h")
set(DBUS_PROXY_H "${CMAKE_CURRENT_BINARY_DIR}/generated/CiCdProxy.h")

file(MAKE_DIRECTORY ${GENERATED_DIR})

add_custom_command(
        OUTPUT ${DBUS_ADAPTOR_H} ${DBUS_PROXY_H}
        COMMAND SDBusCpp::sdbus-c++-xml2cpp ${DBUS_INTERFACE_XML} --adaptor=${DBUS_ADAPTOR_H} --proxy=${DBUS_PROXY_H}
        DEPENDS ${DBUS_INTERFACE_XML}
        COMMENT "Generating D-Bus C++ interfaces from ${DBUS_INTERFACE_XML}"
)

add_executable(cicd_module main.cpp
        src/adapters/cicd_dbus_adapter.cpp
        src/providers/github_provider.cpp
)
add_custom_target(generate_dbus_interfaces DEPENDS ${DBUS_ADAPTOR_H} ${DBUS_PROXY_H})

add_dependencies(cicd_module generate_dbus_interfaces)

target_include_directories(cicd_module PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/generated
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${libenvpp_SOURCE_DIR}/include
)

target_link_libraries(cicd_module PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        ZLIB::ZLIB
        httplib
        nlohmann_json::nlohmann_json
        SDBusCpp::sdbus-c++
        libenvpp::libenvpp
        ${SDBUSCPP_GENERATED_INTERFACES_LIB}
)

target_compile_definitions(cicd_module PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)

install(TARGETS cicd_module DESTINATION bin)
install(
        FILES "${dbus-api_SOURCE_DIR}/CiCD-Module/cicdDataFetch.xml"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/dbus-1/interfaces
        RENAME org.metaos.CiCd.Interface.xml
)