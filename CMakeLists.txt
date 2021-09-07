cmake_minimum_required(VERSION 3.20)
project(Compressor)

set(CMAKE_CXX_STANDARD 20)

include_directories("E:/Boost/sub boost/")
link_directories("E:/Boost/boost_1_77_0/stage/lib/")

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
	add_compile_definitions(__DEBUG__)
endif ()

add_executable(${PROJECT_NAME} main.cpp)

if (${CMAKE_BUILD_TYPE} MATCHES ReleaseX64)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Compressor.exe D:/PATH/
		COMMENT "Copying Release build to D:/PATH/"
	)
endif ()
