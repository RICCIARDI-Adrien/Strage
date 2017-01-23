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
LIBRARIES += -lSDL2 -lSDL2_mixer -lSDL2_ttf -pthread
SOURCES = $(shell find $(PATH_SOURCES) -name "*.cpp")

debug: CPPFLAGS += -g
debug: all

release: CPPFLAGS += -Werror -O2
release: all

all:
	$(CPP) $(CPPFLAGS) -I$(PATH_INCLUDES) $(SOURCES) $(LIBRARIES) -o $(BINARY)

clean:
	rm -f $(BINARY) $(BINARY).exe
