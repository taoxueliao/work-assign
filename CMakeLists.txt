cmake_minimum_required(VERSION 3.16)

project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Qml Quick)

add_executable(app
    src/main.cpp
    src/repository/JsonWordSource.cpp
    src/service/RandomAssigner.cpp
    src/viewmodel/AssignmentListModel.cpp
    src/viewmodel/AssignController.cpp
    resources/app.qrc
)

target_include_directories(app PRIVATE src)

target_link_libraries(app PRIVATE
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
)

add_subdirectory(tests)
