#define T 0x00000000
#define B 0xff000000
#define G 0xff383838
#define W 0xffc0c0c0
#define Y 0xff36c7fb

unsigned int penger_img[32][32] = {
    T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, 
    T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, B, B, T, T, T, T, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, T, T, T, T, T, B, B, B, B, G, B, B, B, T, T, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, T, T, T, T, B, B, G, G, G, G, G, G, B, B, T, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, T, T, T, B, B, G, G, G, G, G, G, G, G, B, T, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, T, T, B, B, G, G, G, G, G, G, G, G, G, B, T, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, T, T, B, G, G, G, G, G, G, G, G, G, G, B, T, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, T, B, B, G, G, G, G, G, G, G, G, G, G, G, B, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, T, B, G, G, G, G, G, B, B, B, G, G, G, G, B, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, T, B, G, G, G, G, G, G, B, B, G, G, G, G, B, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, B, B, G, G, G, G, G, G, G, G, G, G, G, G, B, B, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, B, G, G, G, G, G, G, G, G, G, G, G, B, B, B, B, B, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, B, G, G, G, G, G, G, G, G, B, B, B, B, Y, Y, Y, Y, B, T, T, T, T, T, T,
    T, T, T, T, T, T, T, T, B, G, G, G, G, G, G, G, G, B, B, Y, Y, Y, Y, Y, Y, Y, B, T, T, T, T, T,
    T, T, T, T, T, T, T, B, B, G, G, G, G, G, G, G, G, G, B, B, B, Y, Y, Y, Y, Y, B, T, T, T, T, T,
    T, T, T, T, T, T, T, B, G, G, G, G, G, G, G, G, G, G, G, G, B, B, B, Y, Y, Y, B, T, T, T, T, T,
    T, T, T, T, T, T, T, B, G, G, G, G, G, G, G, G, G, G, G, G, G, B, B, B, B, B, T, T, T, T, T, T,
    T, T, T, T, T, T, T, B, G, G, G, G, G, G, G, G, G, B, B, B, B, B, T, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, B, G, G, G, G, G, G, B, B, B, B, W, W, B, B, B, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, B, G, G, G, G, G, G, B, W, W, W, W, W, W, W, B, T, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, B, G, G, G, G, G, B, W, W, W, W, W, W, W, W, B, B, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, B, G, G, G, G, B, W, W, W, W, W, W, W, W, W, W, B, T, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, B, G, G, G, G, B, W, W, W, W, W, W, W, W, W, W, B, B, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, B, G, G, G, G, B, W, W, W, W, W, W, W, W, W, W, W, B, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, B, G, G, G, G, B, W, W, W, W, W, W, W, W, W, W, W, B, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, B, B, G, G, G, B, W, W, W, W, W, W, W, W, W, W, W, B, T, T, T, T, T, T, T,
    T, T, T, T, T, T, T, B, B, G, G, G, B, W, W, W, W, W, W, W, W, W, W, B, Y, B, B, T, T, T, T, T,
    T, T, T, T, T, B, B, B, Y, B, G, G, B, W, W, W, W, W, W, W, W, B, B, B, Y, Y, B, B, T, T, T, T,
    T, T, T, T, B, B, Y, Y, Y, Y, B, B, B, B, B, B, B, B, B, B, B, Y, Y, Y, Y, Y, Y, B, B, T, T, T,
    T, T, T, T, B, B, Y, Y, Y, Y, Y, Y, B, T, T, T, T, T, T, T, T, B, B, Y, Y, Y, Y, Y, B, T, T, T,
    T, T, T, T, T, B, B, B, Y, Y, B, B, B, T, T, T, T, T, T, T, T, T, B, B, B, Y, Y, B, B, T, T, T,
    T, T, T, T, T, T, T, B, B, B, B, T, T, T, T, T, T, T, T, T, T, T, T, T, B, B, B, B, T, T, T, T,
};
