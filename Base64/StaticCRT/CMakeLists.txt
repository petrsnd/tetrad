INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}" )

INCLUDE( "${PROJECT_SOURCE_DIR}/staticCRT.cmake" )

ADD_LIBRARY( Base64StaticCRT STATIC ${REL_LIBRARY_SRCS} )