OBJ=src/falling.o src/game.o src/hud.o src/main.o src/map.o src/sprite.o
CXXFLAGS=-g `sdl-config --cflags`
LDFLAGS=-g `sdl-config --libs` -lSDL_mixer -lSDL_image


ninjashoppingmadness: $(OBJ)
	g++ $(OBJ) $(CXXFLAGS) $(LDFLAGS) -o ninjashoppingmadness
