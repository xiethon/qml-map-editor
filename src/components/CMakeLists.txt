set(qml_files
    MapMarkerItem.qml
    MapPointItem.qml
    MapLinestringItem.qml
    MapPolygonItem.qml
    MapSegmentItem.qml
)
 
set(sources
    mapgeometry.h
    mapgeometry.cc
    mappoint.h
    mappoint.cc
    maplinestring.h
    maplinestring.cc
    mappolygon.h
    mappolygon.cc
    mapeditorcontroller.h
    mapeditorcontroller.cc
)

qt_add_library(map-editor-components STATIC)

qt_add_qml_module(map-editor-components
    URI map.editor.components
    VERSION 1.0
    QML_FILES   ${qml_files}
    SOURCES     ${sources}    
)

target_link_libraries(map-editor-components
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Positioning
    PRIVATE Qt6::Location
    PRIVATE Qt6::Qml
)

target_include_directories(map-editor-components PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})