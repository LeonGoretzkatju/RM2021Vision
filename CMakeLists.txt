CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
project(TJU-RM-2021)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-w)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_PREFIX_PATH "/home/nuc/Qt5.12.0/5.12.0/gcc_64/")
add_definitions("-Wall -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPATH=\"\\\"${PROJECT_SOURCE_DIR}\\\"\"")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${CMAKE_SYSTEM_NAME}")
SET(BIN_NAME "TJU-RM-2021")

SET(OpenCV_DIR "/usr/local/lib/cmake/opencv4")
FIND_PACKAGE(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Widgets Gui Core PrintSupport XmlPatterns REQUIRED)
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
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Logger/Include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/3rd)


add_library(kcf_tracker STATIC
  KCFcpp/src/fhog.cpp
  KCFcpp/src/kcftracker.cpp)
target_link_libraries(kcf_tracker ${OpenCV_LIBRARIES})
target_include_directories(kcf_tracker PRIVATE KCFcpp/src)

add_subdirectory(3rd/fmt EXCLUDE_FROM_ALL)


FILE(GLOB_RECURSE sourcefiles "Other/Serial/Src/*.cpp" "Other/Camera/Src/*.cpp" "Tools/Src/*.cpp" "Armor/Src/*.cpp" "Predictor/Src/*.cpp" "Logger/Src/*.cpp")
ADD_EXECUTABLE(${BIN_NAME} Main.cpp ${sourcefiles} Tools/Include/Draw_Curve.h Tools/Src/Draw_Curve.cpp Predictor/Include/KalmanFilter.h Predictor/Src/KalmanFilter.cpp
              mainwindow.cpp mainwindow.h mainwindow.ui qcustomplot.cpp qcustomplot.h Predictor/Include/KalmanPosFilter.h Predictor/Src/KalmanPosFilter.cpp Predictor/Include/PidPosition.h Predictor/Src/PidPosition.cpp Other/Serial/Include/CRC_Check.h Other/Serial/Src/CRC_Check.cpp)

TARGET_LINK_LIBRARIES(${BIN_NAME} kcf_tracker ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(${BIN_NAME} ${OpenCV_LIBS})

TARGET_LINK_LIBRARIES(${BIN_NAME} "${PROJECT_SOURCE_DIR}/Other/Camera/libMVSDK.so")
target_link_libraries(${BIN_NAME} fmt-header-only)
target_link_libraries(${BIN_NAME} Qt5::Core Qt5::Widgets Qt5::Gui Qt5::PrintSupport Qt5::XmlPatterns)

ADD_CUSTOM_TARGET(create-startup COMMAND "${PROJECT_SOURCE_DIR}/Tools/create-startup.sh" "${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")
