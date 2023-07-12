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
    // Window Initialization
    InitWindow(width, height, "SkyInstaller");
    // Window Opacity
    SetWindowOpacity(0.5f);
    init();
    while(!WindowShouldClose()){
        // keyboard shortcuts
        switch(current){
            case MAIN: {
                // nothing yet
            } break;
        }
        BeginDrawing();
        switch(current){
            case MAIN: {
                ClearBackground(BLUE);
            }
        }
        EndDrawing();
    }
    end();
    CloseWindow();
    return 0;
}