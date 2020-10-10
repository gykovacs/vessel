CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(PACKAGE_NAME vessel)

INCLUDE_DIRECTORIES(src 
		    ${GSL_INCLUDE_DIR}
		    ${IMAGEIO_INCLUDE_DIRS}
		    ${OPENIP_LIB_INCLUDE_DIR}/openipDS/ 
		    ${OPENIP_LIB_INCLUDE_DIR}/openipSC/ 
		    ${OPENIP_LIB_INCLUDE_DIR}/openipIO/ 
		    ${OPENIP_LIB_INCLUDE_DIR}/openipML/ 
		    ${OPENIP_LIB_INCLUDE_DIR}/openipLL/)

AUX_SOURCE_DIRECTORY(src SRCVAR0)

ADD_EXECUTABLE(${PACKAGE_NAME} ${SRCVAR0})

TARGET_LINK_LIBRARIES(${PACKAGE_NAME} 
		    ${GSL_LIBRARIES} 
		    ${IMAGEIO_LIBS} 
		    openipDS
		    openipSC
		    openipIO
		    openipML
		    openipLL)

INSTALL(TARGETS ${PACKAGE_NAME} DESTINATION bin)
FILE(GLOB FILESDRIVE "../../../share/vessel/drive-model/*")
FILE(GLOB FILESSTARE "../../../share/vessel/stare-model/*")
INSTALL(FILES ${FILESDRIVE} DESTINATION share/vessel/drive-model/)
INSTALL(FILES ${FILESSTARE} DESTINATION share/vessel/stare-model/)
if (UNIX)
    INSTALL(FILES ../../../readme/linux/README.txt DESTINATION share/vessel)
endif (UNIX)
if (WIN32)
    INSTALL(FILES ../../../readme/windows/README.txt DESTINATION share/vessel)
endif (WIN32)

INSTALL(PROGRAMS ../../../scripts/vessel.py DESTINATION bin)
		    
SET(CMAKE_VERBOSE_MAKEFILE on)
