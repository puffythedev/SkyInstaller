// raylib and discord rpc
#include <raylib.h>
#include <a.h>

// Screen System
typedef enum Screen { MAIN } Screen;

// current screen
Screen current = MAIN;

// dimensions
const int width = 400;
const int height = 600;

// main func
int main(){
    InitWindow(width, height, "SkyInstaller");
    init();
    while(!WindowShouldClose()){
        switch(current){
            case MAIN: {
                // nothing yet
            } break;
        }
        
    }
    end();
    CloseWindow();
    return 0;
}