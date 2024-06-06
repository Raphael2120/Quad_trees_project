#include <MLV/MLV_all.h>
#include <stdio.h>
#include "controller.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    char input_image_path[256];
    snprintf(input_image_path, sizeof(input_image_path), "img/input/%s", argv[1]);

    MLV_create_window("Quadtree", "Quadtree", IMAGE_SIZE, IMAGE_SIZE);
    MLV_Image *image = MLV_load_image(input_image_path);
    if (image == NULL) {
        printf("Could not load image %s\n", input_image_path);
        return 1;
    }
    MLV_resize_image(image, IMAGE_SIZE, IMAGE_SIZE);

    draw_menu();

    Quadtree *quadtree = NULL;
    while (1) {
        handle_event(image, &quadtree);
    }

    if (quadtree) {
        free_quadtree(quadtree);
    }
    MLV_free_image(image);
    MLV_free_window();
    return 0;
}
