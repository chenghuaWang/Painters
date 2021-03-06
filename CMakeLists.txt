cmake_minimum_required(VERSION 3.5)

project(Painters VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

add_compile_definitions("VE_ROOT_DIR=${VE_ROOT_DIR}")

file(GLOB_RECURSE framelesswindow_include ./3rdparty/framelesswindow/*.h)
file(GLOB_RECURSE framelesswindow_src ./3rdparty/framelesswindow/*.cpp)

set(TS_FILES Painters_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
        theme/style.qrc
        icon_bind.qrc
        about_window.h about_window.cpp about_window.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Painters
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Painters APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Painters SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Painters
            ${PROJECT_SOURCES}

        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# set basic include path
set(p_core_include_dir ${CMAKE_SOURCE_DIR}/Core)
set(p_mWidget_include_dir ${CMAKE_SOURCE_DIR}/mWidget/include)
set(p_editor_include_dir ${CMAKE_SOURCE_DIR}/Editor/include)
set(3rd_spdlog ${CMAKE_SOURCE_DIR}/spdlog/include/)

include_directories(${p_core_include_dir})
include_directories(${p_core_include_dir}/include)
include_directories(${p_mWidget_include_dir})
include_directories(${p_editor_include_dir})

# self-defined add core lib
add_subdirectory(3rdparty)
add_subdirectory(Core)
add_subdirectory(mWidget)
add_subdirectory(Editor)
target_link_libraries(Painters PRIVATE Qt${QT_VERSION_MAJOR}::Widgets p_core p_m_widget p_editor)
target_link_libraries(Painters PRIVATE Qt6::OpenGLWidgets spdlog)

set_target_properties(Painters PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    AUTORCC ON
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Painters)
endif()
