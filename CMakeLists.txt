cmake_minimum_required(VERSION 3.25)
project(klib LANGUAGES CXX
        VERSION 0.0.1
        DESCRIPTION "Kyle's C++ library")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILES TRUE)
set(VERBOSE YES)

if (MSVC)
    add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>")
else ()
    add_compile_options("-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options("-stdlib=libc++")
    else ()
        # nothing special for gcc at the moment
    endif ()
endif ()
add_compile_options("-DDESKTOP_BUILD")

set(HEADER_FILES
        klib.h
        Arena.h
        Buffer.h
        Dictionary.h
        Exceptions.h
        Test.h
        TLV.h)

set(SOURCE_FILES
        Arena.cc
        Buffer.cc
        Dictionary.cc
        Exceptions.cpp
        Test.cc
        TLV.cc)

add_library(klib STATIC ${SOURCE_FILES} ${HEADER_FILES})
add_executable(phonebook phonebook.cpp)
target_link_libraries(phonebook klib)

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

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        klibConfig.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
)

add_custom_target(cloc
        COMMAND cloc ${SOURCE_FILES} ${HEADER_FILES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

configure_file(klib.pc.in klib.pc @ONLY)
install(TARGETS klib LIBRARY DESTINATION ${PREFIX}/lib)
install(FILES ${HEADER_FILES} DESTINATION include/{klib})
install(FILES klibConfig.cmake DESTINATION share/klib/cmake)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/klib.pc DESTINATION lib/pkgconfig)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc/klib)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man DESTINATION share/man)

include(CMakePack.txt)
include(CMakeDocs.txt)
