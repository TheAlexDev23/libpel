#include <png-easy/png-easy.h>

int pel_init(char* filename, int height, int width)
{
    return png_easy_create_empty(filename, height, width);
}