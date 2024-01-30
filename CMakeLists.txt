project(YOLOv5s)

cmake_minimum_required(VERSION 3.10.2)

# 获取OpenCV的静态链接库:w
# set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opencv_lib)
# file(GLOB OpenCV_LIBS "${LIB_DIR}/*.a")
#set(OpenCV_LIBS
#    libopencv_highgui.a
#    libopencv_imgproc.a
#    libopencv_core.a
#)

set(CMAKE_CXX_FLAGS "-mfloat-abi=hard")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/workspace)

#{CMAKE_CURRENT_SOURCE_DIR}/src/test_opencv_crosshair.cpp
#	     ${CMAKE_CURRENT_SOURCE_DIR}/src/v4l2_camera.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/src/v5lite-s
set(SRC_FILE 
			 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_opencv_crosshair.cpp
	     	 ${CMAKE_CURRENT_SOURCE_DIR}/src/v4l2_camera.cpp
	)

add_executable(main ${SRC_FILE}) 

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/ncnn)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/opencv)

target_link_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/ncnn)
target_link_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv)

target_link_libraries(main ncnn ${OpenCV_LIBS} -fopenmp -lopencv_highgui -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video -lopencv_calib3d -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_core  -llibprotobuf -littnotify -llibjpeg-turbo -llibwebp -llibpng -llibtiff -llibopenjp2 -lIlmImf -lzlib -lquirc -ldl -lm -lpthread -lrt -lstdc++)



