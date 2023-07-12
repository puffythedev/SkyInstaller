all:
	g++ discordinit.cpp main.cpp -I./src/include -L./src/lib -lraylib -lopengl32 -ldiscord-rpc