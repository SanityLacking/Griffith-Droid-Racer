SET(execName hermes)



#    FIND_PACKAGE(zed REQUIRED )
#    INCLUDE_DIRECTORIES(${ZED_INCLUDE_DIRS})
#    link_directories(${ZED_LIBRARY_DIR})
#    TARGET_LINK_LIBRARIES(MY_TARGET_NAME ${ZED_LIBRARIES})
#
#    This file will define the following variables:
#      - ZED_LIBRARIES      : The list of libraries to links against.
#      - ZED_LIBRARY_DIR    : The directory where lib files are.
#      - ZED_INCLUDE_DIRS   : The ZED include directories.

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 OLD)
	cmake_policy(SET CMP0015 OLD)
endif(COMMAND cmake_policy)

SET(EXECUTABLE_OUTPUT_PATH ".")


# Linux
find_package(ZED REQUIRED)




# Mandatory

find_package(OpenCV 2.4 COMPONENTS core highgui imgproc REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

link_directories(${OpenCV_LIBRARY_DIRS})
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)


SET(SRC_FOLDER src)
FILE(GLOB_RECURSE SRC_FILES "${SRC_FOLDER}/*.cpp")

FILE(GLOB_RECURSE SRC_CU_FOLDER "${SRC_FOLDER}/*.cu")

list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_20,code=sm_20)
list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_20,code=sm_21)
list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_30,code=sm_30)
list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_30,code=sm_35)
list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_50,code=sm_50)

set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -std=c++11)

cuda_add_executable(${execName} ${SRC_FILES} ${SRC_CU_FOLDER})

set_property(TARGET ${execName} PROPERTY OUTPUT_NAME "hermes")
add_definitions(-std=c++11)# -m64)

# Add the required libraries for linking:
TARGET_LINK_LIBRARIES(${execName}
			
			/usr/lib/libopencv_calib3d.so
			/usr/lib/libopencv_core.so
			/usr/lib/libopencv_highgui.so
			/usr/lib/libopencv_imgproc.so
			/usr/lib/libopencv_tegra.so
			${CUDA_LIBRARIES}
			${CUDA_nppi_LIBRARY}
			${CUDA_npps_LIBRARY}
			
			)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3" ) # Release Perf mode
