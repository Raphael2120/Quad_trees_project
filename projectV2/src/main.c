/**
 * @file main.c
 * @brief Main entry point for the Quadtree Image Approximation application.
 */

#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include "../include/controller.h"

/**
 * @brief Main function to start the Quadtree Image Approximation application.
 *
 * This function initializes the application, loads the specified image, and starts
 * the main application loop. The application allows users to interact with and manipulate
 * a quadtree representation of the loaded image.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 if the application exits successfully, or 1 if there is an error.
 */
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
