asmRT
=====

Simple raytracer implemented in x86 assembly using Intel syntax. It runs only on widows, and requires SDL to display graphics.

#Build

To generate a binary you need the following:
* [FASM assembler](http://flatassembler.net/)
* Add the line: 
	
	HeapSetInformation,'HeapSetInformation',\

	in the file INCLUDE\API\KERNEL32.INC.

And in order to run it you'll need these libraries and files:
* [SDL v1.2.13](http://www.libsdl.org/) (included).
* [SDL_gfx](http://cms.ferzkopp.net/index.php/software/13-sdl-gfx) (included).
* [Free Image v3.12](http://freeimage.sourceforge.net/) (included).
* Textures (included).

#Usage

To run simply execute asmRT.exe, there are a few key bindings you can try.

* Numbers from 1 to 7: Load a different demo scene.
* Arrows: Rotate the camera.
* A,S,D,W: Camera panning.
* [Enter]: Switch fullscreen.
* PageUp/PageDown: Resolution change.
* Home/End: Change number of threads.
* Mouse move: Change light position.
* Mouse drag (left button): Pick and move an object.
* [Escape]: Exit.
* F1: Toggle extended frame info.


This program was written in 2009.

Enrique CR.
