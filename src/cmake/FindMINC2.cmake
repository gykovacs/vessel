MARK_AS_ADVANCED(NETCDF_INCLUDE_PATH NETCDF_LIB)
MARK_AS_ADVANCED(HDF5_INCLUDE_PATH HDF5_LIB)
MARK_AS_ADVANCED(MINC_INCLUDE_PATH)
MARK_AS_ADVANCED(MINC2_INCLUDE_PATH MINC2_LIB)

SET(NETCDF_INC_SEARCHPATH
   /usr/local/bic/include
)

SET(HDF5_INC_SEARCHPATH
  /usr/local/bic/include
)

SET(MINC_INC_SEARCHPATH
  /usr/local/bic/include
)

SET(MINC2_INC_SEARCHPATH
  /usr/local/bic/include
)

FIND_PATH(NETCDF_INCLUDE_PATH netcdf.h ${NETCDF_INC_SEARCHPATH})
FIND_PATH(HDF5_INCLUDE_PATH hdf5.h ${HDF5_INC_SEARCHPATH})
FIND_PATH(MINC_INCLUDE_PATH minc.h ${MINC_INC_SEARCHPATH})
FIND_PATH(MINC2_INCLUDE_PATH minc2.h ${MINC2_INC_SEARCHPATH})

IF(NETCDF_INCLUDE_PATH)
  SET(NETCDF_INCLUDE  ${NETCDF_INCLUDE_PATH})
ENDIF (NETCDF_INCLUDE_PATH)

IF(HDF5_INCLUDE_PATH)
  SET(HDF5_INCLUDE  ${HDF5_INCLUDE_PATH})
ENDIF (HDF5_INCLUDE_PATH)

IF(MINC_INCLUDE_PATH)
  SET(MINC_INCLUDE ${MINC_INCLUDE_PATH})
ENDIF (MINC_INCLUDE_PATH)


IF(MINC2_INCLUDE_PATH)
  SET(MINC2_INCLUDE ${MINC2_INCLUDE_PATH})
ENDIF (MINC2_INCLUDE_PATH)

FIND_LIBRARY(NETCDF_LIB netcdf /usr/lib /usr/local/lib )
FIND_LIBRARY(HDF5_LIB hdf5 /usr/lib /usr/local/lib )
FIND_LIBRARY(MINC2_LIB minc2 /usr/lib /usr/local/lib )


IF(NETCDF_LIB)
        GET_FILENAME_COMPONENT(NETCDF_LIBRARY_PATH ${NETCDF_LIB} PATH)
ENDIF(NETCDF_LIB)

IF(HDF5_LIB)
        GET_FILENAME_COMPONENT(HDF5_LIBRARY_PATH ${HDF5_LIB} PATH)
ENDIF(HDF5_LIB)

IF(MINC2_LIB)
        GET_FILENAME_COMPONENT(MINC2_LIBRARY_PATH ${MINC2_LIB} PATH)
ENDIF(MINC2_LIB)

SET(MINC2_FOUND 0)

IF(MINC_INCLUDE AND MINC2_INCLUDE AND HDF5_INCLUDE AND NETCDF_INCLUDE)
  IF(MINC2_LIBRARY_PATH AND HDF5_LIBRARY_PATH AND NETCDF_LIBRARY_PATH)
    SET(MINC2_FOUND 1)
  ENDIF (MINC2_LIBRARY_PATH AND HDF5_LIBRARY_PATH AND NETCDF_LIBRARY_PATH)
ENDIF(MINC_INCLUDE AND MINC2_INCLUDE AND HDF5_INCLUDE AND NETCDF_INCLUDE )

IF(MINC2_FOUND)
  INCLUDE_DIRECTORIES( ${NETCDF_INCLUDE} ${HDF5_INCLUDE} ${MINC_INCLUDE} ${MINC2_INCLUDE}  )
  LINK_DIRECTORIES( ${NETCDF_LIBRARY_PATH} ${HDF5_LIBRARY_PATH} ${MINC2_LIBRARY_PATH})
  ADD_DEFINITIONS(-DUSE_MINC2)
ENDIF (MINC2_FOUND)

