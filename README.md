![](https://github.com/RICCIARDI-Adrien/Strage/workflows/Release%20builds/badge.svg)

# Strage

You can now download the game from [itch.io](https://ricciardi-adrien.itch.io/strage).

## Gameplay preview
* [Strage version 0.2](https://www.youtube.com/watch?v=aTI6zjJu4h4)
* [Strage version 0.3](https://www.youtube.com/watch?v=r5JJvJWiGF0)
* [Strage version 0.8](https://www.youtube.com/watch?v=GxyV17knTqo)
* [Strage version 0.9](https://www.youtube.com/watch?v=CBbp0kRIsus)

## How to play
You are in command of a little futuristic tank in an unknown part of the galaxy. What are you doing here ? No one knows, even you...  
Your tank can **move** in up, down, left and right directions using the **keyboard arrow keys**.  You can also **shoot** using **space bar**. Shoot ? Yes, enemies are nearby.  
You do not know why they want you dead, they may have a very good reason, but it's the worse idea they ever had. Defend yourself and escape from this place !  
  
Enemies are teleported to this place to fight you by using big machines. Destroy these machines at sight or they will teleport tons of enemies !  
You have to make your way alive to the level exit. Medipacks and ammunition are hidden somewhere on the map to help you. They can also be dropped by dead enemies, if you are lucky.  
Some levels contain a golden medipack that increases player maximum life amount, will you find them all ?  
  
And remember, *strage* means *slaughter* in Italian...

## Controls
### Keyboard
* **Arrow keys** or **WASD** : move
* **Space bar** : primary shoot
* **Left control** : secondary shoot
* **Escape** : pause game
* **Alt+F4** : quit game

### Xbox 360 game controller
* **Left stick** : move
* **'A' button** : primary shoot
* **'B' button** : secondary shoot
* **Start button** : pause game

### Xbox One game controller
* **Left stick** : move
* **'A' button** : primary shoot
* **'B' button** : secondary shoot
* **Menu button** : pause game

## Building
### Building for Linux
On Debian/Ubuntu systems, you have to install some SDL2 packages.
```
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```
Then simply type the following command in the Strage directory to build the game :
```
make linux
```

You can package the game into an archive thanks to the following command :
```
make linux_release
```

### Building for macOS
Build the executable using the command :
```
make macos
```

You can package the game into a standard macOS app thanks to the following command :
```
make macos_release
```

### Building for Windows
Windows binary is built under Linux using MinGW W64 cross compiler.  
On Debian/Ubuntu systems, install the following package :
```
sudo apt install mingw-w64
```
Now build the executable with the command :
```
make windows
```

You can also package the game into a ZIP archive to easily distribute it by typing the following command :
```
make windows_release
```

## How it works
Levels are made using [Tiled](http://www.mapeditor.org/) editor. They are constituted of 2 layers :
* The *scene* layer containing the textures for the walls and floors.
* The *objects* layer telling where player, spawners and other interactive objects are located on the map.

Scene textures and objects are gathered in two Tiled tilesets (Scene.tsx and Objects.tsx) located in the Tiled_Resources directory.

When exporting a level, let 20 blocks on each map wall side to avoid displaying a black background when the player is approaching the map walls.

## Credits
* Icons :
   * [Bulletproof vest](https://www.flaticon.com/free-icon/bulletproof-vest_238523) : icon made by [Freepik](https://www.freepik.com) from [www.flaticon.com](https://www.flaticon.com)
   * [Machine gun](https://www.flaticon.com/free-icon/machine-gun_238558) : icon made by [Freepik](https://www.freepik.com) from [www.flaticon.com](https://www.flaticon.com)
* Maps :
   * Level 1 made by Yannick Bilcot
   * Levels 4, 5 and 6 made by Sylvain Heude
* Musics : [jamendo](https://www.jamendo.com)
   * Akashic Records - Epic Action Hero
   * Aliaksei Yukhnevich - Epicness
   * Art Music - Epic Trailer
   * Blue Giraffe - Action Intense Cinematic
   * Celestial Aeon Project - Epic
   * E. Erkut - Dark Moment - Dark Epic Trailer
   * Matti Paalanen - Emotion
   * Matti Paalanen - Epic Action
   * Nico Wohlleben - Storm
   * Not Really - Cinematic Epical
   * Not Really - Dramatic Background Epic
   * Soundbay - Epic Future
   * Soundshrim - Epic Adventure
* Sounds : [universal-soundbank.com](http://www.universal-soundbank.com), [soundbible.com](http://soundbible.com), [zapsplat.com](https://www.zapsplat.com), [Freesound](https://freesound.org)
* Sprites : [OpenGameArt](https://opengameart.org)
   * [Explosions](https://opengameart.org/content/explosions-0)
* Textures : [textures.com](http://www.textures.com), [Minecraft EnfiCraft texture pack](http://www.minecrafttexturepacks.com/enficraft)
