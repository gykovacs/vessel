CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(PACKAGE_NAME openipIO)

INCLUDE_DIRECTORIES(${JPEG_INCLUDE_DIR} 
		    ${TIFF_INCLUDE_DIR} 
		    ${PNG_INCLUDE_DIR} 
		    ${MINC2_INCLUDE} 
		    ${MINC_INCLUDE} 
		    ${HDF5_INCLUDE} 
		    ${NETCDF_INCLUDE} 
		    ${OPENIP_LIB_INCLUDE_DIR}/openipDS/
		    ${OPENIP_LIB_INCLUDE_DIR}/openipIO/)

AUX_SOURCE_DIRECTORY(${PACKAGE_NAME} SRCVAR0)
AUX_SOURCE_DIRECTORY(${PACKAGE_NAME}/bmp SRCVAR1)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCVAR0} ${SRCVAR1})

TARGET_LINK_LIBRARIES(${PACKAGE_NAME} 
			${JPEG_LIBRARY} 
			${TIFF_LIBRARY} 
			${PNG_LIBRARY} 
			${MINC2_LIB} 
			${HDF5_LIB} 
			${NETCDF_LIB} 
			openipDS)

INSTALL(TARGETS ${PACKAGE_NAME} 
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)

SET(CMAKE_VERBOSE_MAKEFILE on)
