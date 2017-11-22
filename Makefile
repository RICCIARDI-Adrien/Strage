PATH_INCLUDES = Includes
PATH_SOURCES = Sources
PATH_MACOS_RELEASE = Strage.app
PATH_WINDOWS_RELEASE = Strage

VERSION_SDL2 = 2.0.7
VERSION_SDL2_MIXER = 2.0.2
VERSION_SDL2_TTF = 2.0.14

BINARY = Strage
CPPFLAGS = -W -Wall -std=c++11
SOURCES = $(shell find $(PATH_SOURCES) -name "*.cpp")

# Debug target is only available on Linux
debug: CPP = g++
debug: CPPFLAGS += -g
debug: LIBRARIES = -lSDL2 -lSDL2_mixer -lSDL2_ttf
debug: all

linux: CPP = g++
linux: CPPFLAGS += -Werror -O2 -DNDEBUG
linux: LIBRARIES = -lSDL2 -lSDL2_mixer -lSDL2_ttf
linux: all

macos: CPP = g++
macos: CPPFLAGS += -Werror -O2 -DNDEBUG -DCONFIGURATION_BUILD_FOR_MACOS
macos: LIBRARIES = -framework SDL2 -framework SDL2_mixer -framework SDL2_ttf
macos: all

windows: SDL2-$(VERSION_SDL2) SDL2_mixer-$(VERSION_SDL2_MIXER) SDL2_ttf-$(VERSION_SDL2_TTF)
windows: ADDITIONAL_OBJECTS = Windows_Icon.o
windows: BINARY = Strage.exe
windows: CPP = i686-w64-mingw32-g++
# Avoid shipping MinGW libgcc and libstdc++, build as a GUI program to avoid having an opened console when the game is started
windows: CPPFLAGS += -Werror -O2 -DNDEBUG -static-libgcc -static-libstdc++ -mwindows -ISDL2_Includes
# Windows needs custom libraries to provide WinMain() ; symbols are scattered into several libraries, so make sure they are all found
windows: LIBRARIES = -Wl,--start-group -LSDL2-$(VERSION_SDL2)/i686-w64-mingw32/lib -lSDL2 -LSDL2_mixer-$(VERSION_SDL2_MIXER)/i686-w64-mingw32/lib -lSDL2_mixer -LSDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/lib -lSDL2_ttf -lmingw32 -lSDL2main -Wl,--end-group
windows: windows_generate_executable_icon all windows_clean_executable_icon

