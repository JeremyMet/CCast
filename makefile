all:
	g++ -IC:/MinGW/SDL2/x86_64-w64-mingw32/include -LC:/MinGW/SDL2/x86_64-w64-mingw32/lib bitmap.cpp const_trig.cpp misc.cpp level.cpp player.cpp sprite.cpp ray_engine.cpp main.cpp -lmingw32 -lSDL2main -lSDL2 -o CCast.exe