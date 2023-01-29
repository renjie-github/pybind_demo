cmake_minimum_required(VERSION 3.18...3.20)

# 11, 14, 17, 20
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ version selection")

set(PYTHON_VERSION 3.10)
# Classic Python support: The target Python version can be selected by 
# setting PYBIND11_PYTHON_VERSION or an exact Python installation can 
# be specified with PYTHON_EXECUTABLE
set(PYBIND11_PYTHON_VERSION ${PYTHON_VERSION})
# where to save output dynamic lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

project(pybind_demo VERSION 1.0 LANGUAGES CXX)

# find python (with specified version) of current dev env for find Python.h,  
# identical to: include_directories(/usr/include/python3.10/)
find_package(Python ${PYTHON_VERSION} COMPONENTS Interpreter Development REQUIRED)

add_subdirectory(./extern/pybind11)
include_directories(SYSTEM extern/pybind11/include)
##[Deprecated] find pybind11, identical to: add_subdirectory(./extern/pybind11/install)
## find_package(pybind11 CONFIG REQUIRED PATHS ./extern/pybind11/install)

# specify path to source code
add_subdirectory(./pybind_demo/_libs/src)
# target_include_directories(mylib PUBLIC /mnt/d/Code/Practice/Cpp/pybind_demo/pybind_demo/_libs/src)
# aux_source_directory("./pybind_demo/_libs/src" DIR_SRC)
# add_library(mylib SHARED ${DIR_SRC})