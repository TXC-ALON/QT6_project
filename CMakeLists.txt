cmake_minimum_required(VERSION 3.11)

project(MyProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

MESSAGE("Identifying the OS...")
if(WIN32)
  MESSAGE("This is Windows.")
elseif(APPLE)
  MESSAGE("This is MacOS.")
elseif(UNIX)
  MESSAGE("This is Linux.")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin") # 可执行文件输出目录
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include") # 头文件包含目录
AUX_SOURCE_DIRECTORY(src DIR_SRCS) # 添加源代码文件夹, 自动扫描所有文件

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

# 设置库文件路径和名称
set(LIB_PATH ${CMAKE_SOURCE_DIR}/lib)
MESSAGE(LIB_PATH)
set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/external)
set(SPDLOG_PATH ${EXTERNAL_PATH}/spdlog)

# 设置SPDLOG库的一些lib
set(SPDLOG_INCLUDE_DIR ${SPDLOG_PATH}/include)
message(STATUS "SPDLOG_INCLUDE_DIR IS ${SPDLOG_INCLUDE_DIR}")
set(SPDLOG_LIB ${SPDLOG_PATH}/lib/spdlog.lib)
set(SPDLOG_LIB_D ${SPDLOG_PATH}/lib/spdlogd.lib)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MyProject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        include/Define.h include/Initial.h include/lib.h include/utils.h
        src/utils.cpp
    )
    target_include_directories(MyProject PRIVATE ${SPDLOG_INCLUDE_DIR})
    target_link_libraries(MyProject PRIVATE ${SPDLOG_LIB})

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MyProject APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MyProject SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MyProject
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(MyProject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MyProject)
endif()
set_target_properties(MyProject PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MyProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MyProject)
endif()
