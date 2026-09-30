# 只测读词和抽样，不启动界面。

find_package(Qt5 5.15.2 REQUIRED COMPONENTS Core Test)

add_executable(JsonWordSourceTest
    JsonWordSourceTest.cpp
    ../src/repository/JsonWordSource.cpp
)

target_include_directories(JsonWordSourceTest PRIVATE ../src)
target_link_libraries(JsonWordSourceTest PRIVATE Qt5::Core Qt5::Test)

add_executable(RandomAssignerTest
    RandomAssignerTest.cpp
    ../src/service/RandomAssigner.cpp
)

target_include_directories(RandomAssignerTest PRIVATE ../src)
target_link_libraries(RandomAssignerTest PRIVATE Qt5::Core Qt5::Test)

find_package(Qt5 5.15.2 REQUIRED COMPONENTS Gui Qml Quick)

add_executable(PublishFlowTest
    PublishFlowTest.cpp
    ../src/repository/JsonWordSource.cpp
    ../src/service/RandomAssigner.cpp
    ../src/viewmodel/AssignmentListModel.cpp
    ../src/viewmodel/AssignController.cpp
    ../resources/app.qrc
)

target_include_directories(PublishFlowTest PRIVATE ../src)
target_link_libraries(PublishFlowTest PRIVATE Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Test)
