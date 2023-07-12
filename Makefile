all:
	g++ main.cpp -I./src/include -L./src/lib -lraylib -lopengl32 -lwinmm -mwindows