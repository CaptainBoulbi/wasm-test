#include "penger.c"

#define BLUE 0xffff0000
#define RED 0xff0000ff
#define BLACK 0xff000000

const unsigned int width = 800;
const unsigned int height = 600;
unsigned int BUFFER[width * height];

typedef struct v2 {
    float x, y;
} v2;

#define NB 10
v2 pos[NB];

#define GRAVITY 5

#define SPEED 500
v2 dir[NB];

unsigned int radius = 30;

float random(void);

int rand(int min, int max)
{
    return min + random() * (max - min);
}

void init()
{
    for (int i = 0; i < NB; i++) {
        pos[i].x = rand(radius, width - radius);
        pos[i].y = rand(radius, height - radius);

        dir[i].x = SPEED;
        dir[i].y = SPEED;
    }
}

void go(float dt, int scale)
{
    for (int i = 0; i < NB; i++) {
        v2 npos;
        npos.x = pos[i].x + dir[i].x * dt;
        npos.y = pos[i].y + dir[i].y * dt;

        if (npos.x - radius < 0 || npos.x + radius > width) {
            dir[i].x = -dir[i].x;
        }
        if (npos.y - radius < 0 || npos.y + radius > height) {
            dir[i].y = -dir[i].y;
        }

        pos[i].x += dir[i].x * dt;
        pos[i].y += dir[i].y * dt;
    }

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            unsigned int color = BLUE;
            for (int p = 0; p < NB; p++) {
                unsigned int dw = (unsigned int) pos[p].x - w;
                unsigned int dh = (unsigned int) pos[p].y - h;
                unsigned int d = dw * dw + dh * dh;
                
                if (d <= radius*radius) {
                    unsigned short int red = (float) d/(radius*radius) * 255;
                    color = BLACK + red;
                    break;
                }
            }
            BUFFER[h*width + w] = color;
        }
    }

    v2 penger_origin = {(float) width/2 - 32*scale/2, (float) height/2 - 32*scale/2};
    for (int i = 0; i < 32; i++) {
        for (int y = 0; y < 32; y++) {
            if (penger_img[i][y] == 0)
                continue;
            for (int s1 = 0; s1 < scale; s1++) {
                for (int s2 = 0; s2 < scale; s2++) {
                    int idx_x = penger_origin.x + y*scale+s1;
                    int idx_y = penger_origin.y + i*scale+s2;
                    if (idx_x < 0 || idx_x > width || idx_y < 0 || idx_y > height)
                        continue;
                    BUFFER[idx_y*width + idx_x] = penger_img[i][y];
                }
            }
        }
    }
}
