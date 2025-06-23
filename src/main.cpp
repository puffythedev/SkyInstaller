#include <map>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include "compat.hpp"

#ifndef APPNAME
#define APPNAME "SkyPong"
#endif
#ifndef DOWNURL
#define DOWNURL "https://github.com/puffythedev/SkyPong/releases/download/v0.1.5/SkyPong.zip"
#endif

namespace fs = std::filesystem;

enum class Screen { MAIN, IOD, INSTALLING };

Screen current = Screen::MAIN;

const int width = 600;
const int height = 430;
const int titleBarHeight = 30;

std::string windowsPath = "C:\\PuffyApps\\";

int rectWidth = 500;
int rectHeight = 50;
int rectX1 = (width - rectWidth) / 2;
int rectY1 = height / 6 + titleBarHeight;
int rectX2 = (width - rectWidth) / 2;
int rectY2 = (height * 7) / 16 - rectHeight + titleBarHeight;
int logoSize = 35;
int logoX = rectX1 + rectWidth - 40;
int logoY = rectY1 + (rectHeight - logoSize) / 2;
int arrowSize = 40;
int arrowX = logoX + (logoSize - arrowSize) / 2 + 10;
int arrowY = logoY + (logoSize - arrowSize) / 2;
const char* welc = "Install " APPNAME "!";

Rectangle Start = { width / 2 - 90, height / 2 + titleBarHeight, 180, 50};
Rectangle Install = { rectX1, rectY1, rectWidth, rectHeight };
Rectangle Uninstall = { rectX2, rectY2, rectWidth, rectHeight };
Rectangle ArrowInstall = { logoX, logoY, logoSize, logoSize };
Rectangle closeButton = { width - 30, 5, 20, 20 };
Rectangle titleBar = { 0, 0, static_cast<float>(width), static_cast<float>(titleBarHeight) };

int selectedApp = -1;
float installProgress = 0.0f;
std::string statusMessage = "";
double statusMessageTime = 0;
std::atomic<bool> installing(false);
std::atomic<bool> installDone(false);
std::atomic<bool> installSuccess(false);

bool mkdirSafe(const std::string& path) {
    if (!fs::exists(path)) {
        return fs::create_directories(path);
    }
    return true;
}

