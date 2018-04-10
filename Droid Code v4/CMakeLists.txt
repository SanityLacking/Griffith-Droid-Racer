cmake_minimum_required(VERSION 3.10)
project(droid_racer)

set(CMAKE_CXX_STANDARD 11)

set(DROID_DIR ./droid)
set(CLIENT_DIR ./client)
set(RES_DIR ./res)


include_directories(${DROID_DIR})
include_directories(${CLIENT_DIR})
include_directories(${RES_DIR})

set (
        SOURCE_FILES
            main.cpp
            ${DROID_DIR}/EngineController.h
            ${DROID_DIR}/EngineController.cpp
            ${DROID_DIR}/ArduinoController.h
            ${DROID_DIR}/ArduinoController.cpp
            ${DROID_DIR}/server.cpp
)
add_executable(droid_racer ${SOURCE_FILES})