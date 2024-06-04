// #include <MLV/MLV_all.h>
// #include <stdio.h>
// #include <string.h>
// #include "controller.h"
// #include "view.h"

// void handle_event(MLV_Image *image, Quadtree **quadtree) {
//     int x, y;
//     char file_path[256];
    
//     MLV_wait_mouse(&x, &y);
//     if (x >= 10 && x <= 110 && y >= 10 && y <= 60) {
//         printf("Building quadtree up to max depth %d...\n", MAX_DEPTH);
//         *quadtree = build_quadtree(image, 0, 0, IMAGE_SIZE, 0);
//         MLV_clear_window(MLV_COLOR_BLACK);
//         draw_quadtree(*quadtree, 0, 0, IMAGE_SIZE);
//         MLV_actualise_window();
//         MLV_wait_seconds(2);
//         draw_menu();
//     } else if (x >= 10 && x <= 110 && y >= 70 && y <= 120) {
//         if (*quadtree) {
//             printf("Saving quadtree to binary file...\n");
//             snprintf(file_path, sizeof(file_path), "img/output/quadtree.bin");
//             FILE *file = fopen(file_path, "wb");
//             if (file) {
//                 save_quadtree_binary(file, *quadtree);
//                 fclose(file);
//                 printf("Quadtree saved to %s\n", file_path);
//             } else {
//                 printf("Failed to open file for saving.\n");
//             }
//         } else {
//             printf("Quadtree not built yet.\n");
//         }
//     } else if (x >= 10 && x <= 110 && y >= 130 && y <= 180) {
//         printf("Loading quadtree from binary file...\n");
//         snprintf(file_path, sizeof(file_path), "img/output/quadtree.bin");
//         FILE *file = fopen(file_path, "rb");
//         if (file) {
//             *quadtree = load_quadtree_binary(file, 0);
//             fclose(file);
//             if (*quadtree != NULL) {
//                 MLV_clear_window(MLV_COLOR_BLACK);
//                 draw_quadtree(*quadtree, 0, 0, IMAGE_SIZE);
//                 MLV_actualise_window();
//                 MLV_wait_seconds(2);
//                 draw_menu();
//                 printf("Quadtree loaded from %s\n", file_path);
//             } else {
//                 printf("Failed to load quadtree from binary file.\n");
//             }
//         } else {
//             printf("Failed to open file for loading.\n");
//         }
//     }
// }


#include <MLV/MLV_all.h>
#include <stdio.h>
#include <string.h>
#include "controller.h"
#include "view.h"

void handle_event(MLV_Image *image, Quadtree **quadtree) {
    int x, y;
    char file_path[256];

    MLV_wait_mouse(&x, &y);
    if (x >= 10 && x <= 110 && y >= 10 && y <= 60) {
        for (int depth = 1; depth <= MAX_DEPTH; depth++) {
            printf("Building quadtree up to depth %d...\n", depth);
            *quadtree = build_quadtree(image, 0, 0, IMAGE_SIZE, 0, depth);
            MLV_clear_window(MLV_COLOR_BLACK);
            draw_quadtree(*quadtree, 0, 0, IMAGE_SIZE);
            MLV_actualise_window();
            MLV_wait_milliseconds(500); // Attendre 500ms pour voir chaque niveau
            free_quadtree(*quadtree);
        }
        draw_menu();
    } else if (x >= 10 && x <= 110 && y >= 70 && y <= 120) {
        if (*quadtree) {
            printf("Saving quadtree to binary file...\n");
            snprintf(file_path, sizeof(file_path), "img/output/quadtree.bin");
            FILE *file = fopen(file_path, "wb");
            if (file) {
                save_quadtree_binary(file, *quadtree);
                fclose(file);
                printf("Quadtree saved to %s\n", file_path);
            } else {
                printf("Failed to open file for saving.\n");
            }
        } else {
            printf("Quadtree not built yet.\n");
        }
    } else if (x >= 10 && x <= 110 && y >= 130 && y <= 180) {
        printf("Loading quadtree from binary file...\n");
        snprintf(file_path, sizeof(file_path), "img/output/quadtree.bin");
        FILE *file = fopen(file_path, "rb");
        if (file) {
            *quadtree = load_quadtree_binary(file, 0);
            fclose(file);
            if (*quadtree != NULL) {
                MLV_clear_window(MLV_COLOR_BLACK);
                draw_quadtree(*quadtree, 0, 0, IMAGE_SIZE);
                MLV_actualise_window();
                MLV_wait_seconds(2);
                draw_menu();
                printf("Quadtree loaded from %s\n", file_path);
            } else {
                printf("Failed to load quadtree from binary file.\n");
            }
        } else {
            printf("Failed to open file for loading.\n");
        }
    }
}
