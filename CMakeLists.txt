cmake_minimum_required(VERSION 3.16)

project(TINYLanguageScanner
    VERSION 1.0.0
    DESCRIPTION "Professional Desktop Scanner for TINY Language in C++ & Qt Widgets"
    LANGUAGES CXX
)

# Enforce C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt Automation
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Discover Qt6 or Qt5 seamlessly
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

message(STATUS "Configuring with Qt version: ${Qt${QT_VERSION_MAJOR}_VERSION}")

# Modular Source Files
set(MODEL_SOURCES
    src/models/Token.cpp
    src/models/Scanner.cpp
)

set(CONTROLLER_SOURCES
    src/controllers/MainController.cpp
)

set(VIEW_SOURCES
    src/views/MainWindow.cpp
    src/views/CodeEditor.cpp
)

set(UTIL_SOURCES
    src/utils/Theme.cpp
)

# Application Executable
# WIN32 prevents console window popup on Windows desktop release
add_executable(TINYScanner WIN32
    src/main.cpp
    ${MODEL_SOURCES}
    ${CONTROLLER_SOURCES}
    ${VIEW_SOURCES}
    ${UTIL_SOURCES}
)

target_include_directories(TINYScanner PRIVATE
    src
    src/models
    src/controllers
    src/views
    src/utils
)

target_link_libraries(TINYScanner PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

# Compile Options & Optimization
if(MSVC)
    target_compile_options(TINYScanner PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(TINYScanner PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Standalone Scanner Unit Tests (Does not require Qt GUI)
enable_testing()
add_executable(ScannerUnitTests
    tests/test_scanner.cpp
    src/models/Token.cpp
    src/models/Scanner.cpp
)
target_include_directories(ScannerUnitTests PRIVATE
    src/models
)
add_test(NAME ScannerUnitTests COMMAND ScannerUnitTests)

# Installation Rules for Standalone Deployment
install(TARGETS TINYScanner DESTINATION bin)
