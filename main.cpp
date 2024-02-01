#include <raylib.h>
#include <cstdint>
#include "FastNoiseLite.h"

int main()
{
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    const int imgWidth = 800;
    const int imgHeight = 800;

    InitWindow(screenWidth, screenHeight, "Pixel Manipulation");

    SetTargetFPS(60);

    Image img = GenImageColor(imgHeight, imgHeight, WHITE);
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    auto pixels = (Color*)img.data;
    Texture tex = LoadTextureFromImage(img);

    FastNoiseLite base;
    base.SetFractalOctaves(5);
    base.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    base.SetFractalType(FastNoiseLite::FractalType_FBm);


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // your update logic goes here
        for(int y = 0; y < imgHeight; y++)
        {
            for(int x = 0; x < imgWidth; x++)
            {
                float posY = (float)((y - imgHeight / 2)) / ((float)imgHeight / 2);
                float posX = (float)((x - imgHeight / 2)) / ((float)imgHeight / 2);

                float islandInfluence = (2 - (abs(posX) + abs(posY))) / 2;
                islandInfluence *= 255;
                float displacement = 100;
                float c1 = ((base.GetNoise((float)x, (float)y, 0.0f) + 1) / 2) * 255;

                auto avg = (c1 + islandInfluence) / 2;

                if (avg < 190)
                    pixels[y * imgWidth + x] = (Color){0,0,139,255}; // Dark Blue
                else if (avg < 200)
                    pixels[y * imgWidth + x] = (Color){255,255,153,255}; // Yellow
                else if (avg < 210)
                    pixels[y * imgWidth + x] = (Color){0,139,0,255}; // Green
                else if (avg < 220)
                    pixels[y * imgWidth + x] = (Color){0,100,0,255}; // Dark Green
                else if (avg < 230)
                    pixels[y * imgWidth + x] = (Color){165,42,0,255}; // Brown
                else
                    pixels[y * imgWidth + x] = (Color){255,255,255,255}; // White


                    /*
                pixels[y * imgWidth + x] = (Color){
                    (uint8_t)avg,
                    (uint8_t)avg,
                    (uint8_t)avg,
                    255
                };
                */

            }
        }
        UpdateTexture(tex, pixels);

        // drawing logic goes here
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(tex, 100, 100, WHITE);
        EndDrawing();
    }

    UnloadTexture(tex);

    CloseWindow();
    return 0;
}