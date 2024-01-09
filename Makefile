PATH_INCLUDES = Includes
PATH_SOURCES = Sources
PATH_LINUX_RELEASE = /tmp/Strage
PATH_MACOS_RELEASE = Strage.app
PATH_WINDOWS_RELEASE = Strage

VERSION_STRAGE = 0.14
VERSION_SDL2 = 2.0.14
VERSION_SDL2_IMAGE = 2.0.5
VERSION_SDL2_MIXER = 2.0.4
VERSION_SDL2_TTF = 2.0.15

BINARY = Strage
CPPFLAGS = -W -Wall -std=c++11 -DCONFIGURATION_VERSION=\"$(VERSION_STRAGE)\"
SOURCES = $(wildcard $(PATH_SOURCES)/*.cpp)

# Copy all relevant game files to the directory specified as first argument
define CopyGameDataFiles
	@# Copy directories
	cp -r Levels $(1)
	cp -r Sounds $(1)
	cp -r Textures $(1)

	@# Copy files
	cp Liberation_Sans_Bold.ttf $(1)
	cp README.md $(1)
endef

# Debug target is only available on Linux
debug: CPP = g++
debug: CPPFLAGS += -g
debug: LIBRARIES = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
debug: all

linux: CPP = g++
linux: CPPFLAGS += -Werror -O2 -DNDEBUG
linux: LIBRARIES = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
linux: all

macos: CPP = g++
# Make all frameworks being searched in the application Frameworks directory
macos: CPPFLAGS += -Werror -O2 -DNDEBUG -DCONFIGURATION_BUILD_FOR_MACOS -F Frameworks -rpath @executable_path/../Frameworks -rpath @executable_path/Frameworks
macos: LIBRARIES = -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf
macos: Frameworks/SDL2-$(VERSION_SDL2).framework Frameworks/SDL2_image-$(VERSION_SDL2_IMAGE).framework Frameworks/SDL2_mixer-$(VERSION_SDL2_MIXER).framework Frameworks/SDL2_ttf-$(VERSION_SDL2_TTF).framework all

# MacOS SDL dependencies
Frameworks/SDL2-$(VERSION_SDL2).framework:
	curl https://www.libsdl.org/release/SDL2-$(VERSION_SDL2).dmg -o /tmp/SDL2-$(VERSION_SDL2).dmg
	hdiutil attach /tmp/SDL2-$(VERSION_SDL2).dmg
	mkdir -p Frameworks
	@# Explicitly adding the SDL version to the framework makes the makefile rule automatically download an unpack the new version when the SDL version variable is updated (it would not have been the case if the directory name was not dependent of a version), so SDL frameworks are always up-to-date even without doing a "make clean" when version changed
	cp -r /Volumes/SDL2/SDL2.framework Frameworks/SDL2-$(VERSION_SDL2).framework
	@# However, framework must be named the same as in the DMG image, so use a symlink to fake the name
	ln -sf SDL2-$(VERSION_SDL2).framework Frameworks/SDL2.framework
	hdiutil eject /Volumes/SDL2

Frameworks/SDL2_image-$(VERSION_SDL2_IMAGE).framework:
	curl https://www.libsdl.org/projects/SDL_image/release/SDL2_image-$(VERSION_SDL2_IMAGE).dmg -o /tmp/SDL2_image-$(VERSION_SDL2_IMAGE).dmg
	hdiutil attach /tmp/SDL2_image-$(VERSION_SDL2_IMAGE).dmg
	cp -r /Volumes/SDL2_image/SDL2_image.framework Frameworks/SDL2_image-$(VERSION_SDL2_IMAGE).framework
	ln -sf SDL2_image-$(VERSION_SDL2_IMAGE).framework Frameworks/SDL2_image.framework
	hdiutil eject /Volumes/SDL2_image

Frameworks/SDL2_mixer-$(VERSION_SDL2_MIXER).framework:
	curl https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-$(VERSION_SDL2_MIXER).dmg -o /tmp/SDL2_mixer-$(VERSION_SDL2_MIXER).dmg
	hdiutil attach /tmp/SDL2_mixer-$(VERSION_SDL2_MIXER).dmg
	cp -r /Volumes/SDL2_mixer/SDL2_mixer.framework Frameworks/SDL2_mixer-$(VERSION_SDL2_MIXER).framework
	ln -sf SDL2_mixer-$(VERSION_SDL2_MIXER).framework Frameworks/SDL2_mixer.framework
	hdiutil eject /Volumes/SDL2_mixer

Frameworks/SDL2_ttf-$(VERSION_SDL2_TTF).framework:
	curl https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-$(VERSION_SDL2_TTF).dmg -o /tmp/SDL2_ttf-$(VERSION_SDL2_TTF).dmg
	hdiutil attach /tmp/SDL2_ttf-$(VERSION_SDL2_TTF).dmg
	cp -r /Volumes/SDL2_ttf/SDL2_ttf.framework Frameworks/SDL2_ttf-$(VERSION_SDL2_TTF).framework
	ln -sf SDL2_ttf-$(VERSION_SDL2_TTF).framework Frameworks/SDL2_ttf.framework
	hdiutil eject /Volumes/SDL2_ttf

windows: ADDITIONAL_OBJECTS = Windows_Icon.o
windows: BINARY = Strage.exe
windows: CPP = i686-w64-mingw32-g++
# Avoid shipping MinGW libgcc and libstdc++, build as a GUI program to avoid having an opened console when the game is started
windows: CPPFLAGS += -Werror -O2 -DNDEBUG -static-libgcc -static-libstdc++ -mwindows -ISDL2_Includes
# Windows needs custom libraries to provide WinMain() ; symbols are scattered into several libraries, so make sure they are all found
windows: LIBRARIES = -Wl,--start-group -LSDL2-$(VERSION_SDL2)/i686-w64-mingw32/lib -lSDL2 -LSDL2_image-$(VERSION_SDL2_IMAGE)/i686-w64-mingw32/lib -lSDL2_image -LSDL2_mixer-$(VERSION_SDL2_MIXER)/i686-w64-mingw32/lib -lSDL2_mixer -LSDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/lib -lSDL2_ttf -lmingw32 -lSDL2main -Wl,--end-group
windows: SDL2-$(VERSION_SDL2) SDL2_image-$(VERSION_SDL2_IMAGE) SDL2_mixer-$(VERSION_SDL2_MIXER) SDL2_ttf-$(VERSION_SDL2_TTF) windows_generate_executable_icon all windows_clean_executable_icon

# Windows SDL dependencies
# Each rule copies headers to a unique SDL2 include directory because SDL_mixer and SDL_ttf internally refer to SDL2.h as a local file (i.e. #include "SDL2.h"), so multiple directories specified with -I options don't work
# Each rule also places the DLLs needed by the game executable near to the executable to make it easily testable using wine
SDL2-$(VERSION_SDL2):
	wget https://www.libsdl.org/release/SDL2-devel-$(VERSION_SDL2)-mingw.tar.gz -O /tmp/SDL2-devel-$(VERSION_SDL2)-mingw.tar.gz
	tar -xf /tmp/SDL2-devel-$(VERSION_SDL2)-mingw.tar.gz
	mkdir -p SDL2_Includes
	cp -r SDL2-$(VERSION_SDL2)/i686-w64-mingw32/include/* SDL2_Includes
	cp SDL2-$(VERSION_SDL2)/i686-w64-mingw32/bin/SDL2.dll .

SDL2_image-$(VERSION_SDL2_IMAGE):
	wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-$(VERSION_SDL2_IMAGE)-mingw.tar.gz -O /tmp/SDL2_image-devel-$(VERSION_SDL2_IMAGE)-mingw.tar.gz
	tar -xf /tmp/SDL2_image-devel-$(VERSION_SDL2_IMAGE)-mingw.tar.gz
	cp -r SDL2_image-$(VERSION_SDL2_IMAGE)/i686-w64-mingw32/include/* SDL2_Includes
	cp SDL2_image-$(VERSION_SDL2_IMAGE)/i686-w64-mingw32/bin/libpng16-16.dll .
	cp SDL2_image-$(VERSION_SDL2_IMAGE)/i686-w64-mingw32/bin/SDL2_image.dll .

SDL2_mixer-$(VERSION_SDL2_MIXER):
	wget https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-$(VERSION_SDL2_MIXER)-mingw.tar.gz -O /tmp/SDL2_mixer-devel-$(VERSION_SDL2_MIXER)-mingw.tar.gz
	tar -xf /tmp/SDL2_mixer-devel-$(VERSION_SDL2_MIXER)-mingw.tar.gz
	cp -r SDL2_mixer-$(VERSION_SDL2_MIXER)/i686-w64-mingw32/include/* SDL2_Includes
	cp SDL2_mixer-$(VERSION_SDL2_MIXER)/i686-w64-mingw32/bin/libmpg123-0.dll .
	cp SDL2_mixer-$(VERSION_SDL2_MIXER)/i686-w64-mingw32/bin/SDL2_mixer.dll .

SDL2_ttf-$(VERSION_SDL2_TTF):
	wget https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-$(VERSION_SDL2_TTF)-mingw.tar.gz -O /tmp/SDL2_ttf-devel-$(VERSION_SDL2_TTF)-mingw.tar.gz
	tar -xf /tmp/SDL2_ttf-devel-$(VERSION_SDL2_TTF)-mingw.tar.gz
	cp -r SDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/include/* SDL2_Includes
	cp SDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/bin/libfreetype-6.dll .
	cp SDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/bin/SDL2_ttf.dll .
	cp SDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/bin/zlib1.dll .

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
	@# Remove macOS build files
	rm -rf Frameworks
	@# Remove Windows build files
	rm -rf SDL2-$(VERSION_SDL2) SDL2_image-$(VERSION_SDL2_IMAGE) SDL2_mixer-$(VERSION_SDL2_MIXER) SDL2_ttf-$(VERSION_SDL2_TTF) SDL2_Includes *.dll

linux_release: linux
	@# Create application directories
	rm -rf $(PATH_LINUX_RELEASE)
	mkdir $(PATH_LINUX_RELEASE)
	
	@# Copy game data
	$(call CopyGameDataFiles,$(PATH_LINUX_RELEASE))
	@# Copy binary
	cp Strage $(PATH_LINUX_RELEASE)
	
	@# Create a compressed archive
	tar -C $(dir $(PATH_LINUX_RELEASE)) -c $(notdir $(PATH_LINUX_RELEASE))/ -J -f ../Strage_Version_$(VERSION_STRAGE)_Linux.tar.xz
	
	@# Remove now useless release directory
	rm -r $(PATH_LINUX_RELEASE)

macos_release: macos
	@# Create application directories
	rm -rf $(PATH_MACOS_RELEASE)
	mkdir -p $(PATH_MACOS_RELEASE)/Contents/Frameworks
	mkdir -p $(PATH_MACOS_RELEASE)/Contents/MacOS
	mkdir -p $(PATH_MACOS_RELEASE)/Contents/Resources

	@# Copy needed frameworks
	cp -r Frameworks/SDL2.framework $(PATH_MACOS_RELEASE)/Contents/Frameworks
	cp -r Frameworks/SDL2_image.framework $(PATH_MACOS_RELEASE)/Contents/Frameworks
	cp -r Frameworks/SDL2_mixer.framework $(PATH_MACOS_RELEASE)/Contents/Frameworks
	cp -r Frameworks/SDL2_ttf.framework $(PATH_MACOS_RELEASE)/Contents/Frameworks

	@# Copy game data
	$(call CopyGameDataFiles,$(PATH_MACOS_RELEASE)/Contents/Resources)
	@# Copy binary
	cp Strage $(PATH_MACOS_RELEASE)/Contents/MacOS
	
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
	zip -r ../Strage_Version_$(VERSION_STRAGE)_MacOS.zip $(PATH_MACOS_RELEASE)
	
	@# Remove now useless release directory
	rm -r $(PATH_MACOS_RELEASE)

windows_release: windows
	@# Create release root directory
	rm -rf $(PATH_WINDOWS_RELEASE)
	mkdir $(PATH_WINDOWS_RELEASE)

	@# Copy game data
	$(call CopyGameDataFiles,$(PATH_WINDOWS_RELEASE))
	@# Copy binary
	cp Strage.exe $(PATH_WINDOWS_RELEASE)

	@# Add needed DLLs
	cp SDL2-$(VERSION_SDL2)/i686-w64-mingw32/bin/SDL2.dll $(PATH_WINDOWS_RELEASE)
	cp SDL2_image-$(VERSION_SDL2_IMAGE)/i686-w64-mingw32/bin/libpng16-16.dll $(PATH_WINDOWS_RELEASE)
	cp SDL2_image-$(VERSION_SDL2_IMAGE)/i686-w64-mingw32/bin/SDL2_image.dll $(PATH_WINDOWS_RELEASE)
	@# Use SDL image ZLIB DLL instead of TTF's one because SDL image's one is more complete and contains SDL image mandatory functions which are not present in TTF DLL version
	cp SDL2_image-$(VERSION_SDL2_IMAGE)/i686-w64-mingw32/bin/zlib1.dll $(PATH_WINDOWS_RELEASE)
	cp SDL2_mixer-$(VERSION_SDL2_MIXER)/i686-w64-mingw32/bin/libmpg123-0.dll $(PATH_WINDOWS_RELEASE)
	cp SDL2_mixer-$(VERSION_SDL2_MIXER)/i686-w64-mingw32/bin/SDL2_mixer.dll $(PATH_WINDOWS_RELEASE)
	cp SDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/bin/libfreetype-6.dll $(PATH_WINDOWS_RELEASE)
	cp SDL2_ttf-$(VERSION_SDL2_TTF)/i686-w64-mingw32/bin/SDL2_ttf.dll $(PATH_WINDOWS_RELEASE)
	
	@# Create a compressed archive
	zip -r ../Strage_Version_$(VERSION_STRAGE)_Windows.zip $(PATH_WINDOWS_RELEASE)
	
	@# Remove now useless release directory
	rm -r $(PATH_WINDOWS_RELEASE)

check_memory_leak:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BINARY)

check_processor_usage:
	valgrind --tool=callgrind ./$(BINARY)
