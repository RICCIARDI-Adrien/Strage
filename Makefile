PATH_INCLUDES = Includes
PATH_SOURCES = Sources

CPPFLAGS = -W -Wall -std=c++11
LIBRARIES = -lSDL2 -lSDL2_mixer -lSDL2_ttf -pthread
SOURCES = $(shell find $(PATH_SOURCES) -name "*.cpp")

debug: BINARY = Strage
debug: CPP = g++
debug: CPPFLAGS += -g
debug: all

linux: BINARY = Strage
linux: CPP = g++
linux: CPPFLAGS += -Werror -O2
linux: all

windows: BINARY = Strage.exe
windows: CPP = i686-w64-mingw32-g++
# Avoid shipping MinGW libgcc and libstdc++, build as a GUI program to avoid having an opened console when the game is started
windows: CPPFLAGS += -Werror -O2 -DBUILD_FOR_WINDOWS -static-libgcc -static-libstdc++ -mwindows
# Windows needs custom libraries to provide WinMain()
windows: LIBRARIES += -lmingw32 -lSDL2main
windows: all

all:
	$(CPP) $(CPPFLAGS) -I$(PATH_INCLUDES) $(SOURCES) -Wl,--start-group $(LIBRARIES) -Wl,--end-group -o $(BINARY)

clean:
	rm -f $(BINARY) $(BINARY).exe
