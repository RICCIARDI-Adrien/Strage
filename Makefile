CPP = g++
CPPFLAGS = -W -Wall -std=c++11

PATH_INCLUDES = Includes
PATH_SOURCES = Sources

BINARY = Strage
# Windows specific
ifeq ($(OS),Windows_NT)
	# Avoid shipping MinGW libgcc and libstdc++
	CPPFLAGS += -static-libgcc -static-libstdc++ -mwindows
	# Windows needs custom libraries to provide WinMain()
	LIBRARIES = -lmingw32 -lSDL2main
endif
LIBRARIES += -lSDL2 -lSDL2_mixer -lSDL2_ttf
SOURCES = $(shell find $(PATH_SOURCES) -name "*.cpp")

# Program configuration
CPPFLAGS += -DCONFIGURATION_DISPLAY_WIDTH=1024
CPPFLAGS += -DCONFIGURATION_DISPLAY_HEIGHT=768
# For a 60Hz refresh frequency, the period is 1/60 = 0.016ms
CPPFLAGS += -DCONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS=16
CPPFLAGS += -DCONFIGURATION_DISPLAY_HUD_LIFE_POINTS_X=10
CPPFLAGS += -DCONFIGURATION_DISPLAY_HUD_LIFE_POINTS_Y=10
CPPFLAGS += -DCONFIGURATION_DISPLAY_HUD_AMMUNITION_X=250
CPPFLAGS += -DCONFIGURATION_DISPLAY_HUD_AMMUNITION_Y=10
CPPFLAGS += -DCONFIGURATION_DISPLAY_HUD_SPAWNERS_X=510
CPPFLAGS += -DCONFIGURATION_DISPLAY_HUD_SPAWNERS_Y=10
# Sampling frequency in Hz
CPPFLAGS += -DCONFIGURATION_AUDIO_SAMPLING_FREQUENCY=44100
# How many sounds can be played simultaneously
CPPFLAGS += -DCONFIGURATION_AUDIO_CHANNELS_COUNT=48
# Set which logs to display (error logs are always enabled) :
# 0 : error messages
# 1 : error and information messages
# 2 : error, information and debug messages
# 3 : all previous stuff plus in-game displaying of some meaningful collision rectangles
CPPFLAGS += -DCONFIGURATION_LOG_LEVEL=1
CPPFLAGS += -DCONFIGURATION_PATH_LEVELS=\"Levels\"
CPPFLAGS += -DCONFIGURATION_PATH_SOUNDS=\"Sounds\"
CPPFLAGS += -DCONFIGURATION_PATH_TEXTURES=\"Textures\"
CPPFLAGS += -DCONFIGURATION_LEVEL_MAXIMUM_WIDTH=256
CPPFLAGS += -DCONFIGURATION_LEVEL_MAXIMUM_HEIGHT=256
CPPFLAGS += -DCONFIGURATION_LEVEL_BLOCK_SIZE=64
# The probability (in range 0% to 100%) to spawn an item when an enemy dies
CPPFLAGS += -DCONFIGURATION_GAMEPLAY_MEDIPACK_ITEM_SPAWN_PROBABILITY_PERCENTAGE=10
CPPFLAGS += -DCONFIGURATION_GAMEPLAY_AMMUNITION_ITEM_SPAWN_PROBABILITY_PERCENTAGE=20
# The probability (in range 0% to 100%) to spawn a specific enemy (sum of all enemies percentages must be 100 and each percentage must be less than the previous one)
CPPFLAGS += -DCONFIGURATION_GAMEPLAY_SMALL_ENEMY_SPAWN_PROBABILITY_PERCENTAGE=80
CPPFLAGS += -DCONFIGURATION_GAMEPLAY_MEDIUM_ENEMY_SPAWN_PROBABILITY_PERCENTAGE=15
CPPFLAGS += -DCONFIGURATION_GAMEPLAY_BIG_ENEMY_SPAWN_PROBABILITY_PERCENTAGE=5

debug: CPPFLAGS += -g
debug: all

release: CPPFLAGS += -Werror -O2
release: all

all:
	$(CPP) $(CPPFLAGS) -I$(PATH_INCLUDES) $(SOURCES) $(LIBRARIES) -o $(BINARY)

clean:
	rm -f $(BINARY) $(BINARY).exe
