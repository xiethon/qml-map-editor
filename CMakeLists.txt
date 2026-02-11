cmake_minimum_required(VERSION 3.22)

project(
    qml-map-editor
    VERSION 1.0.0
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/xiethon/qml-map-editor"
    DESCRIPTION "A QML Map Editor for Qt Quick & QtLocation, supporting point,linestring,polygon,circle and square"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)
include(CPM)
CPMAddPackage("gh:xiethon/tianditu-plugin@1.1.3")

find_package(Qt6 REQUIRED COMPONENTS Quick Qml Network Location Positioning)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(${CMAKE_PROJECT_NAME}
    src/main.cc
)

qt_add_qml_module(${CMAKE_PROJECT_NAME}
    URI map.editor
    VERSION 1.0
    QML_FILES
        src/App.qml
        src/EditorMap.qml
        src/MapEditorToolBar.qml
        src/MapEditorActionBar.qml
        src/MapEditorActionButton.qml
        src/MapEditorLayer.qml
    RESOURCES
        assets/clean.svg
        assets/closed.svg
        assets/lock.svg
        assets/unlock.svg
        assets/delete.svg
        assets/marker.svg
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE Qt6::Quick 
    PRIVATE Qt6::Qml
    PRIVATE Qt6::Network
    PRIVATE Qt6::Location
    PRIVATE Qt6::Positioning

    PRIVATE tianditu-plugin
    PRIVATE map-editor-components
    PRIVATE map-editor-componentsplugin
)

add_subdirectory(src/components)