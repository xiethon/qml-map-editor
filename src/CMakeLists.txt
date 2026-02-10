qt_add_qml_module(${CMAKE_PROJECT_NAME}
    URI map.editor
    VERSION 1.0
    QML_FILES
        App.qml
        EditorMap.qml
        MapEditorToolBar.qml
        MapEditorActionBar.qml
        MapEditorActionButton.qml
        MapEditorLayer.qml
    RESOURCES
        assets/clean.svg
        assets/closed.svg
        assets/lock.svg
        assets/unlock.svg
        assets/delete.svg
        assets/point.svg
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

add_subdirectory(components)