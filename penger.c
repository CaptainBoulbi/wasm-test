#define t 0x00000000
#define G 0xff000000
#define g 0xff383838
#define w 0xffc0c0c0
#define y 0xff36c7fb

unsigned int penger_img[32][32] = {
    t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,B,B,t,t,t,t,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,t,t,t,t,t,B,B,B,B,g,B,B,B,t,t,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,t,t,t,t,B,B,g,g,g,g,g,g,B,B,t,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,t,t,t,B,B,g,g,g,g,g,g,g,g,B,t,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,t,t,B,B,g,g,g,g,g,g,g,g,g,B,t,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,t,t,B,g,g,g,g,g,g,g,g,g,g,B,t,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,t,B,B,g,g,g,g,g,g,g,g,g,g,g,B,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,t,B,g,g,g,g,g,B,B,B,g,g,g,g,B,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,t,B,g,g,g,g,g,g,B,B,g,g,g,g,B,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,B,B,g,g,g,g,g,g,g,g,g,g,g,g,B,B,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,B,g,g,g,g,g,g,g,g,g,g,g,B,B,B,B,B,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,B,g,g,g,g,g,g,g,g,B,B,B,B,y,y,y,y,B,t,t,t,t,t,t,
    t,t,t,t,t,t,t,t,B,g,g,g,g,g,g,g,g,B,B,y,y,y,y,y,y,y,B,t,t,t,t,t,
    t,t,t,t,t,t,t,B,B,g,g,g,g,g,g,g,g,g,B,B,B,y,y,y,y,y,B,t,t,t,t,t,
    t,t,t,t,t,t,t,B,g,g,g,g,g,g,g,g,g,g,g,g,B,B,B,y,y,y,B,t,t,t,t,t,
    t,t,t,t,t,t,t,B,g,g,g,g,g,g,g,g,g,g,g,g,g,B,B,B,B,B,t,t,t,t,t,t,
    t,t,t,t,t,t,t,B,g,g,g,g,g,g,g,g,g,B,B,B,B,B,t,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,B,g,g,g,g,g,g,B,B,B,B,w,w,B,B,B,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,B,g,g,g,g,g,g,B,w,w,w,w,w,w,w,B,t,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,B,g,g,g,g,g,B,w,w,w,w,w,w,w,w,B,B,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,B,g,g,g,g,B,w,w,w,w,w,w,w,w,w,w,B,t,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,B,g,g,g,g,B,w,w,w,w,w,w,w,w,w,w,B,B,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,B,g,g,g,g,B,w,w,w,w,w,w,w,w,w,w,w,B,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,B,g,g,g,g,B,w,w,w,w,w,w,w,w,w,w,w,B,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,B,B,g,g,g,B,w,w,w,w,w,w,w,w,w,w,w,B,t,t,t,t,t,t,t,
    t,t,t,t,t,t,t,B,B,g,g,g,B,w,w,w,w,w,w,w,w,w,w,B,y,B,B,t,t,t,t,t,
    t,t,t,t,t,B,B,B,y,B,g,g,B,w,w,w,w,w,w,w,w,B,B,B,y,y,B,B,t,t,t,t,
    t,t,t,t,B,B,y,y,y,y,B,B,B,B,B,B,B,B,B,B,B,y,y,y,y,y,y,B,B,t,t,t,
    t,t,t,t,B,B,y,y,y,y,y,y,B,t,t,t,t,t,t,t,t,B,B,y,y,y,y,y,B,t,t,t,
    t,t,t,t,t,B,B,B,y,y,B,B,B,t,t,t,t,t,t,t,t,t,B,B,B,y,y,B,B,t,t,t,
    t,t,t,t,t,t,t,B,B,B,B,t,t,t,t,t,t,t,t,t,t,t,t,t,B,B,B,B,t,t,t,t,
};
