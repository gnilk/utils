#
# CMAKE file for Gnilk Utility Library
#
# Logger and XML parser have own repo's - don't use this one
#
set(CMAKE_OSX_ARCHITECTURES x86_64)
set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)
cmake_minimum_required(VERSION 2.8)


project(utilities)

set(CMAKE_SOURCE_DIR .)
set(CMAKE_BINARY_DIR ./bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ./lib)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

include_directories("${PROJECT_SOURCE_DIR}")

#
# Utility library
#

# core 
list(APPEND srcutils curve.cpp)
list(APPEND srcutils event.cpp)
list(APPEND srcutils expsolver.cpp)
list(APPEND srcutils observer.cpp)
list(APPEND srcutils PropertyContainer.cpp)
list(APPEND srcutils thread.cpp)
list(APPEND srcutils tokenizer.cpp)
list(APPEND srcutils urlparser.cpp)
list(APPEND srcutils memfile.cpp)

# tests
list(APPEND srcutils tests/test_memfile.cpp) 
list(APPEND srcutils tests/test_expsolver.cpp) 
list(APPEND srcutils tests/test_xmlparser.cpp) 


add_library(gnilkutil SHARED ${srcutils})
target_include_directories(gnilkutil PUBLIC .)
target_include_directories(gnilkutil PUBLIC /usr/local/include)

#set_property(TARGET gnilkutil PROPERTY CXX_STANDARD 11)
set_property(TARGET gnilkutil PROPERTY CXX_STANDARD 11)

add_definitions(-D__MEMFILE_STD__)

#add_definitions(-D__GOA_GLFW__)
#add_definitions(-D__GOA_MACOS__)


#
# solver
#
add_executable(solver solver.cpp expsolver.cpp tokenizer.cpp)
target_include_directories(solver PRIVATE .)

add_executable(utilstest utilstest.cpp)
target_include_directories(utilstest PRIVATE .)
set_property(TARGET utilstest PROPERTY CXX_STANDARD 11)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/local/lib)

if (APPLE) 
    message(STATUS ${CMAKE_OSX_ARCHITECTURES})
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(CORE_FRAMEWORK CoreFoundation)
   
    list(APPEND libdep ${COCOA_FRAMEWORK} ${IOKIT_FRAMEWORK})
endif()
target_link_libraries(gnilkutil ${libdep})

target_link_libraries(solver ${libdep})
target_link_libraries(utilstest gnilkutil ${libdep})
