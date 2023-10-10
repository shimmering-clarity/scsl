cmake_minimum_required(VERSION 3.25)
project(klib LANGUAGES CXX VERSION 0.0.1)

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

include(CTest)
enable_testing()

add_executable(tlv_test tlvTest.cc)
target_link_libraries(tlv_test klib)
add_test(tlvTest tlv_test)

add_executable(dictionary_test dictionaryTest.cc)
target_link_libraries(dictionary_test klib)
add_test(dictionaryTest dictionary_test)

add_executable(buffer_test bufferTest.cc)
target_link_libraries(buffer_test klib)
add_test(bufferTest buffer_test)

include(CMakePack.txt)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	klibConfig.cmake
	VERSION ${PACKAGE_VERSION}
	COMPATIBILITY AnyNewerVersion
)