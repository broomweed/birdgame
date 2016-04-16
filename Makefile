WINDOWS_MAKE=mingw32-make
UNIX_MAKE=make

win: main.cpp Animation.hpp Entity.hpp SpriteSheet.hpp TileMap.hpp
	$(WINDOWS_MAKE) -f Makefile.win

nix: main.cpp Animation.hpp Entity.hpp SpriteSheet.hpp TileMap.hpp
	$(UNIX_MAKE) -f Makefile.nix
