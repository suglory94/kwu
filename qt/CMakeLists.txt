cmake_minimum_required(VERSION 3.16)

project(QtAssignment VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    gameboardwidget.cpp gameboardwidget.h gameboardwidget.ui
    gamesettingdialog.cpp gamesettingdialog.h gamesettingdialog.ui
    mainwindow.cpp mainwindow.h mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QtAssignment
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        Move.h
        replaywidget.h replaywidget.cpp replaywidget.ui


    )
    qt_finalize_executable(QtAssignment)
else()
    add_executable(QtAssignment ${PROJECT_SOURCES})
endif()

target_link_libraries(QtAssignment PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(QtAssignment PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
