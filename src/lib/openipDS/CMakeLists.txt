CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(PACKAGE_NAME openipDS)

INCLUDE_DIRECTORIES(${OPENIP_LIB_INCLUDE_DIR}/${PACKAGE_NAME})
AUX_SOURCE_DIRECTORY(${PACKAGE_NAME} SRCVAR0)
AUX_SOURCE_DIRECTORY(${PACKAGE_NAME}/minilzo SRCVAR1)
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCVAR0} ${SRCVAR1})

INSTALL(TARGETS ${PACKAGE_NAME} 
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)

SET(CMAKE_VERBOSE_MAKEFILE on)
