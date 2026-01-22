cmake_minimum_required(VERSION 3.25)

project(AR-S VERSION 0.0.3 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)           
set(CMAKE_CXX_STANDARD_REQUIRED ON)  
set(CMAKE_CXX_EXTENSIONS OFF) 

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        9.1.0
)

FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG 56a1ed11ad7841255048659024363cd0fade31f4
)
FetchContent_MakeAvailable(Corrosion)

add_subdirectory(protocols)       
add_subdirectory(modules)
