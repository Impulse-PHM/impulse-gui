cmake_minimum_required(VERSION 3.19)

set(APP_NAME "impulse-gui")
set(APP_DISPLAY_NAME "Impulse Personal Health Manager (Impulse PHM)")
set(APP_VERSION 0.1.0)
set(APP_AUTHOR "Joshua Goring")
set(APP_AUTHOR_EMAIL "qualitysoftwareguy@gmail.com")

# Config values to help integrate impulse-core via the "cxx" Rust crate
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(IMPULSE_CORE_DIR ${CMAKE_CURRENT_LIST_DIR}/../impulse-core)
set(RUST_TARGET_DIR ${IMPULSE_CORE_DIR}/target)
set(CXXBRIDGE_DIR ${RUST_TARGET_DIR}/cxxbridge)
set(IMPULSE_CORE_BRIDGE_CC ${CXXBRIDGE_DIR}/impulse-core/src/bridge.rs.cc)

# Determine which Rust profile to use for impulse-core -- by default go with debug for convenience
set(RUST_PROFILE "debug")
# Could not find a more convenient way for CMake to compare a string while ignoring its letter case
string(TOLOWER "${CMAKE_BUILD_TYPE}" _cmake_build_type)
if(_cmake_build_type STREQUAL "release" OR
    _cmake_build_type STREQUAL "relwithdebinfo" OR
    _cmake_build_type STREQUAL "minsizerel")
    set(RUST_PROFILE "release")
endif()


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
    src/main.cpp
    ${IMPULSE_CORE_BRIDGE_CC}
    src/mainwindow.h
    src/mainwindow.cpp
    src/mainwindow.ui
    src/welcomedialog.h
    src/welcomedialog.cpp
    src/welcomedialog.ui
    src/util/notification.h
    src/util/notification.cpp
    src/newuserdialog.h
    src/newuserdialog.cpp
    src/newuserdialog.ui
)

qt_add_resources(impulse-gui "resources"
    PREFIX "/"
    FILES
        styles/main.qss
)

# Tells the target where to find the custom header files
target_include_directories(impulse-gui PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CXXBRIDGE_DIR}
)

target_link_libraries(impulse-gui
    PRIVATE
        Qt::Core
        Qt::Widgets
        ${RUST_TARGET_DIR}/${RUST_PROFILE}/libimpulse_core.a
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
