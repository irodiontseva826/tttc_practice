cmake_minimum_required(VERSION 3.30)

project(PrintDataTypeInfo)

set(LLVM_DIR $ENV{LLVM_DIR})
set(Clang_DIR $ENV{Clang_DIR})

find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

include(AddLLVM)

add_llvm_library(PrintDataTypeInfo  MODULE PrintDataTypeInfo.cpp PLUGIN_TOOL clang)
