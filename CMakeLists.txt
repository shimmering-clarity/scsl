cmake_minimum_required(VERSION 3.22)
project(scsl LANGUAGES CXX
        VERSION 1.1.3
        DESCRIPTION "Shimmering Clarity Standard Library")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILES TRUE)
set(VERBOSE YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
#add_link_options("-fsanitize=address")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options("-stdlib=libc++")
else ()
    # nothing special for gcc At the moment
endif ()

add_compile_definitions(SCSL_DESKTOP_BUILD)
add_compile_definitions(SCSL_VERSION=${PROJECT_VERSION})

set(HEADER_FILES
        include/scsl/scsl.h
        include/scsl/Arena.h
        include/scsl/Buffer.h
        include/scsl/Commander.h
        include/scsl/Dictionary.h
        include/scsl/Flags.h
        include/scsl/SimpleConfig.h
        include/scsl/StringUtil.h
        include/scsl/TLV.h

        include/scmp/estimation.h
        include/scmp/geom.h
        include/scmp/scmp.h
        include/scmp/Math.h
        include/scmp/geom/Coord2D.h
        include/scmp/geom/Orientation.h
        include/scmp/geom/Quaternion.h
        include/scmp/geom/Vector.h
        include/scmp/estimation/Madgwick.h

        include/sctest/sctest.h
        include/sctest/Assert.h
        include/sctest/Checks.h
        include/sctest/Exceptions.h
        include/sctest/Report.h
        include/sctest/SimpleSuite.h
)

include_directories(include)

set(SOURCE_FILES
        src/sl/Arena.cc
        src/sl/Buffer.cc
        src/sl/Commander.cc
        src/sl/Dictionary.cc
        src/test/Exceptions.cc
        src/sl/Flags.cc
        src/sl/SimpleConfig.cc
        src/sl/StringUtil.cc
        src/sl/TLV.cc

        src/scmp/Math.cc
        src/scmp/Coord2D.cc
        src/scmp/Orientation.cc
        src/scmp/Quaternion.cc

        src/test/Assert.cc
        src/test/Report.cc
        src/test/SimpleSuite.cc
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

add_executable(phonebook src/bin/phonebook.cc)
target_link_libraries(phonebook scsl)

include(CTest)
enable_testing()

set(TEST_SOURCES)
macro(generate_test name)
    add_executable(test_${name} test/${name}.cc ${TEST_SOURCES} ${ARGN})
    target_link_libraries(test_${name} ${PROJECT_NAME})
    target_include_directories(test_${name} PRIVATE test)
    add_test(test_${name} test_${name})
endmacro()

# core standard library
generate_test(buffer)
generate_test(tlv)
generate_test(dictionary)
generate_test(stringutil)

# math and physics
generate_test(coord2d)
generate_test(madgwick)
generate_test(math)
generate_test(orientation)
generate_test(quaternion)
generate_test(vector)

# test tooling
add_executable(flags-demo test/flags.cc)
target_link_libraries(flags-demo ${PROJECT_NAME})

add_executable(simple-test-demo test/simple_suite_example.cc)
target_link_libraries(simple-test-demo ${PROJECT_NAME})

add_executable(config-explorer test/config-explorer.cc)
target_link_libraries(config-explorer ${PROJECT_NAME})

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
install(DIRECTORY include/ DESTINATION include)
install(FILES scslConfig.cmake DESTINATION share/scsl/cmake)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/scsl.pc DESTINATION lib/pkgconfig)

include(cmake/packaging.cmake)
include(cmake/docs.cmake)
endif()
