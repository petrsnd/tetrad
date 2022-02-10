INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}" )

STRING( TOLOWER "$ENV{Platform}" generator )
IF( NOT generator STREQUAL "arm" )
	SET( ENGINETEST_SRCS
		TestRunner.cpp
		TestPlayerSerialize.cpp
		TestTileSerialize.cpp
		TestTileFlip.cpp
		TestGameSerialize.cpp
		TestGameFlip.cpp
		TestMoveSerialize.cpp
		TestMoveFlip.cpp )

	ADD_EXECUTABLE( EngineTests ${ENGINETEST_SRCS} )

	LINK_DIRECTORIES( "${PROJECT_SOURCE_DIR}/JsonSpirit" "${PROJECT_SOURCE_DIR}/Engine" )

	TARGET_LINK_LIBRARIES( EngineTests
		Base64
		JsonSpirit
		Engine
		${Boost_LIBRARIES} )

	ENABLE_TESTING()
	ADD_TEST( EngineTests ${CMAKE_CURRENT_BINARY_DIR}/EngineTests )
ENDIF()