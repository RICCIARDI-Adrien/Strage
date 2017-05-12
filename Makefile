PATH_INCLUDES = Includes
PATH_SOURCES = Sources
PATH_WINDOWS_RELEASE = ../Strage_Windows_Release

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

windows: ADDITIONAL_OBJECTS = Windows_Icon.o
windows: BINARY = Strage.exe
windows: CPP = i686-w64-mingw32-g++
# Avoid shipping MinGW libgcc and libstdc++, build as a GUI program to avoid having an opened console when the game is started
windows: CPPFLAGS += -Werror -O2 -static-libgcc -static-libstdc++ -mwindows
# Windows needs custom libraries to provide WinMain() ; symbols are scattered into several libraries, so make sure they are all found
windows: LIBRARIES = -Wl,--start-group -lSDL2 -lSDL2_mixer -lSDL2_ttf -lmingw32 -lSDL2main -Wl,--end-group
windows: windows_generate_executable_icon all windows_clean_executable_icon

windows_generate_executable_icon:
	@# Create resource file (.rc)
	echo "id ICON \"Development_Resources/Windows_Icon.ico\"" > Icon.rc
	
	@# Embed the icon to a special file
	i686-w64-mingw32-windres Icon.rc Windows_Icon.o

windows_clean_executable_icon:
	rm Icon.rc Windows_Icon.o

all:
	$(CPP) $(CPPFLAGS) -I$(PATH_INCLUDES) $(SOURCES) $(ADDITIONAL_OBJECTS) $(LIBRARIES) -o $(BINARY)

clean:
	rm -f $(BINARY) $(BINARY).exe

windows_release: windows
	@# Create release root directory
	rm -rf $(PATH_WINDOWS_RELEASE)
	mkdir $(PATH_WINDOWS_RELEASE)

	@# Copy relevant directories
	cp -r Levels $(PATH_WINDOWS_RELEASE)
	cp -r Sounds $(PATH_WINDOWS_RELEASE)
	cp -r Textures $(PATH_WINDOWS_RELEASE)

	@# Copy relevant files
	cp Liberation_Sans_Bold.ttf $(PATH_WINDOWS_RELEASE)
	cp README.md $(PATH_WINDOWS_RELEASE)
	cp Strage.exe $(PATH_WINDOWS_RELEASE)
	
	@# Download needed SDL prebuit packages
	wget https://www.libsdl.org/release/SDL2-devel-2.0.5-mingw.tar.gz -O /tmp/SDL2-devel-2.0.5-mingw.tar.gz
	wget https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.1-mingw.tar.gz -O /tmp/SDL2_mixer-devel-2.0.1-mingw.tar.gz
	wget https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.14-mingw.tar.gz -O /tmp/SDL2_ttf-devel-2.0.14-mingw.tar.gz
	
	@# Uncompress SDL packages
	tar -xf /tmp/SDL2-devel-2.0.5-mingw.tar.gz -C /tmp
	tar -xf /tmp/SDL2_mixer-devel-2.0.1-mingw.tar.gz -C /tmp
	tar -xf /tmp/SDL2_ttf-devel-2.0.14-mingw.tar.gz -C /tmp

	@# Add needed DLLs
	cp /tmp/SDL2-2.0.5/i686-w64-mingw32/bin/SDL2.dll $(PATH_WINDOWS_RELEASE)
	cp /tmp/SDL2_mixer-2.0.1/i686-w64-mingw32/bin/SDL2_mixer.dll $(PATH_WINDOWS_RELEASE)
	cp /tmp/SDL2_mixer-2.0.1/i686-w64-mingw32/bin/smpeg2.dll $(PATH_WINDOWS_RELEASE)
	cp /tmp/SDL2_ttf-2.0.14/i686-w64-mingw32/bin/libfreetype-6.dll $(PATH_WINDOWS_RELEASE)
	cp /tmp/SDL2_ttf-2.0.14/i686-w64-mingw32/bin/SDL2_ttf.dll $(PATH_WINDOWS_RELEASE)
	cp /tmp/SDL2_ttf-2.0.14/i686-w64-mingw32/bin/zlib1.dll $(PATH_WINDOWS_RELEASE)
