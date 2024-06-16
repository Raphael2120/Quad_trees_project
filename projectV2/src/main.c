#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include "../include/controller.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    MLV_create_window("Quadtree Image Approximation", NULL, 860, IMAGE_SIZE);

    MLV_Image *image = MLV_load_image(argv[1]);
    if (image == NULL) {
        printf("Could not load image %s\n", argv[1]);
        return 1;
    }
    MLV_resize_image(image, IMAGE_SIZE, IMAGE_SIZE);

    run_application(image);

    MLV_free_image(image);
    MLV_free_window();

    return 0;
}
