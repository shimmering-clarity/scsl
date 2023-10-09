cmake_minimum_required(VERSION 3.25)
project(klib CXX)

set(CMAKE_CXX_STANDARD 14)
if(MSVC)
	add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>")
else()
	add_compile_options("-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		add_compile_options("-stdlib=libc++")
	else()
		# nothing special for gcc at the moment
	endif()
endif()
add_compile_options("-DDESKTOP_BUILD")

set(HEADER_FILES
	Arena.h
	Buffer.h
	Dictionary.h
	TLV.h)

add_library(klib STATIC
	Arena.cc
	Buffer.cc
	Dictionary.cc
	TLV.cc)
install(TARGETS klib LIBRARY DESTINATION ${PREFIX}/lib)
install(FILES ${HEADER_FILES} DESTINATION include/klib)
install(FILES klibConfig.cmake DESTINATION share/klib/cmake)

add_executable(tlv_test tlvTest.cc)
target_link_libraries(tlv_test klib)

add_executable(dictionary_test dictionaryTest.cc)
target_link_libraries(dictionary_test klib)

add_executable(buffer_test bufferTest.cc)
target_link_libraries(buffer_test klib)

