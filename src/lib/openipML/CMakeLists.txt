CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(PACKAGE_NAME openipML)

#PROJECT(${PACKAGE_NAME} CXX)

INCLUDE_DIRECTORIES(${OPENIP_LIB_INCLUDE_DIR}/openipML
		    ${OPENIP_LIB_INCLUDE_DIR}/openipDS
		    ${OPENIP_LIB_INCLUDE_DIR}/openipSC
		    ${OPENIP_LIB_INCLUDE_DIR}/openipIO)

AUX_SOURCE_DIRECTORY(${PACKAGE_NAME} SRCVAR0)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCVAR0} ${PACKAGE_NAME}/fann/doublefann.c
#					      ${PACKAGE_NAME}/fann/fann.c
#					      ${PACKAGE_NAME}/fann/fann_cascade.c
#					      ${PACKAGE_NAME}/fann/fann_error.c
#					      ${PACKAGE_NAME}/fann/fann_io.c
#					      ${PACKAGE_NAME}/fann/fann_train.c
#					      ${PACKAGE_NAME}/fann/fann_train_data.c
						)

TARGET_LINK_LIBRARIES(${PACKAGE_NAME} 
			${OPENCL_LIBRARY}
			openipDS
			openipSC
			openipIO)

INSTALL(TARGETS ${PACKAGE_NAME} 
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)

SET(CMAKE_VERBOSE_MAKEFILE on)
