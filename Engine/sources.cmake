SET( LIBRARY_SRCS
	GuidCreator.h
	Exceptions.h
	Definitions.h
	Interfaces.h
	JsonSerializable.h
	JsonSerializeUtil.h JsonSerializeUtil.cpp
	Move.h
	EnumIterators.cpp
	GameBoard.h GameBoard.cpp
	GameMechanicsUtil.h GameMechanicsUtil.cpp
	GameMechanics.h GameMechanics.cpp
	TetradFinder.h TetradFinder.cpp
	MoveGenerator.h MoveGenerator.cpp
	Player.h Player.cpp
	EventNotifier.h EventNotifier.cpp
	Game.h Game.cpp
	MutexObject.h MutexObject.cpp
	Engine.cpp
	Logger.h
	Logger.cpp )

IF( MSVC )
	SET( LIBRARY_SRCS
		${LIBRARY_SRCS}
		stdafx.h
		stdafx.cpp
		targetver.h )
ENDIF( MSVC )