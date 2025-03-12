cmake_minimum_required(VERSION 3.19)
project(timp LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network Test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_standard_project_setup()

# основное приложение (тут БЕЗ тестов!!!)
qt_add_executable(timp
    WIN32 MACOSX_BUNDLE
    main.cpp
    apiservice.h apiservice.cpp
    chatcontroller.h chatcontroller.cpp
    dialog.h dialog.cpp dialog.ui
    loginwindow.h loginwindow.cpp loginwindow.ui
    messageformatter.h messageformatter.cpp
    emojiconverter.h emojiconverter.cpp
    # УБРАТЬ testcontroller.* отсюда, если они есть!!!
)

target_link_libraries(timp
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

# тесты - ОТДЕЛЬНЫМ таргетом
qt_add_executable(timp_tests
    testcontroller.h testcontroller.cpp
    apiservice.h apiservice.cpp
    chatcontroller.h chatcontroller.cpp
    messageformatter.h messageformatter.cpp
    emojiconverter.h emojiconverter.cpp
)

target_link_libraries(timp_tests
    PRIVATE
        Qt::Core
        Qt::Test
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
