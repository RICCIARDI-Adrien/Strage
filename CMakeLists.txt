cmake_minimum_required(VERSION 2.6)

project(Strage)

set(SOURCES
	Sources/AudioManager.cpp
	Sources/LevelManager.cpp
	Sources/Main.cpp
	Sources/Renderer.cpp
	Sources/TextureManager.cpp
)

add_executable(Strage ${SOURCES})
include_directories(Includes)
target_link_libraries(Strage SDL2 SDL2_mixer SDL2_ttf)

if (UNIX)
	# g++ specific flags
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=c++11 -O2")
	target_link_libraries(Strage pthread)
elseif (WIN32)
	# Windows needs a specific SDL library to provide WinMain()
	target_link_libraries(Strage SDL2main)
	# Remove Visual Studio ZERO_CHECK target (used to detect modified CMake files) because the game is not developed on Windows
	set(CMAKE_SUPPRESS_REGENERATION true)
endif (UNIX)