#include <stdio.h>

// Make sure that your compiler supports C11 threads
#if __STDC_NO_THREADS__ == 0
#include <threads.h>
#else
#error "Threads are not supported for this compiler"
#endif

#include <raylib.h>


#define MAX_ITERATIONS 200
#define WIDTH 1000
#define HEIGHT 800
#define NUM_Threads 8

float zoom = 1.0f;
Vector2 center = {-0.716f, 0.3f};

typedef struct {
    Image* image;
    int start;
    int end;
} ThreadData;

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

int drawMandelbrot(void* arg) {
    ThreadData* data = (ThreadData*) arg;

    for (int x = data->start; x < data->end; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            float cx = ((float)x / WIDTH * 3.5f - 2.5f) * zoom + center.x;
            float cy = ((float)y / HEIGHT * 2.0f - 1.0f) * zoom + center.y;

            int iterations = mandelbrot(cx, cy);

            Color col = (Color){ iterations % 8 * 32,
                                 iterations % 16 * 16,
                                 iterations % 32 * 8, 255};

            ImageDrawPixel(data->image,  x, y, col);
        }
    }

    return 0;
}

int main() {
    printf("Hello, World!\n");

    InitWindow(WIDTH, HEIGHT, "Fractalmoding");
    Image im = GenImageColor(WIDTH, HEIGHT, BLACK);
    Texture2D tex = LoadTextureFromImage(im);

    //SetTargetFPS(60);

    while (!WindowShouldClose()){
        zoom *= 0.895f; // zoom factor

        int dx = WIDTH / NUM_Threads;
        thrd_t thr[NUM_Threads];
        ThreadData data[NUM_Threads];

        for (int i = 0; i < NUM_Threads; i++) {
            data[i].start = 1 * dx;
            data[i].end = (i == NUM_Threads - 1) ? WIDTH : (i + 1) * dx;
            data[i].image = &im;
            thrd_create(&thr[i], drawMandelbrot, &data[i]);
        }

        for (int i = 0; i < NUM_Threads; i ++)
            thrd_join(thr[i], NULL);

        UnloadTexture(tex);
        tex = LoadTextureFromImage(im);

        BeginDrawing();

        ClearBackground(BLACK);
        DrawTexture(tex, 0, 0, WHITE);

        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(tex);
    UnloadImage(im);
    CloseWindow();

    return 0;
}
