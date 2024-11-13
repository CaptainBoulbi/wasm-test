#include "penger.c"

#define GREEN 0xff00ff00
#define RED 0xff0000ff
#define BLUE 0xffff0000
#define BLACK 0xff000000

#define GRAVITY 9

const unsigned int width = 800;
const unsigned int height = 600;
unsigned int BUFFER[width * height];

int get_scale(void);

typedef struct v2 {
    float x, y;
} v2;

v2 penger_pos = {400, 300};

float random(void);

int rand(int min, int max)
{
    return min + random() * (max - min);
}

typedef enum Key {
    SHIFT = 16,
    SPACE = 32,
    ARROW_LEFT = 37,
    ARROW_RIGHT = 39,
    ARROW_UP = 38,
    ARROW_DOWN = 40,
    KEY_COUNT,
} Key;

Key keys[KEY_COUNT] = {0};

void key_pressed(int key)
{
    if (key < 0 || key >= KEY_COUNT) return;
    keys[key] = 1;
}

void key_released(int key)
{
    if (key < 0 || key >= KEY_COUNT) return;
    keys[key] = 0;
}

v2 velocity = {0, 0};

void reset_velocity(void)
{
    velocity = (v2){0, 0};
}

void rebondi(v2 *pos, int scale)
{
    if (pos->x - penger_width*scale/2 < 0) {
        pos->x = penger_width*scale/2;
        velocity.x /= -1.5;
    }
    if (pos->y - penger_height*scale/2 < 0) {
        pos->y = penger_height*scale/2;
        velocity.y /= -1.5;
    }
    if (pos->x + penger_width*scale/2 >= width) {
        pos->x = width - penger_width*scale/2;
        velocity.x /= -1.5;
    }
    if (pos->y + penger_height*scale/2 >= height) {
        pos->y = height - penger_height*scale/2;
        velocity.y /= -1.5;
    }
}

void init()
{
}

void go(float dt)
{
    int scale = get_scale();

    if (keys[SPACE]) {
        velocity.y += velocity.y < 0 ? -10 : 10;
        velocity.x += rand(-10, 10);
    }

    if (!keys[ARROW_UP] && !keys[ARROW_DOWN]) {
        velocity.y += GRAVITY * dt;
        penger_pos.y += velocity.y;
    }
    if (!keys[ARROW_LEFT] && !keys[ARROW_RIGHT]) {
        penger_pos.x += velocity.x;
    }

    float speed = 10.0f;
    if ((keys[ARROW_UP] || keys[ARROW_DOWN]) && (keys[ARROW_LEFT] || keys[ARROW_RIGHT]))
        speed = 6.324f; // sqrt(speed) * 2 with speed = 10
    if (keys[SHIFT])
        speed /= 2.0f;

    if (keys[ARROW_RIGHT]) {
        penger_pos.x += speed;
        if (velocity.x < 0) velocity.x *= -1;
    }
    if (keys[ARROW_LEFT]) {
        penger_pos.x -= speed;
        if (velocity.x > 0) velocity.x *= -1;
    }
    if (keys[ARROW_DOWN])
        penger_pos.y += speed;
    if (keys[ARROW_UP])
        penger_pos.y -= speed;

    for (int i = 0; i < width * height; i++)
        BUFFER[i] = GREEN;


    rebondi(&penger_pos, scale);

    v2 penger_origin = {0};
    penger_origin.x = penger_pos.x - penger_width*scale/2;
    penger_origin.y = penger_pos.y - penger_height*scale/2;

    for (int y = 0; y < penger_height; y++) {
        for (int i = 0; i < penger_width; i++) {
            if (penger_img[y][i] <= 0x00FFFFFF)
                continue;
            for (int s1 = 0; s1 < scale; s1++) {
                for (int s2 = 0; s2 < scale; s2++) {
                    int idx_x = penger_origin.x + i*scale+s1;
                    int idx_y = penger_origin.y + y*scale+s2;
                    if (idx_x < 0 || idx_x >= width || idx_y < 0 || idx_y >= height)
                        continue;
                    BUFFER[idx_y*width + idx_x] = penger_img[y][i];
                }
            }
        }
    }
}
