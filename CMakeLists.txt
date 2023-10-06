cmake_minimum_required(VERSION 3.25)
project(klib CXX)

set(CMAKE_CXX_STANDARD 14)

add_library(klib STATIC
	Arena.cc
	TLV.cc)

add_executable(tlv_test tlv_test.cc)
target_link_libraries(tlv_test klib)
