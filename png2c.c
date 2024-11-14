#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, const char *argv[])
{
    if (argc < 2) {
        fprintf(stdout, "usage: %s <image path>\n", argv[0]);
        fprintf(stderr, "ERROR: no image provided\n");
        return 1;
    }

    const char *img_path = argv[1];

    int width, height, n;
    unsigned int *data = (unsigned int*)stbi_load(img_path, &width, &height, &n, 4);

    if (n != 4) {
        fprintf(stderr, "ERROR: n components is not 4\n");
        return 1;
    }

    int file_name_len = strlen(img_path) - strlen(".png");
    
    printf("int %.*s_height = %d;\n", file_name_len, img_path, height);
    printf("int %.*s_width = %d;\n", file_name_len, img_path, width);
    printf("unsigned int %.*s_img[%d][%d] = {\n", file_name_len, img_path, height, width);

    for (int y = 0; y < height; y++) {
        printf("    ");
        for (int i = 0; i < width; i++) {
            printf("0x%08X,", data[y*width + i]);
        }
        printf("\n");
    }

    printf("};\n");
}

