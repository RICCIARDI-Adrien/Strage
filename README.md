# Strage

## How to play
You are in command of a little space ship (flying-saucer model) in an unknown part of the galaxy. What are you doing here ? No one knows, even you...  
Your ship can **move** in up, down, left and right directions using the **keyboard arrow keys**.  You can also **shoot** using **space bar**. Shoot ? Yes, enemies are nearby.  
You do not know why they want you dead, they may have a very good reason, but it's the worse idea they ever had. Defend yourself and escape from this place !  
  
Enemies are teleported to this place to fight you by using big machines. Destroy these machines at sight or they will teleport tons of enemies !  
You have to make your way alive to the level exit. Medipacks and ammunition are hidden somewhere on the map to help you. They can also be dropped by dead enemies, if you are lucky.  
  
And remember, *strage* means *slaughter* in Italian...

## Building
On Debian/Ubuntu systems, you have to install some SDL2 packages.
```
sudo apt install libsdl2-dev libsdl2-mixer-dev libsdl2-ttf-dev
```
Then simply type *make* in the Strage directory to build the game.

## How it works
Levels are made using [Tiled](http://www.mapeditor.org/) editor. They are constituted of 2 layers :
* The *scene* layer containing the textures for the walls and floors.
* The *objects* layer telling where player, spawners and other interactive objects are located on the map.

Scene textures and objects are gathered in two Tiled tilesets (Scene.tsx and Objects.tsx) located in the Tiled_Resources directory.