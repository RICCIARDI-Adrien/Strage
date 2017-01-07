# Strage

## How to play
*Strage* means *slaughter* in Italian. TODO  
TODO controls

## Building
On Debian/Ubuntu systems, you have to install some SDL2 packages.
```
sudo apt install libsd2-dev libsdl2-ttf-dev
```
Then simply type *make* in the Strage directory to build the game.

## How it works
Levels are made using [Tiled](http://www.mapeditor.org/) editor. They are constituted of 2 layers :
* The *scene* layer containing the textures for the walls and floors
* The *objects* layer telling where player, spawners and other interactive objects are located on the map