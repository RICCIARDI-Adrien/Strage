PATH_INCLUDES = Includes
PATH_SOURCES = Sources

BINARY = Strage
CPPFLAGS = -W -Wall -std=c++11
SOURCES = $(shell find $(PATH_SOURCES) -name "*.cpp")

# Debug target is only available on Linux
debug: CPP = g++
debug: CPPFLAGS += -g
debug: LIBRARIES = -lSDL2 -lSDL2_mixer -lSDL2_ttf
debug: all

linux: CPP = g++
linux: CPPFLAGS += -Werror -O2
linux: LIBRARIES = -lSDL2 -lSDL2_mixer -lSDL2_ttf
linux: all

macos: CPP = g++
macos: CPPFLAGS += -Werror -O2 -DCONFIGURATION_BUILD_FOR_MACOS
macos: LIBRARIES = -framework SDL2 -framework SDL2_mixer -framework SDL2_ttf
macos: all

windows: BINARY = Strage.exe
windows: CPP = i686-w64-mingw32-g++
# Avoid shipping MinGW libgcc and libstdc++, build as a GUI program to avoid having an opened console when the game is started
windows: CPPFLAGS += -Werror -O2 -static-libgcc -static-libstdc++ -mwindows
# Windows needs custom libraries to provide WinMain() ; symbols are scattered into several libraries, so make sure they are all found
windows: LIBRARIES = -Wl,--start-group -lSDL2 -lSDL2_mixer -lSDL2_ttf -lmingw32 -lSDL2main -Wl,--end-group
windows: all

all:
	$(CPP) $(CPPFLAGS) -I$(PATH_INCLUDES) $(SOURCES) $(LIBRARIES) -o $(BINARY)

clean:
	rm -f $(BINARY) $(BINARY).exe
