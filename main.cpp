#include <raylib.h>
#include <cstdint>
#include "FastNoiseLite.h"
#include "iostream"

int main()
{
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    const int imgWidth = 400;
    const int imgHeight = 400;

    InitWindow(screenWidth, screenHeight, "Pixel Manipulation");

    SetTargetFPS(60);

    Image img = GenImageColor(imgHeight, imgHeight, WHITE);
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    float *heightData = new float(imgWidth * imgHeight);
    auto pixels = (Color*)img.data;
    Texture tex = LoadTextureFromImage(img);

    FastNoiseLite base;
    base.SetFractalOctaves(5);
    base.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    base.SetFractalType(FastNoiseLite::FractalType_FBm);

    int counter = 0;

    for(int y = 0; y < imgHeight; y++)
    {
        for(int x = 0; x < imgWidth; x++)
        {
            float posY = (float)((y - imgHeight / 2)) / ((float)imgHeight / 2);
            float posX = (float)((x - imgWidth / 2)) / ((float)imgWidth / 2);

            float islandInfluence = (2 - (abs(posX) + abs(posY))) / 2;
            islandInfluence *= 255;
            //float displacement = 100;
            float c1 = ((base.GetNoise((float)x, (float)y, 0.0f) + 1) / 2) * 255;
            counter++;
            //std::cout << counter << std::endl;
            //std::cout << "x: " << x << " y: " << y << std::endl;
            //std::cout << c1 << std::endl;

            auto avg = (c1 + islandInfluence) / 2;

            heightData[y * imgWidth + x] = avg;

        }
    }

    counter = 0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // your update logic goes here

        //std::cout << "ggg" << std::endl;
        for(int y = 0; y < imgHeight; y++)
        {
            for(int x = 0; x < imgWidth; x++)
            {
                counter++;
                //heightData[y * imgWidth + x] -= 25;
                //std::cout << counter << std::endl;
                //std::cout << "x: " << x << " y: " << y << std::endl;
                if (heightData[y * imgWidth + x] < 190)
                {
                    pixels[y * imgWidth + x] = (Color){0,0,139,255}; // Dark Blue
                }
                else if (heightData[y * imgWidth + x] < 200)
                {
                    pixels[y * imgWidth + x] = (Color) {255, 255, 153, 255}; // Yellow
                }
                else if (heightData[y * imgWidth + x] < 210)
                {
                    pixels[y * imgWidth + x] = (Color){0,139,0,255}; // Green
                }
                else if (heightData[y * imgWidth + x] < 220)
                {
                    pixels[y * imgWidth + x] = (Color){0,100,0,255}; // Dark Green
                }
                else if (heightData[y * imgWidth + x] < 230)
                {
                    pixels[y * imgWidth + x] = (Color){165,42,0,255}; // Brown
                }
                else
                {
                    pixels[y * imgWidth + x] = (Color){255,255,255,255}; // White
                }
            }
        }




        UpdateTexture(tex, pixels);

        // drawing logic goes here
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(tex, 100, 100, WHITE);
        std::cout << "dsadasdsadasdas" << std::endl;
        EndDrawing();

    }

    delete heightData;

    UnloadTexture(tex);

    CloseWindow();
    return 0;
}