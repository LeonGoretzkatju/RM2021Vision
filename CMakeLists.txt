CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
project(TJU-RM-2021)
add_definitions(-w)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE RELEASE)
add_definitions("-Wall -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")

SET(OpenCV_DIR "/usr/local/lib/cmake/opencv4")
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(Threads)

FIND_PACKAGE(Eigen3)


# LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/Others)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Armor/Include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Predictor/Include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Tools/Include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Other/Serial/Include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Other/Camera/Include)

add_library(kcf_tracker STATIC
  KCFcpp/src/fhog.cpp
  KCFcpp/src/kcftracker.cpp)
target_link_libraries(kcf_tracker ${OpenCV_LIBRARIES})
target_include_directories(kcf_tracker PRIVATE KCFcpp/src)


FILE(GLOB_RECURSE sourcefiles "Other/Serial/Src/*.cpp" "Other/Camera/Src/*.cpp" "Tools/Src/*.cpp" "Armor/Src/*.cpp" "Predictor/Src/*.cpp")
ADD_EXECUTABLE(TJU-RM-2021 Main.cpp ${sourcefiles} Tools/Include/Draw_Curve.h Tools/Src/Draw_Curve.cpp Predictor/Include/KalmanFilter.h Predictor/Src/KalmanFilter.cpp)

TARGET_LINK_LIBRARIES(TJU-RM-2021 kcf_tracker ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(TJU-RM-2021 ${OpenCV_LIBS})

TARGET_LINK_LIBRARIES(TJU-RM-2021 "${PROJECT_SOURCE_DIR}/Other/Camera/libMVSDK.so")