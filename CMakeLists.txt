cmake_minimum_required(VERSION 3.0)
project(CellularAutomata)

set (CMAKE_CXX_STANDARD 17)
find_package(SFML 2.5.1 COMPONENTS audio graphics window system REQUIRED)

set(SOURCES
    main.cpp
    App.cpp
    )

add_executable(CellularAutomata ${SOURCES})

target_link_libraries(CellularAutomata sfml-audio sfml-graphics sfml-window sfml-system)
