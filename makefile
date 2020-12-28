all:
	g++ bitmap.cpp move_and_collide.c const_trig.cpp misc.cpp level.cpp player.cpp ray_engine.cpp main.cpp -std=c++11 -lSDL2main -lSDL2 -o CCast.out
