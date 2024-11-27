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

    if (argc < 3) { // no id provided
        char *last_slash = strrchr(img_path, '/');
        char extracted[50];

        if (last_slash != NULL) {
            // Extract the part after the last '/'
            strcpy(extracted, last_slash + 1);
            
            // Remove the '.png' extension
            char *dot = strrchr(extracted, '.');
            if (dot != NULL) {
                *dot = '\0'; // Null-terminate the string at the '.'
            }
        }

        printf("int %s_height = %d;\n", extracted, height);
        printf("int %s_width = %d;\n", extracted, width);
        printf("unsigned int %s_img[%d][%d] = {\n", extracted, height, width);

        for (int y = 0; y < height; y++) {
            printf("    ");
            for (int i = 0; i < width; i++) {
                printf("0x%08X,", data[y*width + i]);
            }
            printf("\n");
        }

        printf("};\n");
    } else { // id provided
        int id = atoi(argv[2]);

        printf("unsigned int penger_img_%d[] = {\n", id);

        for (int y = 0; y < height; y++) {
            printf("    ");
            for (int i = 0; i < width; i++) {
                printf("0x%08X,", data[y*width + i]);
            }
            printf("\n");
        }

        printf("};\n");

        printf("void penger_init_%d(void) {\n", id);
        printf("    pengers_height[%d] = %d;\n", id, height);
        printf("    pengers_width[%d] = %d;\n", id, width);
        printf("    pengers_img[%d] = penger_img_%d;\n", id, id);
        printf("}\n");
    }
}

