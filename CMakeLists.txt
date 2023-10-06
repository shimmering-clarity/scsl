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

add_library(klib STATIC
	Arena.cc
	TLV.cc
	Phonebook.cc)

add_executable(tlv_test tlv_test.cc)
target_link_libraries(tlv_test klib)

add_executable(phonebook_test phonebook_test.cc)
target_link_libraries(phonebook_test klib)

