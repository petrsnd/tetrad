INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}" )

ADD_EXECUTABLE( tetrad-cli
	ConsoleView.cpp
	stdafx.cpp
	stdafx.h
	targetver.h )

TARGET_LINK_LIBRARIES( tetrad-cli Engine )