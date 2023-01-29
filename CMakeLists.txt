# CMakeList.txt: MCRCONnogui 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("MCRCONnogui")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#asio目录
set(ASIO_PATH "F:/Desktop/c++/asio-1.24.0")
include_directories("${ASIO_PATH}/include")

#Python目录
set(PYTHON_PATH "C:/Program Files (x86)/Microsoft Visual Studio/Shared/Python39_64")
#Python lib文件名
set(PYTHON_LIB "python39")
include_directories("${PYTHON_PATH}/include")

include_directories("${CMAKE_SOURCE_DIR}/include")

file(GLOB CPP_FILES
	"${CMAKE_SOURCE_DIR}/*.h"
	"${CMAKE_SOURCE_DIR}/*.cpp"
	"${CMAKE_SOURCE_DIR}/*.ixx"

	"${CMAKE_SOURCE_DIR}/include/*.h"
	"${CMAKE_SOURCE_DIR}/include/*.ixx"
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
)

if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /O2 /Ot /GL /Gy")

	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif(MSVC)

# 将源代码添加到此项目的可执行文件。
add_executable (MCRCONnogui ${CPP_FILES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_link_libraries(MCRCONnogui ${PYTHON_PATH}/libs/${PYTHON_LIB}_d.lib)
else()
	target_link_libraries(MCRCONnogui ${PYTHON_PATH}/libs/${PYTHON_LIB}.lib)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET MCRCONnogui PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
