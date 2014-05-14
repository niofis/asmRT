asmRT
=====

Simple raytracer implemented in x86 assembly using Intel syntax. It runs only on widows, and requires SDL to display graphics.

In order to generate a binary you need the following:
* [FASM assembler](http://flatassembler.net/)
* Add the line: 
	HeapSetInformation,'HeapSetInformation',\
	in file INCLUDE\API\KERNEL32.INC of fasm

And in order to run it you'll need these libraries and files:
* [SDL v1.2.13](http://www.libsdl.org/)(included).
* [SDL_gfx](http://cms.ferzkopp.net/index.php/software/13-sdl-gfx)(included).
* [Free Image v3.12](http://freeimage.sourceforge.net/)(included).
* Textures(included).

This program was written in 2009.
