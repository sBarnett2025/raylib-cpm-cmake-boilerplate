#include <raylib.h>
#include <cstdint>
#include "FastNoiseLite.h"
#include "iostream"
#include <time.h>

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

    float *heightData;
    heightData = new float[imgWidth * imgHeight];

    float materialTotal = 0;

    auto pixels = (Color*)img.data;
    Texture tex = LoadTextureFromImage(img);

    FastNoiseLite base;
    base.SetFractalOctaves(8);
    base.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    base.SetFractalType(FastNoiseLite::FractalType_FBm);

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

            auto avg = (c1 + islandInfluence) / 2;

            heightData[y * imgWidth + x] = avg;

            if ((x == 0) || (y == 0) || (x == imgWidth - 1) || (y == imgHeight - 1))
            {
                heightData[y * imgWidth + x] = 0;
            }


        }
    }

    float materialPerStep = 1;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // your update logic goes here

        // do rain
        materialTotal = 0;

        srand(time(NULL));
        int randOne = rand() % imgWidth;
        int randTwo = rand() % imgHeight;
        Vector2 point = {(float)randOne, (float)randTwo};

        bool lowestPointYet;
        float lowestHeight = heightData[(int)point.y * imgWidth + (int)point.x];
        Vector2 current = point;
        Vector2 next;

        std::cout << "lololololololololol" << std::endl;

        while (!lowestPointYet)
        {
            std::cout << "pp" << std::endl;
            // calculate flow
            Vector2 dirs[4] = {1,0,-1,0,0,1,0,-1};
            for (int i = 0; i < 4; i++)
            {
                Vector2 temp = {current.x + dirs[i].x, current.y + dirs[i].y};
                if (heightData[(int)temp.y * imgWidth + (int)temp.x] < lowestHeight)
                {
                    lowestHeight = heightData[(int)temp.y * imgWidth + (int)temp.x];
                    next = temp;
                }

            }
            if (heightData[(int)current.y * imgWidth + (int)current.x] == lowestHeight)
            {
                lowestPointYet = true;
            }
            heightData[(int)current.y * imgWidth + (int)current.x] -= materialPerStep;
            materialTotal += materialPerStep;

            current = next;

        }
        heightData[(int)current.y * imgWidth + (int)current.x] += materialTotal;




        
        // draw pixels
        for(int y = 0; y < imgHeight; y++)
        {
            for(int x = 0; x < imgWidth; x++)
            {

                // check height
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
        //std::cout << "ggg" << std::endl;
        DrawTexture(tex, 100, 100, WHITE);
        //std::cout << "hhh" << std::endl;
        EndDrawing();

    }

    delete[] heightData;

    UnloadTexture(tex);

    CloseWindow();
    return 0;
}