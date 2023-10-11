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
    # compile options:
    #   -Wall                   Default to all errors.
    #   -Wextra                 And a few extra.
    #   -Werror                 And require them to be fixed to build.
    #   -Wno-unused-function    This is a library. Not every function is used here.
    #   -Wno-unused-parameter   Some functions have parameters defined for compatibility,
    #                           and aren't used in the implementation.

    add_compile_options("-Wall" "-Wextra" "-Werror" "-Wno-unused-function" "-Wno-unused-parameter" "$<$<CONFIG:RELEASE>:-O2>")
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
        TLV.h
        WinHelpers.h)

set(SOURCE_FILES
        Arena.cc
        Buffer.cc
        Dictionary.cc
        Exceptions.cc
        Test.cc
        TLV.cc
        Commander.cc
        Commander.h
        WinHelpers.cc)

if (APPLE)
add_library(klib
        STATIC
        ${SOURCE_FILES} ${HEADER_FILES})
else ()
add_library(klib
        STATIC
        ${SOURCE_FILES} ${HEADER_FILES})
endif()

add_executable(phonebook phonebook.cc)
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

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(cloc
        COMMAND cloc ${SOURCE_FILES} ${HEADER_FILES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

configure_file(klib.pc.in klib.pc @ONLY)
install(TARGETS klib LIBRARY DESTINATION lib)
install(TARGETS phonebook RUNTIME DESTINATION bin)
install(FILES ${HEADER_FILES} DESTINATION include/klib)
install(FILES klibConfig.cmake DESTINATION share/klib/cmake)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/klib.pc DESTINATION lib/pkgconfig)

include(CMakePack.txt)
include(CMakeDocs.txt)
endif()
