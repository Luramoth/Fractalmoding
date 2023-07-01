#include <stdio.h>
#include <raylib.h>

#define MAX_ITERATIONS 200
#define WIDTH 1000
#define HEIGHT 800

int mandelbrot(float x, float y) {
    float xx = 0.0f, yy= 0.0f;
    int iter = 0;

    while (xx*xx + yy*yy <= 4.0f && iter < MAX_ITERATIONS){
        float xt = xx*xx - yy*yy + x;
        yy = 2.0f * xx * yy + y;
        xx = xt;
        iter ++;
    }

    return iter;
}

int main() {
    printf("Hello, World!\n");

    InitWindow(WIDTH, HEIGHT, "Fractalmoding");
    Image im = GenImageColor(WIDTH, HEIGHT, BLACK);
    Texture2D tex = LoadTextureFromImage(im);

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        for (int x = 0; x < WIDTH; ++x){
            for (int y = 0; y < HEIGHT; ++y){
                float cx = (float)x / WIDTH * 3.5f - 2.5f;
                float cy = (float)y / HEIGHT * 2.0f - 1.0f;

                int iterations = mandelbrot(cx, cy);
                Color col = (Color){iterations % 8 * 32,
                                    iterations % 16 * 16,
                                    iterations % 32 * 8, 255};

                ImageDrawPixel(&im, x, y, col);
            }
        }
        UnloadTexture(tex);
        tex = LoadTextureFromImage(im);

        BeginDrawing();
        DrawTexture(tex, 0, 0, WHITE);
        EndDrawing();
    }

    UnloadTexture(tex);
    UnloadImage(im);
    CloseWindow();

    return 0;
}
