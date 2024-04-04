cmake_minimum_required(VERSION 3.5)
project(RackWirer)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)

# Sets the version number
set (Sample_VERSION_MAJOR 0)
set (Sample_VERSION_MINOR 0)
set (Sample_VERSION_PATCH 1)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffunction-sections -fdata-sections")

# Tells cmake to load the entire src directory
include_directories("${PROJECT_SOURCE_DIR}/src/**")

# Set the sources and headers
file(GLOB_RECURSE RW_SOURCES CMAKE_CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE RW_HEADERS CMAKE_CONFIGURE_DEPENDS "src/*.h")

# Define a new variable as empty
set(RW_INCLUDE_DIRS "")

# goes for each header file and gets the directory and adds it to the include directories variable that was defined above
foreach (_headerFile ${RW_HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND RW_INCLUDE_DIRS ${_dir})
endforeach()

# Removes duplicates from the include directories variable
list(REMOVE_DUPLICATES RW_INCLUDE_DIRS)

# Sets the cpp files as the source files
add_executable(RackWirer ${RW_SOURCES})

# adds the header files to the include directories
target_include_directories(RackWirer PRIVATE ${RW_INCLUDE_DIRS})

# Pipewire
target_include_directories(RackWirer PUBLIC ${PIPEWIRE_INCLUDE_DIRS})
target_link_libraries(RackWirer ${PIPEWIRE_LIBRARIES})
target_compile_options(RackWirer PUBLIC ${PIPEWIRE_CFLAGS_OTHER})


# Adds dbus include directories
target_include_directories(RackWirer PUBLIC "/usr/include/dbus-1.0")
target_include_directories(RackWirer PUBLIC "/usr/lib/dbus-1.0/include")

# Adds x11 and dbus libraries
target_link_libraries(RackWirer "X11")
target_link_libraries(RackWirer "dbus-1")
target_link_options(RackWirer PUBLIC "-pthread")

# Adds c++0x, rdynamic, wall, and wextra flags
target_compile_options(RackWirer PUBLIC "-std=c++0x")
target_compile_options(RackWirer PUBLIC "-rdynamic")
target_compile_options(RackWirer PUBLIC "-Wall")
target_compile_options(RackWirer PUBLIC "-Wextra")
target_compile_options(RackWirer PRIVATE -Wno-gnu)

# Sets output directory
set_target_properties(RackWirer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out")
