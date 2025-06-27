cmake_minimum_required(VERSION 3.16)
project(telback VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Quick QuickControls2 Sql SerialPort
)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(apptelback
    main.cpp
    packetreceiver.cpp
    packetcontroller.cpp
    telemetrymodel.cpp
    CircularBuffer.h
    FileLogger.h
    packetreceiver.h
    packetcontroller.h
    telemetrymodel.h
)

qt_add_qml_module(apptelback
    URI telback
    VERSION 1.0
    QML_FILES
        Main.qml
        PacketView.qml
        QML_FILES TelemetryDashboard.qml
)

set_target_properties(apptelback PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION        ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING  ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE                       TRUE
    WIN32_EXECUTABLE                    TRUE
)

target_link_libraries(apptelback PRIVATE
    Qt6::Quick Qt6::QuickControls2 Qt6::Sql Qt6::SerialPort
)

include(GNUInstallDirs)
install(TARGETS apptelback
    BUNDLE   DESTINATION .
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
