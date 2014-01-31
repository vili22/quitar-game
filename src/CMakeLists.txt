CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(QT_QMAKE_EXECUTABLE /usr/bin/qmake-qt4)
SET(QT_MIN_VERSION "4.8.0")

INCLUDE(FindOpenGL)

FIND_PATH(FREETYPE2_INCLUDE_DIR freetype/freetype.h
  ../include/freetype2
  /usr/local/include/freetype2
  /usr/include/freetype2
  )

#FIND_PACKAGE(SFML 2.1 REQUIRED audio)

ADD_LIBRARY(quitar-game
  Action.cpp
  chore.cpp
  GameView.cpp
  GLExtensionProxy.cpp
  Input.cpp
  Mesh.cpp
  MeshFactory.cpp
  midiEvent.cpp
  midiFileParser.cpp
  midiStream.cpp
  SoftwareElementDataBuffer.cpp
  SoftwareVertexDataBuffer.cpp
  Sound.cpp
  Reaction.cpp
  TextRenderer.cpp
  Texture2D.cpp
  VertexDataBufferManager.cpp
  VertexFormat.cpp
  WM.cpp	
)

IF (NOT WIN32)	
  SET (GLFW_EXTRA_LIBS Xxf86vm)
ENDIF (NOT WIN32)

TARGET_LINK_LIBRARIES(quitar-game
  glfw${GLFW_SUFFIX}
  ${OPENGL_LIBRARIES}
  freetype${FREETYPE_SUFFIX}
  ${GLFW_EXTRA_LIBS}
  /usr/local/lib/libsfml-audio.so.2
  /usr/local/lib/libsfml-graphics.so.2
  /usr/local/lib/libsfml-system.so.2
  /usr/local/lib/libsfml-window.so.2
)

IF (CMAKE_COMPILER_IS_GNUCXX)
  SET_TARGET_PROPERTIES(quitar-game PROPERTIES COMPILE_FLAGS "-O2 -fno-inline-functions -Wall -Wextra -Wno-unused-parameter")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIR})
LINK_DIRECTORIES(${BOOST_LIB_DIR})

INCLUDE(FindQt4)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

QT4_AUTOMOC(LauncherWindow.cpp)


LINK_LIBRARIES(${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} boost_filesystem${BOOST_SUFFIX} quitar-game)

ADD_EXECUTABLE(quitargame-launcher LauncherWindow.cpp SongIterator.cpp)
TARGET_LINK_LIBRARIES(quitargame-launcher ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} boost_system${BOOST_SUFFIX})

ADD_EXECUTABLE(quitargame silkystrings.cpp)
TARGET_LINK_LIBRARIES(quitargame quitar-game)




