#include "pengers.h"
#include "hand.c"

#define GREEN 0xff00ff00
#define RED 0xff0000ff
#define BLUE 0xffff0000
#define BLACK 0xff000000

#define EPSILON 0.000001f
#define GRAVITY 12
#define AIR_RESISTANCE 5

const unsigned int width = 800;
const unsigned int height = 600;
unsigned int BUFFER[width * height];
int id = 0;
int dir = 0;

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

float get_pos_x(void)
{
    return penger_pos.x - pengers_width[id];
}
float get_pos_y(void)
{
    return penger_pos.y - pengers_height[id];
}

typedef struct Player {
    int rid, id, x, y, dir;
} Player;

#define MAX_PLAYERS 20
Player players[MAX_PLAYERS] = {0};
int nb_players = 0;

void deco_player(int rid)
{
    for (int i = 0; i < nb_players; i++) {
        if (players[i].rid == rid) {
            players[i].rid = -1;
            return;
        }
    }
}

void draw_player(int rid, int id, int x, int y, int dir)
{
    if (nb_players >= MAX_PLAYERS) return;
    for (int i = 0; i < nb_players; i++) {
        if (players[i].rid == rid) {
            players[i].id = id;
            players[i].x = x;
            players[i].y = y;
            players[i].dir = dir;
            return;
        }
    }
    players[nb_players].rid = rid;
    players[nb_players].id = id;
    players[nb_players].x = x;
    players[nb_players].y = y;
    players[nb_players].dir = dir;
    nb_players++;
}

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
    D = 68,
    Q = 81,
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

typedef struct Collision {
    int x, y, width, height;
} Collision;

#define NB_COLLISIONS 10
Collision collisions[NB_COLLISIONS] = {};
int nb_collisions = 0;

Collision collision_union(Collision rec1, Collision rec2)
{
    Collision overlap = { 0 };

    int left = (rec1.x > rec2.x)? rec1.x : rec2.x;
    int right1 = rec1.x + rec1.width;
    int right2 = rec2.x + rec2.width;
    int right = (right1 < right2)? right1 : right2;
    int top = (rec1.y > rec2.y)? rec1.y : rec2.y;
    int bottom1 = rec1.y + rec1.height;
    int bottom2 = rec2.y + rec2.height;
    int bottom = (bottom1 < bottom2)? bottom1 : bottom2;

    if ((left < right) && (top < bottom)) {
        overlap.x = left;
        overlap.y = top;
        overlap.width = right - left;
        overlap.height = bottom - top;
    }

    return overlap;
}

Collision collision_rec(Collision fix, int i, int scale)
{
    Collision col = collision_union(fix, (Collision) {
            .x = penger_pos.x - pengers_width[id]*scale/2,
            .y = penger_pos.y - pengers_height[id]*scale/2,
            .width = pengers_width[id]*scale,
            .height = pengers_height[id]*scale,
            });
    if (col.width == 0 && col.height == 0) return col;
    if (col.width == 1 && col.height == 1) return col;

    float div = -1.8;
    if (col.width <= col.height) {
        if (fix.x + fix.width == col.x + col.width)
            penger_pos.x = fix.x + fix.width + pengers_width[id]*scale/2;
        else if (fix.x == col.x)
            penger_pos.x = col.x - pengers_width[id]*scale/2 + 1;
        velocity.x /= div;
    } else {
        if (fix.y + fix.height == col.y + col.height)
            penger_pos.y = fix.y + fix.height + pengers_height[id]*scale/2;
        else if (fix.y == col.y)
            penger_pos.y = col.y - pengers_height[id]*scale/2 + 1;
        velocity.y /= div;
    }
    return col;
}

void reset_collisions()
{
    nb_collisions = 0;
}
void add_collisions(int x, int y, int width, int height)
{
    if (nb_collisions >= NB_COLLISIONS) return;
    collisions[nb_collisions++] = (Collision) {
        .x = x,
        .y = y,
        .width = width,
        .height = height,
    };
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
    float div = -1.8;
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
    collisions[nb_collisions++] = (Collision) {
        .x = 100,
        .y = 400,
        .width = 50,
        .height = 100,
    };
    collisions[nb_collisions++] = (Collision) {
        .x = 150,
        .y = 450,
        .width = 500,
        .height = 50,
    };
    collisions[nb_collisions++] = (Collision) {
        .x = 650,
        .y = 400,
        .width = 50,
        .height = 100,
    };
    collisions[nb_collisions++] = (Collision) {
        .x = 280,
        .y = 200,
        .width = 75,
        .height = 75,
    };
    collisions[nb_collisions++] = (Collision) {
        .x = 445,
        .y = 200,
        .width = 75,
        .height = 75,
    };
}

