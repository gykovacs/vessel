# FindOpenCL.cmake
# sets OPENCL_INCLUDE_DIR, OPENCL_LIBRARY, OPENCL_FOUND

# Finding header files
FIND_PATH(OPENCL_INCLUDE_DIR
  NAMES
    CL/cl.h OpenCL/cl.h
  PATHS
    $ENV{AMDAPPSDKROOT}/include
    $ENV{INTELOCLSDKROOT}/include
    $ENV{NVSDKCOMPUTE_ROOT}/OpenCL/common/inc
    $ENV{ATISTREAMSDKROOT}/include)

# Setting possible library paths
IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
  # 32bit architecture
  SET(OPENCL_LIB_SEARCH_PATH
    ${OPENCL_LIB_SEARCH_PATH}
    $ENV{AMDAPPSDKROOT}/lib/x86
    $ENV{INTELOCLSDKROOT}/lib/x86
    $ENV{NVSDKCOMPUTE_ROOT}/OpenCL/common/lib/Win32
    $ENV{ATISTREAMSDKROOT}/lib/x86)
ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
  # 64bit architecture
  SET(OPENCL_LIB_SEARCH_PATH
    ${OPENCL_LIB_SEARCH_PATH}
    $ENV{AMDAPPSDKROOT}/lib/x86_64
    $ENV{INTELOCLSDKROOT}/lib/x64
    $ENV{NVSDKCOMPUTE_ROOT}/OpenCL/common/lib/x64
    $ENV{ATISTREAMSDKROOT}/lib/x86_64)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)

# Finding libraries
FIND_LIBRARY(
    OPENCL_LIBRARY
    NAMES OpenCL
    PATHS ${OPENCL_LIB_SEARCH_PATH})

# Handling REQUIRED and QUIET args
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  OpenCL
  DEFAULT_MSG
  OPENCL_LIBRARY OPENCL_INCLUDE_DIR)

# Test application
IF (OPENCL_INCLUDE_DIR AND OPENCL_LIBRARY )
  SET (_OPENCL_VERSION_TEST_SOURCE
  " #if __APPLE__
    #include <OpenCL/cl.h>
    #else /* !__APPLE__ */
    #include <CL/cl.h>
    #endif /* __APPLE__ */

    #include <stdio.h>
    #include <stdlib.h>

    int main()
    {
      char *version;
      cl_int result;
      cl_platform_id id;
      size_t n;

      result = clGetPlatformIDs(1, &id, NULL);

      if (result == CL_SUCCESS) 
      {
	result = clGetPlatformInfo(id, CL_PLATFORM_VERSION, 0, NULL, &n);

        if (result == CL_SUCCESS) 
        {
	  version = (char*)malloc(n * sizeof(char));

	  result = clGetPlatformInfo(id, CL_PLATFORM_VERSION, n, version, NULL);

           if (result == CL_SUCCESS)
	    printf(\"%s\", version);

           free(version);
        }
      }

      return result == CL_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
    }")

  # Writing test application into file
  SET (_OPENCL_VERSION_SOURCE "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/openclversion.c")
  FILE (WRITE ${_OPENCL_VERSION_SOURCE} "${_OPENCL_VERSION_TEST_SOURCE}\n")

  # Compile and run test application
  TRY_RUN (_OPENCL_VERSION_RUN_RESULT _OPENCL_VERSION_COMPILE_RESULT
      ${CMAKE_BINARY_DIR} ${_OPENCL_VERSION_SOURCE}
      RUN_OUTPUT_VARIABLE _OPENCL_VERSION_STRING
      CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:STRING=${OPENCL_INCLUDE_DIRS}"
		  "-DLINK_LIBRARIES:STRING=${OPENCL_LIBRARY}")

  # Print status
  IF (_OPENCL_VERSION_RUN_RESULT EQUAL 0)
    MESSAGE(STATUS ${_OPENCL_VERSION_STRING})
  ELSEIF (_OPENCL_VERSION_RUN_RESULT EQUAL 0)
    MESSAGE(WARNING "Running test program failed.")
  ENDIF (_OPENCL_VERSION_RUN_RESULT EQUAL 0)

ENDIF (OPENCL_INCLUDE_DIR AND OPENCL_LIBRARY)

