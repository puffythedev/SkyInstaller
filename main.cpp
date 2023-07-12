// raylib and discord rpc
#include <raylib.h>

// Screen System
typedef enum Screen { MAIN } Screen;

// current screen
Screen current = MAIN;

// dimensions
const int width = 600;
const int height = 400;
const char* welc = "Welcome To SkyInstaller!";

// rectangles
Rectangle Start = { width / 2 - 90, height / 2, 180, 50};
Rectangle Info = { width - 25, height - 25, 20, 20 };

// main func
int main(){
    // Window Initialization
    InitWindow(width, height, "SkyInstaller");
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
                DrawRectangleRec(Start, WHITE);
                DrawRectangleRec(Info, WHITE);
                DrawText("Start", width / 2 - 40, height / 2 + 10, 30, BLACK);
                DrawText("Welcome To SkyInstaller!", width / 2 - (MeasureText(welc, 40) / 2), height / 4, 40, BLACK);
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}