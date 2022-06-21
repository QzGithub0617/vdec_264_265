cmake_minimum_required(VERSION 3.10)
project(ffmpeg_demo)

set(ffmpeg_libs_DIR /usr/lib/x86_64-linux-gnu)
set(ffmpeg_headers_DIR /usr/include/x86_64-linux-gnu)

add_library( avcodec SHARED IMPORTED)
add_library( avfilter SHARED IMPORTED )
add_library( swresample SHARED IMPORTED )
add_library( swscale SHARED IMPORTED )
add_library( avformat SHARED IMPORTED )
add_library( avutil SHARED IMPORTED )


set_target_properties( avcodec PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavcodec.so )
set_target_properties( avfilter PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavfilter.so )
set_target_properties( swresample PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libswresample.so )
set_target_properties( swscale PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libswscale.so )
set_target_properties( avformat PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavformat.so )
set_target_properties( avutil PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavutil.so )


# 添加头文件路径到编译器的头文件搜索路径下，多个路径以空格分隔
include_directories( ${ffmpeg_headers_DIR}  ./src)
link_directories(${ffmpeg_libs_DIR} )


set(CMAKE_CXX_STANDARD 14)
add_executable(ffmpeg_demo src/main.cpp src/video_decoder.cpp) 
target_link_libraries(${PROJECT_NAME}  avcodec avformat avutil swresample swscale swscale avfilter )

