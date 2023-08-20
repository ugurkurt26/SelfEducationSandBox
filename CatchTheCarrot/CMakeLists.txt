cmake_minimum_required(VERSION 3.0.0)
project(CatchTheCarrot)

set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON) 

file(GLOB CatchTheCarrot_SRC
     "*.cpp"
)

file(GLOB CatchTheCarrot_HDR
     "*.hpp"
)

add_executable(CatchTheCarrot ${CatchTheCarrot_SRC} ${CatchTheCarrot_HDR})

find_package(Boost 1.71.0 REQUIRED COMPONENTS system regex random thread filesystem iostreams)
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui ml video features2d calib3d objdetect videoio imgcodecs flann)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(CatchTheCarrot ${Boost_LIBRARIES})
target_link_libraries(CatchTheCarrot ${OpenCV_LIBS})

set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -Wall -fmessage-length=0 -pthread")

target_compile_features(CatchTheCarrot PRIVATE cxx_std_17)
