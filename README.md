# Strage

## How to play
You are in command of a little futuristic tank in an unknown part of the galaxy. What are you doing here ? No one knows, even you...  
Your ship can **move** in up, down, left and right directions using the **keyboard arrow keys**.  You can also **shoot** using **space bar**. Shoot ? Yes, enemies are nearby.  
You do not know why they want you dead, they may have a very good reason, but it's the worse idea they ever had. Defend yourself and escape from this place !  
  
Enemies are teleported to this place to fight you by using big machines. Destroy these machines at sight or they will teleport tons of enemies !  
You have to make your way alive to the level exit. Medipacks and ammunition are hidden somewhere on the map to help you. They can also be dropped by dead enemies, if you are lucky.  
Some levels contain a golden medipack that increases player maximum life amount, will you find them all ?  
  
And remember, *strage* means *slaughter* in Italian...

## Controls
* **Keyboard arrow keys** : move
* **Space bar** : shoot
* **Escape** : pause game
* **Alt+F4** : quit game
* **R** : reload current level

## Building
### Building for Linux
On Debian/Ubuntu systems, you have to install some SDL2 packages.
```
sudo apt install libsdl2-dev libsdl2-mixer-dev libsdl2-ttf-dev
```
Then simply type *make linux* in the Strage directory to build the game.

### Building for macOS
Install the following SDL libraries and headers by dragging/dropping each framework file in /Library/Frameworks :
* [SDL2](https://www.libsdl.org/release/SDL2-2.0.5.dmg)
* [SDL2 Mixer](https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.1.dmg)
* [SDL2 TTF](https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.dmg)

Then, build the executable using the command *make macos*.

### Building for Windows
Windows binary is built under Linux using MinGW W64 cross compiler.  
On Debian/Ubuntu systems, install the following package :
```
sudo apt install mingw-w64
```
Now, manually install needed SDL2 libraries and headers. First, download the following MinGW prebuilt packages :
* [SDL2](https://www.libsdl.org/release/SDL2-devel-2.0.5-mingw.tar.gz)
* [SDL2 Mixer](https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.1-mingw.tar.gz)
* [SDL2 TTF](https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.14-mingw.tar.gz)

Uncompress the three archives and copy each *i686-w64-mingw32/include* directory content to */usr/i686-w64-mingw32/include*.  
Copy also all *i686-w64-mingw32/lib* directories content to */usr/i686-w64-mingw32/lib*.

Finally, build the executable with the command *make windows*.

The following DLLs are required for the game to run on Windows (simply put them next to Strage.exe executable). DLLs are found in the *i686-w64-mingw32/bin* directory of the previously downloaded SDL archives, the archive corresponding to a DLL is specified in brackets.
* SDL2.dll (SDL2)
* SDL2_mixer.dll (SDL2_mixer)
* smpeg2.dll (SDL2_mixer)
* libfreetype-6.dll (SDL2_ttf)
* SDL2_ttf.dll (SDL2_ttf)
* zlib1.dll (SDL2_ttf)

## How it works
Levels are made using [Tiled](http://www.mapeditor.org/) editor. They are constituted of 2 layers :
* The *scene* layer containing the textures for the walls and floors.
* The *objects* layer telling where player, spawners and other interactive objects are located on the map.

Scene textures and objects are gathered in two Tiled tilesets (Scene.tsx and Objects.tsx) located in the Tiled_Resources directory.

## Credits
* Musics : [jamendo](https://www.jamendo.com)
   * Akashic Records - Epic Action Hero
   * Art Music - Epic Trailer
   * Blue Giraffe - Action Intense Cinematic
   * Celestial Aeon Project - Epic
   * E. Erkut - Dark Moment - Dark Epic Trailer
   * Matti Paalanen - Emotion
   * Matti Paalanen - Epic Action
   * Soundbay - Epic Future
   * Soundshrim - Epic Adventure
* Sounds : [universal-soundbank.com](http://www.universal-soundbank.com), [soundbible.com](http://soundbible.com)
* Textures : [textures.com](http://www.textures.com), [Minecraft EnfiCraft texture pack](http://www.minecrafttexturepacks.com/enficraft)
