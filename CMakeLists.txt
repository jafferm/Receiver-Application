cmake_minimum_required(VERSION 3.10)
project(ReceiverApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GLIB REQUIRED glib-2.0)
find_package(Boost REQUIRED)

add_executable(ReceiverApp 
    src/ReceiverMain.cpp
    src/ReceiverPipelineMgr.cpp
    )

target_include_directories(ReceiverApp PRIVATE
    ${GSTREAMER_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

target_link_directories(ReceiverApp PRIVATE
    ${GSTREAMER_LIBRARY_DIRS}
    ${GLIB_LIBRARY_DIRS}
)

target_link_libraries(ReceiverApp
    ${GSTREAMER_LIBRARIES}
    ${GLIB_LIBRARIES}
)

# Optional: cleaner warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(ReceiverApp PRIVATE -Wall -Wextra -Wpedantic)
endif()
