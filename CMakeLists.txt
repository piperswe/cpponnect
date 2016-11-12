cmake_minimum_required(VERSION 3.0)
project(cpponnect)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES app.cpp app.h server.cpp server.h middleware.h request-response.h request-response.cpp)
add_library(cpponnect ${SOURCE_FILES})
target_link_libraries(cpponnect ${Boost_LIBRARIES})

add_executable(helloworld test.cpp)
target_link_libraries(helloworld cpponnect)

install(TARGETS cpponnect
        ARCHIVE DESTINATION lib)
install(DIRECTORY .
        DESTINATION include/cpponnect
        FILES_MATCHING PATTERN "*.h")