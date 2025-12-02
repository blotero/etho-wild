cmake_minimum_required(VERSION 3.20)
project(EthoWild VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find dependencies (provided by vcpkg)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)
find_package(OpenCV REQUIRED)

# Qt Automoc/uic/rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/VideoWorker.cpp
    src/Config.cpp
    src/CsvExporter.cpp
    src/ThemeManager.cpp
)

# Headers (for MOC)
set(HEADERS
    src/MainWindow.hpp
    src/VideoWorker.hpp
    src/Config.hpp
    src/CsvExporter.hpp
    src/BehaviorRecord.hpp
    src/ThemeManager.hpp
)

add_executable(EthoWild ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(EthoWild PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    ${OpenCV_LIBS}
)

# Copy behaviors.json config file to build directory
configure_file(
    ${CMAKE_SOURCE_DIR}/behaviors.json
    ${CMAKE_BINARY_DIR}/behaviors.json
    COPYONLY
)

# Installation rules
install(TARGETS EthoWild DESTINATION bin)
install(FILES behaviors.json DESTINATION bin)
