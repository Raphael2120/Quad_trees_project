/**
 * @file controller.c
 * @brief Main application controller for handling user interactions and quadtree operations.
 */

#include "../include/controller.h"
#include "../include/quadtree.h"
#include "../include/heap.h"

#include <MLV/MLV_all.h>

#define MAX_FILENAME_LENGTH 256

/**
 * @brief Runs the main application loop.
 *
 * This function initializes and runs the main application loop, handling
 * user interactions and quadtree operations such as drawing, saving, and loading.
 *
 * @param image The image to be processed with the quadtree.
 */
void run_application(MLV_Image *image) {
    QuadtreeNode* quadtree = NULL;

    while (1) {
        draw_buttons();
        int x, y;
        MLV_wait_mouse(&x, &y);

        int button = handle_button_click(x, y);

        switch (button) {
            case 1:
                if (quadtree) free_quadtree(quadtree);
                quadtree = draw_quadtree_no_loss(image);
                break;
            case 2:
                if (quadtree) {
                    char file_path[MAX_FILENAME_LENGTH];
                    snprintf(file_path, sizeof(file_path), "img/output/quadtree.qtn");
                    save_image_quadtree_bw(file_path, quadtree);
                }
                break;
            case 3:
                if (quadtree) {
                    char file_path[MAX_FILENAME_LENGTH];
                    snprintf(file_path, sizeof(file_path), "img/output/quadtree.qtc");
                    save_image_quadtree(file_path, quadtree);
                }
                break;
            case 4:
                if (quadtree) {
                    draw_quadtree_with_loss(quadtree, image);
                }
                break;
            case 5:
                if (quadtree) {
                    char file_path[MAX_FILENAME_LENGTH];
                    snprintf(file_path, sizeof(file_path), "img/output/minimized_quadtree.qtn");
                    save_image_quadtree_bw(file_path, quadtree);
                }
                break;
            case 6:
                if (quadtree) {
                    char file_path[MAX_FILENAME_LENGTH];
                    snprintf(file_path, sizeof(file_path), "img/output/minimized_quadtree.qtc");
                    save_image_quadtree(file_path, quadtree);
                }
                break;
            case 7:
                {
                    char image_name[MAX_FILENAME_LENGTH];
                    printf("Enter the image filename: ");
                    scanf("%s", image_name);
                    
                    const char* ext = get_file_extension(image_name);

                    if (strcmp(ext, "qtn") == 0) {
                        quadtree = load_image_quadtree_bw(image_name);
                        if (quadtree) {
                            MLV_clear_window(MLV_COLOR_BLACK);
                            draw_entire_quadtree(quadtree);
                        }
                    } else if (strcmp(ext, "qtc") == 0) {
                        quadtree = load_image_quadtree(image_name);
                        if (quadtree) {
                            MLV_clear_window(MLV_COLOR_BLACK);
                            draw_entire_quadtree(quadtree);
                        }
                    } else {
                        MLV_Image *new_image = MLV_load_image(image_name);
                        if (new_image) {
                            MLV_resize_image(new_image, IMAGE_SIZE, IMAGE_SIZE);
                            MLV_clear_window(MLV_COLOR_BLACK);
                            MLV_draw_image(new_image, 0, 0);
                            MLV_actualise_window();
                            MLV_free_image(new_image);
                        } else {
                            printf("Could not load image %s\n", image_name);
                        }
                    }
                }
                break;
            default:
                break;
        }
    }

    if (quadtree) free_quadtree(quadtree);
}