void draw(float dt)
{
    int scale = get_scale();

    // position du penger en haut a gauche de l'image
    v2 penger_origin = {0};
    penger_origin.x = penger_pos.x - pengers_width[id]*scale/2;
    penger_origin.y = penger_pos.y - pengers_height[id]*scale/2;

    // jump
    static int jumped = 0;
    if (keys[SPACE] && !jumped) {
        jumped = 1;
        velocity.y = -5;
        velocity.x += rand(-2, 2);
    } else if (!keys[SPACE]) {
        jumped = 0;
    }

    // mouse push
    if (collision(mouse, penger_origin.x, penger_origin.y, pengers_width[id]*scale, pengers_height[id]*scale)) {
        v2 force = v2_diff(penger_pos, mouse);
        force = v2_normalize(force);
        force = v2_scale(force, 5);
        velocity.x += force.x;
        velocity.y += force.y;
    }

    if ((!keys[ARROW_LEFT] || !keys[Q]) && (!keys[ARROW_RIGHT] || !keys[D])) {
        penger_pos.x += velocity.x;
    }
    if (id == 28) // fatger id
        velocity.y += GRAVITY * dt * 7;
    else
        velocity.y += GRAVITY * dt;
    penger_pos.y += velocity.y;
    if (velocity.x <= -0.1)
        velocity.x += AIR_RESISTANCE * dt;
    else if (velocity.x >= 0.1)
        velocity.x -= AIR_RESISTANCE * dt;
    else velocity.x = 0;
    penger_pos.x += velocity.x;

    // movement
    int x_collide = 0;
    float speed = 100.0f * dt;
    if (keys[SHIFT])
        speed /= 2.0f;

    if (keys[ARROW_RIGHT] || keys[D]) {
        penger_pos.x += speed;
        if (velocity.x < 0) {
            velocity.x *= -1;
            x_collide = 1;
        }
        else if (velocity.x >= -EPSILON) velocity.x = speed;
    }
    if (keys[ARROW_LEFT] || keys[Q]) {
        penger_pos.x -= speed;
        if (velocity.x > 0) {
            velocity.x *= -1;
            x_collide = 1;
        }
        else if (velocity.x <= EPSILON) velocity.x = -speed;
    }

    // background
    for (int i = 0; i < width * height; i++)
        BUFFER[i] = GREEN;

    rebondi(&penger_pos, scale);

    for (int i = 0; i < nb_collisions; i++) {
        Collision col = collision_rec(collisions[i], i, scale);
        x_collide = x_collide || (col.height > 1);
    }

    // dessine le penger des autres joueur
    for (int p = 0; p < nb_players; p ++) {
        Player player = players[p];
        if (player.rid == -1) continue;
        int scale = 2;
        for (int y = 0; y < pengers_height[player.id]; y++) {
            for (int i = 0; i < pengers_width[player.id]; i++) {
                int real_i = i;
                if (player.dir == -1)
                    real_i = pengers_width[player.id]-i-1;

                if (pengers_img[player.id][y*pengers_width[player.id] + real_i] <= 0x00FFFFFF) // pixel transparant
                    continue;

                for (int s1 = 0; s1 < scale; s1++) {
                    for (int s2 = 0; s2 < scale; s2++) {
                        int idx_x = player.x + i*scale+s1;
                        int idx_y = player.y + y*scale+s2;
                        if (idx_x < 0 || idx_x >= width || idx_y < 0 || idx_y >= height)
                            continue;
                        BUFFER[idx_y*width + idx_x] = pengers_img[player.id][y*pengers_width[player.id] + real_i];
                    }
                }
            }
        }
    }

    // dessine le penger sur le canva
    for (int y = 0; y < pengers_height[id]; y++) {
        for (int i = 0; i < pengers_width[id]; i++) {
            int i_for_reverse_pixel_rendering_it_s_craazy = 0;
            static int last_dir = 1;
            if ((velocity.x < -EPSILON && !x_collide) || last_dir == -1) {
                i_for_reverse_pixel_rendering_it_s_craazy = pengers_width[id]-i-1;
                last_dir = -1;
            }
            if ((velocity.x > EPSILON && !x_collide) || last_dir == 1) {
                i_for_reverse_pixel_rendering_it_s_craazy = i;
                last_dir = 1;
            }
            dir = last_dir;

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

    // draw collisions box
    for (int i = 0; i < nb_collisions; i++) {
        for (int y = collisions[i].y; y < collisions[i].y + collisions[i].height; y++) {
            for (int x = collisions[i].x; x < collisions[i].x + collisions[i].width; x++) {
                if ((y/2 + x/3)%8 < 4)
                    BUFFER[y*width + x] = BLUE;
                else
                    BUFFER[y*width + x] = RED;
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
