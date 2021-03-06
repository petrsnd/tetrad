IF( MSVC )
    # Statically link to Visual C++ CRT
    FOREACH( Flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO )
        IF( ${Flag_var} MATCHES "/MD" )
            STRING( REGEX REPLACE "/MD" "/MT" ${Flag_var} "${${Flag_var}}" )
        ENDIF( ${Flag_var} MATCHES "/MD" )
        IF( ${Flag_var} MATCHES "/MDd" )
            STRING( REGEX REPLACE "/MDd" "/MTd" ${Flag_var} "${${Flag_var}}" )
        ENDIF( ${Flag_var} MATCHES "/MDd" )
    ENDFOREACH( Flag_var )
ENDIF( MSVC )

GET_FILENAME_COMPONENT( PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} PATH )

INCLUDE( "${PARENT_DIR}/sources.cmake" )

SET( REL_LIBRARY_SRCS "" )
FOREACH( Src_file ${LIBRARY_SRCS} )
    SET( REL_LIBRARY_SRCS ${REL_LIBRARY_SRCS} "${PARENT_DIR}/${Src_file}" )
ENDFOREACH( Src_file )