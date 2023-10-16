# Doxygen support for scsl.
find_package(Doxygen)

if (${DOXYGEN_FOUND})
set(DOXYGEN_GENERATE_MAN YES)
set(DOXYGEN_GENERATE_LATEX YES)
set(DOXYGEN_EXTRACT_ALL YES)
message(STATUS "Doxygen found, building docs.")

doxygen_add_docs(scsl_docs
	${HEADER_FILES} ${SOURCE_FILES}
	USE_STAMP_FILE)
add_dependencies(scsl scsl_docs)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc/scsl)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man DESTINATION share)

endif ()

