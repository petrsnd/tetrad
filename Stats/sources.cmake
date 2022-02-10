SET( LIBRARY_SRCS
	GameData.h
	GameData.cpp
	StatsCalculator.h
	StatsCalculator.cpp )

IF( MSVC )
	SET( LIBRARY_SRCS
		${LIBRARY_SRCS}
		stdafx.h
		stdafx.cpp
		targetver.h )
ENDIF( MSVC )