void InstallApp() {
    std::string zipFile = windowsPath + APPNAME + ".zip";
    std::string extractPath = windowsPath + APPNAME;

    if (!mkdirSafe(extractPath)) {
        installSuccess = false;
        installDone = true;
        return;
    }

    if (!WinAPI::DownloadFile(DOWNURL, zipFile)) {
        installSuccess = false;
        installDone = true;
        return;
    }

    installProgress = 0.5f;

    if (!WinAPI::UnzipFile(zipFile, extractPath)) {
        installSuccess = false;
        installDone = true;
        fs::remove(zipFile);
        return;
    }
    
    fs::remove(zipFile);
    installProgress = 1.0f;
    installSuccess = true;
    installDone = true;
    current = Screen::IOD;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(width, height, APPNAME);

    bool draggingWindow = false;
    Vector2 dragStartPosition = {0, 0};
    Vector2 windowStartPosition = {0, 0};
    std::thread installThread;

    while (!WindowShouldClose()) {
        Vector2 mousePosition = GetMousePosition();
        bool startClicked = CheckCollisionPointRec(mousePosition, Start) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
        bool closeClicked = CheckCollisionPointRec(mousePosition, closeButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
        bool titleBarClicked = CheckCollisionPointRec(mousePosition, titleBar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        if (titleBarClicked) {
            draggingWindow = true;
            dragStartPosition = mousePosition;
            windowStartPosition = GetWindowPosition();
        }

        if (draggingWindow && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 delta = Vector2Subtract(mousePosition, dragStartPosition);
            SetWindowPosition(windowStartPosition.x + delta.x, windowStartPosition.y + delta.y);
        } else {
            draggingWindow = false;
        }

        if (closeClicked) {
            break;
        }

        switch (current) {
            case Screen::MAIN: {
                if (startClicked) {
                    current = Screen::IOD;
                }
            } break;

            case Screen::IOD: {
                if (CheckCollisionPointRec(mousePosition, Install) &&
                    IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    selectedApp = 0;
                }

                if (CheckCollisionPointRec(mousePosition, Uninstall) &&
                    IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    statusMessage = "Uninstall not implemented yet!";
                    statusMessageTime = GetTime() + 3;
                }

                if (selectedApp >= 0 && !installing) {
                    current = Screen::INSTALLING;
                    installing = true;
                    installDone = false;
                    installSuccess = false;
                    installProgress = 0.0f;
                    installThread = std::thread(InstallApp);
                    installThread.detach();
                }
            } break;

            case Screen::INSTALLING: {
                if (installDone) {
                    if (installSuccess) {
                        statusMessage = APPNAME " installed successfully!";
                    } else {
                        statusMessage = "Failed to install " APPNAME;
                    }
                    statusMessageTime = GetTime() + 3;
                    current = Screen::IOD;
                    installing = false;
                }
            } break;
        }

        BeginDrawing();
        switch (current) {
            case Screen::MAIN: {
                ClearBackground(BLUE);

                DrawRectangleRec(titleBar, DARKGRAY);
                DrawText(APPNAME, 10, 5, 20, WHITE);
                DrawRectangleRec(closeButton, RED);
                DrawText("x", width - 25, 5, 20, WHITE);

                DrawRectangleRec(Start, WHITE);
                DrawText("Start", width / 2 - 40, height / 2 + 10 + titleBarHeight, 30, BLACK);
                DrawText(welc,
                         width / 2 - (MeasureText(welc, 40) / 2),
                         height / 4 + titleBarHeight, 40, WHITE);
            } break;
            
            case Screen::IOD: {
                ClearBackground(BLUE);
                
                DrawRectangleRec(titleBar, DARKGRAY);
                DrawText(APPNAME, 10, 5, 20, WHITE);
                DrawRectangleRec(closeButton, RED);
                DrawText("x", width - 25, 5, 20, WHITE);
                
                DrawText("Select Action:", rectX1, rectY1 - 30, 20, WHITE);
                DrawRectangleRec(Install, BLACK);
                DrawText("Install App", rectX1 + 20, rectY1 + 15, 20, WHITE);
                
                DrawText("Uninstall App", rectX2 + 20, rectY2 + 15, 20, WHITE);
                DrawRectangleRec(Uninstall, BLACK);
                
                DrawRectangleRec(ArrowInstall, RED);
                DrawText(">", static_cast<float>(arrowX), static_cast<float>(arrowY), arrowSize, BLACK);
                
                if (selectedApp >= 0) {
                    int yPos = rectY1 + (2*rectHeight) + 30;
                    DrawRectangle(rectX1, yPos, rectWidth, rectHeight, RED);
                    DrawText(APPNAME, rectX1 + 20, yPos + 15, 20, WHITE);
                }
            } break;
            
            case Screen::INSTALLING: {
                ClearBackground(BLUE);
                
                DrawRectangleRec(titleBar, DARKGRAY);
                DrawText(APPNAME, 10, 5, 20, WHITE);
                DrawRectangleRec(closeButton, RED);
                DrawText("x", width - 25, 5, 20, WHITE);
                
                std::string text = "Installing " APPNAME "...";
                int textWidth = MeasureText(text.c_str(), 30);
                DrawText(text.c_str(), width/2 - textWidth/2, height/2 - 60, 30, WHITE);
                
                int barWidth = width - 100;
                DrawRectangle(50, height/2 - 20, barWidth, 40, LIGHTGRAY);
                DrawRectangle(50, height/2 - 20, barWidth * installProgress, 40, GREEN);
                DrawRectangleLines(50, height/2 - 20, barWidth, 40, DARKGRAY);
                
                std::string progressText = std::to_string(static_cast<int>(installProgress * 100)) + "%";
                int progressWidth = MeasureText(progressText.c_str(), 20);
                DrawText(progressText.c_str(), width/2 - progressWidth/2,
                         height/2 - 10, 20, DARKGRAY);
            } break;
        }
        
        if (GetTime() < statusMessageTime) {
            int msgWidth = MeasureText(statusMessage.c_str(), 20);
            DrawRectangle(width/2 - msgWidth/2 - 10,
                          height - 50,
                          msgWidth + 20,
                          30,
                          Fade(DARKGRAY, 0.7f));
            DrawText(statusMessage.c_str(),
                     width/2 - msgWidth/2,
                     height - 45, 20, WHITE);
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}