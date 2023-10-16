cmake_minimum_required(VERSION 3.22)
project(scsl LANGUAGES CXX
        VERSION 0.2.2
        DESCRIPTION "Shimmering Clarity Standard Library")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILES TRUE)
set(VERBOSE YES)

# compile options:
#   -Wall                   Default to all errors.
#   -Wextra                 And a few extra.
#   -Werror                 And require them to be fixed to build.
#   -Wno-unused-function    This is a library. Not every function is used here.
#   -Wno-unused-parameter   Some functions have parameters defined for compatibility,
#                           and aren't used in the implementation.
add_compile_options(
        "-static"
        "-Wall"
        "-Wextra"
        "-Werror"
        "-Wno-unused-function"
        "-Wno-unused-parameter"
        "-g"
        "$<$<CONFIG:RELEASE>:-O2>"
)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options("-stdlib=libc++")
else ()
    # nothing special for gcc at the moment
endif ()

add_compile_definitions(SCSL_DESKTOP_BUILD)
add_compile_definitions(SCSL_VERSION=${PROJECT_VERSION})

set(HEADER_FILES scsl.h
        Arena.h
        Buffer.h
        Commander.h
        Dictionary.h
        Exceptions.h
        Flag.h
        StringUtil.h
        TLV.h
        Test.h
)

set(SOURCE_FILES
        Arena.cc
        Buffer.cc
        Commander.cc
        Dictionary.cc
        Exceptions.cc
        Flag.cc
        StringUtil.cc
        TLV.cc
        Test.cc
)

if (APPLE)
add_library(scsl
        STATIC
        ${SOURCE_FILES} ${HEADER_FILES})
else ()
add_library(scsl
        STATIC
        ${SOURCE_FILES} ${HEADER_FILES})
endif()

add_executable(phonebook phonebook.cc)
target_link_libraries(phonebook scsl)

include(CTest)
enable_testing()

add_executable(buffer_test bufferTest.cc)
target_link_libraries(buffer_test scsl)
add_test(bufferTest buffer_test)

add_executable(tlv_test tlvTest.cc)
target_link_libraries(tlv_test scsl)
add_test(tlvTest tlv_test)

add_executable(dictionary_test dictionaryTest.cc)
target_link_libraries(dictionary_test scsl)
add_test(dictionaryTest dictionary_test)

add_executable(flag_test flagTest.cc)
target_link_libraries(flag_test scsl)
add_test(flagTest flag_test)

add_executable(stringutil_test stringutil_test.cc)
target_link_libraries(stringutil_test scsl)
add_test(stringutilTest stringutil_test)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        scslConfig.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
)

if (NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(cloc
        COMMAND cloc ${SOURCE_FILES} ${HEADER_FILES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_custom_target(deploy-docs
        COMMAND rsync --delete-after --progress -auvz ${CMAKE_CURRENT_BINARY_DIR}/html/* docs.shimmering-clarity.net:sites/cc/${PROJECT_NAME}/
        DEPENDS scsl_docs
)

configure_file(scsl.pc.in scsl.pc @ONLY)
install(TARGETS scsl LIBRARY DESTINATION lib)
install(TARGETS phonebook RUNTIME DESTINATION bin)
install(FILES ${HEADER_FILES} DESTINATION include/scsl)
install(FILES scslConfig.cmake DESTINATION share/scsl/cmake)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/scsl.pc DESTINATION lib/pkgconfig)

include(cmake/packaging.cmake)
include(cmake/docs.cmake)
endif()
