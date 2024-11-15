#include "pengers.h"
#include "hand.c"

#define GREEN 0xff00ff00
#define RED 0xff0000ff
#define BLUE 0xffff0000
#define BLACK 0xff000000

#define EPSILON 0.000001f
#define GRAVITY 9

const unsigned int width = 800;
const unsigned int height = 600;
unsigned int BUFFER[width * height];
int id = 0;

// importer depuis js
int get_scale(void);
float random(void); // flemme de coder un algo random, je recup celui de js (Math.random)
float sqrtf(float val); // pareil

typedef struct v2 {
    float x, y;
} v2;

// position du penger au milieu
v2 penger_pos = {width/2, height/2};
v2 velocity = {0, 0};
v2 mouse = {0, 0};

v2 v2_diff(v2 vec1, v2 vec2)
{
    return (v2) {
        .x = vec1.x - vec2.x,
        .y = vec1.y - vec2.y,
    };
}
v2 v2_normalize(v2 vec)
{
    v2 result = {0};
    float length = sqrtf((vec.x*vec.x) + (vec.y*vec.y));

    if (length > 0) {
        float ilength = 1.0f/length;
        result.x = vec.x*ilength;
        result.y = vec.y*ilength;
    }

    return result;
}
v2 v2_scale(v2 vec, int scale)
{
    return (v2) {
        .x = vec.x * scale,
        .y = vec.y * scale,
    };
}

// keyboard code definit par js
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

void set_velocity(float x, float y)
{
    velocity = (v2){x, y};
}

void set_mouse(float x, float y)
{
    mouse = (v2){x, y};
}

int rand(int min, int max)
{
    return min + random() * (max - min);
}

void rebondi(v2 *pos, int scale)
{
    float div = -1.5;
    if (pos->x - pengers_width[id]*scale/2 < 0) {
        pos->x = pengers_width[id]*scale/2;
        velocity.x /= div;
    }
    if (pos->y - pengers_height[id]*scale/2 < 0) {
        pos->y = pengers_height[id]*scale/2;
        velocity.y /= div;
    }
    if (pos->x + pengers_width[id]*scale/2 >= width) {
        pos->x = width - pengers_width[id]*scale/2;
        velocity.x /= div;
    }
    if (pos->y + pengers_height[id]*scale/2 >= height) {
        pos->y = height - pengers_height[id]*scale/2;
        velocity.y /= div;
    }
}

int collision(v2 point, int x, int y, int w, int h)
{
    return (point.x >= x && point.x < x + w &&
            point.y >= y && point.y < y + h);
}

void init()
{
    pengers_init();
}

void draw(float dt)
{
    int scale = get_scale();

    // position du penger en haut a gauche de l'image
    v2 penger_origin = {0};
    penger_origin.x = penger_pos.x - pengers_width[id]*scale/2;
    penger_origin.y = penger_pos.y - pengers_height[id]*scale/2;

    // jump
    if (keys[SPACE]) {
        velocity.y += velocity.y < 0 ? -10 : 10;
        velocity.x += rand(-10, 10);
    }

    // mouse push
    if (collision(mouse, penger_origin.x, penger_origin.y, pengers_width[id]*scale, pengers_height[id]*scale)) {
        v2 force = v2_diff(penger_pos, mouse);
        force = v2_normalize(force);
        force = v2_scale(force, 5);
        velocity.x += force.x;
        velocity.y += force.y;
    }

    // update pos avec velocity si pas de touche presser
    if (!keys[ARROW_UP] && !keys[ARROW_DOWN]) {
        velocity.y += GRAVITY * dt;
        penger_pos.y += velocity.y;
    }
    if (!keys[ARROW_LEFT] && !keys[ARROW_RIGHT]) {
        penger_pos.x += velocity.x;
    }

    // movement
    float speed = 10.0f;
    if ((keys[ARROW_UP] || keys[ARROW_DOWN]) && (keys[ARROW_LEFT] || keys[ARROW_RIGHT]))
        speed = 6.324f; // sqrt(speed) * 2 with speed = 10
    if (keys[SHIFT])
        speed /= 2.0f;

    if (keys[ARROW_RIGHT]) {
        penger_pos.x += speed;
        if (velocity.x < 0) velocity.x *= -1;
        else if (velocity.x >= -EPSILON) velocity.x = 1;
    }
    if (keys[ARROW_LEFT]) {
        penger_pos.x -= speed;
        if (velocity.x > 0) velocity.x *= -1;
        else if (velocity.x <= EPSILON) velocity.x = -1;
    }
    if (keys[ARROW_DOWN])
        penger_pos.y += speed;
    if (keys[ARROW_UP])
        penger_pos.y -= speed;

    // background
    for (int i = 0; i < width * height; i++)
        BUFFER[i] = GREEN;

    rebondi(&penger_pos, scale);

    // dessine le penger sur le canva
    for (int y = 0; y < pengers_height[id]; y++) {
        for (int i = 0; i < pengers_width[id]; i++) {
            int i_for_reverse_pixel_rendering_it_s_craazy = i;
            if (velocity.x < EPSILON)
                i_for_reverse_pixel_rendering_it_s_craazy = pengers_width[id]-i-1;
            if (pengers_img[id][y*pengers_width[id] + i_for_reverse_pixel_rendering_it_s_craazy] <= 0x00FFFFFF) // pixel transparant
                continue;
            for (int s1 = 0; s1 < scale; s1++) {
                for (int s2 = 0; s2 < scale; s2++) {
                    int idx_x = penger_origin.x + i*scale+s1;
                    int idx_y = penger_origin.y + y*scale+s2;
                    if (idx_x < 0 || idx_x >= width || idx_y < 0 || idx_y >= height)
                        continue;
                    BUFFER[idx_y*width + idx_x] = pengers_img[id][y*pengers_width[id] + i_for_reverse_pixel_rendering_it_s_craazy];
                }
            }
        }
    }

    // draw hand
    for (int y = 0; y < hand_height; y++) {
        for (int x = 0; x < hand_width; x++) {
            if (hand_img[y][x] <= 0x00FFFFFF) // pixel transparant
                continue;
            int idx_x = x + mouse.x;
            int idx_y = y + mouse.y;
            if (idx_x < 0 || idx_x >= width || idx_y < 0 || idx_y >= height)
                continue;
            BUFFER[idx_y*width + idx_x] = hand_img[y][x];
        }
    }
}
