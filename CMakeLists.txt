cmake_minimum_required(VERSION 2.8)
project(CameraCalibration_OpenCV)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

find_package(OpenCV 3.0 QUIET)
if(NOT OpenCV_FOUND)
   find_package(OpenCV 2.4.3 QUIET)
   if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(
${PROJECT_SOURCE_DIR}
)

add_executable(TakeCameraImages take_camera_images.cc)
target_link_libraries(TakeCameraImages
${OpenCV_LIBS}
)

add_executable(CameraCalibration camera_calibration.cc)
target_link_libraries(CameraCalibration
${OpenCV_LIBS}
)

add_executable(CameraCalibrationFishEye camera_calibration_fisheye.cc)
target_link_libraries(CameraCalibrationFishEye
${OpenCV_LIBS}
)
