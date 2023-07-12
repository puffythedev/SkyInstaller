all:
	g++ main.cpp -o out/SkyInstaller.exe -I./src/include -L./src/lib -lraylib -lopengl32 -lwinmm -mwindows