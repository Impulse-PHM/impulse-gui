cmake_minimum_required(VERSION 3.19)

# Configuration values
set(APP_NAME "impulse-gui")
set(APP_DISPLAY_NAME "Impulse Personal Health Manager (Impulse PHM)")
set(APP_VERSION 0.1.0)
set(APP_AUTHOR "Joshua Goring")
set(APP_AUTHOR_EMAIL "qualitysoftwareguy@gmail.com")

# Generate the real build_config file, based on the config template,
# and store it with the other build files.
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/build_config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/build_config.h
  # Only replace @VAR@ placeholder instead of any that start with "${}"
  @ONLY
)

project(
    impulse-gui
    VERSION ${APP_VERSION}
    DESCRIPTION "Own and manage your personal health information."
    LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(impulse-gui
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Tells the target where to find the config header
target_include_directories(impulse-gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(impulse-gui
    PRIVATE
        Qt::Core
        Qt::Widgets
)

set_target_properties(impulse-gui PROPERTIES
    OUTPUT_NAME "impulse-phm"
)

include(GNUInstallDirs)

install(TARGETS impulse-gui
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET impulse-gui
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
