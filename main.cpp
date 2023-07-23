// raylib
#include <raylib.h>

// Screen System
typedef enum Screen { MAIN, IOD } Screen;

// current screen
Screen current = MAIN;

// dimensions
const int width = 600;
const int height = 400;
int rectWidth = 500;
int rectHeight = 50;
int rectX1 = (width - rectWidth) / 2;
int rectY1 = height / 6;
int rectX2 = (width - rectWidth) / 2;
int rectY2 = (height * 7) / 16 - rectHeight;
int logoSize = 35;
int logoX = rectX1 + rectWidth - 40; 
int logoY = rectY1 + (rectHeight - logoSize) / 2;
int arrowSize = 40; 
int arrowX = logoX + (logoSize - arrowSize) / 2 + 10;
int arrowY = logoY + (logoSize - arrowSize) / 2;
float rotationAngle = 90.0f;
const char* welc = "Welcome To SkyInstaller!";

// rectangles
Rectangle Start = { width / 2 - 90, height / 2, 180, 50};
Rectangle Info = { width - 25, height - 25, 20, 20 };
Rectangle Install = { rectX1, rectY1, rectWidth, rectHeight };
Rectangle Uninstall = { rectX2, rectY2, rectWidth, rectHeight };
Rectangle ArrowInstall = { logoX, logoY, logoSize, logoSize };

// main func
int main(){
    // Window Initialization
    InitWindow(width, height, "SkyInstaller");
    while(!WindowShouldClose()){
        Vector2 mousePosition = GetMousePosition();
        bool startClicked = CheckCollisionPointRec(mousePosition, Start) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
        // keyboard shortcuts
        switch(current){
            case MAIN: {
                if(startClicked){
                    current = IOD;
                }
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
            } break;
            case IOD: {
                ClearBackground(BLUE);
                DrawRectangleRec(Install, BLACK);
                DrawRectangleRec(Uninstall, BLACK);
                DrawRectangleRec(ArrowInstall, RED);
                DrawTextEx(GetFontDefault(), ">", { static_cast<float>(arrowX), static_cast<float>(arrowY) }, 
                   static_cast<float>(arrowSize), 0.0f, BLACK);
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}