# Windows SDL dependencies
# Each rule copies headers to a unique SDL2 include directory because SDL_mixer and SDL_ttf internally refer to SDL2.h as a local file (i.e. #include "SDL2.h"), so multiple directories specified with -I options don't work
SDL2-$(VERSION_SDL2):
	wget https://www.libsdl.org/release/SDL2-devel-$(VERSION_SDL2)-mingw.tar.gz -O /tmp/SDL2-devel-$(VERSION_SDL2)-mingw.tar.gz
	tar -xf /tmp/SDL2-devel-$(VERSION_SDL2)-mingw.tar.gz
	mkdir -p SDL2_Includes
	cp -r SDL2-$(VERSION_SDL2)/i686-w64-mingw32/include/* SDL2_Includes

SDL2_mixer-$(VERSION_SDL2_MIXER):
	wget https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-$(VERSION_SDL2_MIXER)-mingw.tar.gz -O /tmp/SDL2_mixer-devel-$(VERSION_SDL2_MIXER)-mingw.tar.gz
	tar -xf /tmp/SDL2_mixer-devel-$(VERSION_SDL2_MIXER)-mingw.tar.gz
	mkdir -p SDL2_Includes
	cp -r SDL2_mixer-$(VERSION_SDL2_MIXER)/i686-w64-mingw32/include/* SDL2_Includes

SDL2_ttf-$(VERSION_SDL2_TTF):
	wget https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-$(VERSION_SDL2_TTF)-mingw.tar.gz -O /tmp/SDL2_ttf-devel-$(VERSION_SDL2_TTF)-mingw.tar.gz
	tar -xf /tmp/SDL2_ttf-devel-$(VERSION_SDL2_TTF)-mingw.tar.gz
	mkdir -p SDL2_Includes
	cp -r SDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/include/* SDL2_Includes

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

# Make all frameworks being searched in the application Frameworks directory
macos_release: CPPFLAGS += -rpath @executable_path/../Frameworks
macos_release: macos
	@# Create application directories
	rm -rf $(PATH_MACOS_RELEASE)
	mkdir -p $(PATH_MACOS_RELEASE)/Contents/Frameworks
	mkdir -p $(PATH_MACOS_RELEASE)/Contents/MacOS
	mkdir -p $(PATH_MACOS_RELEASE)/Contents/Resources

	@# Copy needed frameworks
	cp -r /Library/Frameworks/SDL2.framework $(PATH_MACOS_RELEASE)/Contents/Frameworks
	cp -r /Library/Frameworks/SDL2_mixer.framework $(PATH_MACOS_RELEASE)/Contents/Frameworks
	cp -r /Library/Frameworks/SDL2_ttf.framework $(PATH_MACOS_RELEASE)/Contents/Frameworks

	@# Copy binary
	cp Strage $(PATH_MACOS_RELEASE)/Contents/MacOS

	@# Copy game resource files
	cp -r Levels $(PATH_MACOS_RELEASE)/Contents/Resources
	cp -r Sounds $(PATH_MACOS_RELEASE)/Contents/Resources
	cp -r Textures $(PATH_MACOS_RELEASE)/Contents/Resources
	cp Liberation_Sans_Bold.ttf $(PATH_MACOS_RELEASE)/Contents/Resources
	cp README.md $(PATH_MACOS_RELEASE)/Contents/Resources
	
	@# Copy app icon
	cp Development_Resources/MacOS_Icon.icns $(PATH_MACOS_RELEASE)/Contents/Resources/Icon.icns
	
	@# Create Info.plist file
	echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<plist version=\"1.0\">" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<dict>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<key>CFBundleName</key>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<string>Strage</string>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<key>CFBundlePackageType</key>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<string>APPL</string>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<key>CFBundleSignature</key>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<string>????</string>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<key>CFBundleExecutable</key>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<string>Strage</string>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<key>LSMinimumSystemVersion</key>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<string>10.5.0</string>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<key>CFBundleIconFile</key>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "<string>Icon.icns</string>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "</dict>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	echo "</plist>" >> $(PATH_MACOS_RELEASE)/Contents/Info.plist
	
	@# Create a compressed archive
	zip -r ../Strage_MacOS.zip $(PATH_MACOS_RELEASE)
	
	@# Remove now useless release directory
	rm -r $(PATH_MACOS_RELEASE)

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

	@# Add needed DLLs
	cp SDL2-$(VERSION_SDL2)/i686-w64-mingw32/bin/SDL2.dll $(PATH_WINDOWS_RELEASE)
	cp SDL2_mixer-$(VERSION_SDL2_MIXER)/i686-w64-mingw32/bin/libmpg123-0.dll $(PATH_WINDOWS_RELEASE)
	cp SDL2_mixer-$(VERSION_SDL2_MIXER)/i686-w64-mingw32/bin/SDL2_mixer.dll $(PATH_WINDOWS_RELEASE)
	cp SDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/bin/libfreetype-6.dll $(PATH_WINDOWS_RELEASE)
	cp SDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/bin/SDL2_ttf.dll $(PATH_WINDOWS_RELEASE)
	cp SDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/bin/zlib1.dll $(PATH_WINDOWS_RELEASE)
	
	@# Create a compressed archive
	zip -r ../Strage_Windows.zip $(PATH_WINDOWS_RELEASE)
	
	@# Remove now useless release directory
	rm -r $(PATH_WINDOWS_RELEASE)
