cmake_minimum_required(VERSION 3.19)
project(timp LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_standard_project_setup()

qt_add_executable(timp
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    networkmanager.h networkmanager.cpp
    tester.h tester.cpp
    loginwindow.h loginwindow.cpp loginwindow.ui
    dialog.h dialog.cpp
    dialog.h dialog.cpp dialog.ui
    messageformatter.h messageformatter.cpp
    emojiconverter.h emojiconverter.cpp
)

target_link_libraries(timp
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

include(GNUInstallDirs)

install(TARGETS timp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET timp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
