#include <MLV/MLV_all.h>
#include <stdio.h>
#include <string.h>
#include "controller.h"
#include "view.h"

void handle_event(MLV_Image *image, Quadtree **quadtree) {
    int x, y;
    char file_path_binary[256];
    char file_path_rgba[256];
    
    MLV_wait_mouse(&x, &y);
    if (x >= 10 && x <= 110 && y >= 10 && y <= 60) {
        for (int depth = 1; depth <= MAX_DEPTH; depth++) {
            printf("Building quadtree up to depth %d...\n", depth);
            *quadtree = build_quadtree(image, 0, 0, IMAGE_SIZE, 0, depth);
            MLV_clear_window(MLV_COLOR_BLACK);
            draw_quadtree(*quadtree, 0, 0, IMAGE_SIZE);
            MLV_actualise_window();
            MLV_wait_milliseconds(500);
            free_quadtree(*quadtree);
        }
        draw_menu();
    } else if (x >= 10 && x <= 110 && y >= 70 && y <= 120) {
        *quadtree = build_quadtree(image, 0, 0, IMAGE_SIZE, 0, MAX_DEPTH);
        if (*quadtree) {
            printf("Saving quadtree to binary and RGBA files...\n");
            snprintf(file_path_binary, sizeof(file_path_binary), "img/output/quadtree_binary.bin");
            snprintf(file_path_rgba, sizeof(file_path_rgba), "img/output/quadtree_rgba.bin");
            
            // Sauvegarde du fichier binaire
            FILE *file_binary = fopen(file_path_binary, "wb");
            if (file_binary) {
                save_quadtree_binary(file_binary, *quadtree);
                fclose(file_binary);
                printf("Quadtree saved to %s\n", file_path_binary);
            } else {
                printf("Failed to open binary file for saving.\n");
            }
            
            // Sauvegarde du fichier RGBA
            FILE *file_rgba = fopen(file_path_rgba, "wb");
            if (file_rgba) {
                save_quadtree_binary_rgba(file_rgba, *quadtree);
                fclose(file_rgba);
                printf("Quadtree saved to %s\n", file_path_rgba);
            } else {
                printf("Failed to open RGBA file for saving.\n");
            }
        } else {
            printf("Quadtree not built yet.\n");
        }
    // } else if (x >= 10 && x <= 110 && y >= 70 && y <= 120) {
    //     *quadtree = build_quadtree(image, 0, 0, IMAGE_SIZE, 0, MAX_DEPTH);
    //     if (*quadtree) {
    //         printf("Saving quadtree to binary file...\n");
    //         snprintf(file_path, sizeof(file_path), "img/output/quadtree.bin");
    //         FILE *file = fopen(file_path, "wb");
    //         if (file) {
    //             printf("Choose save type (1: binary, 2: binary RGBA): ");
    //             int save_type;
    //             scanf("%d", &save_type);
    //             if (save_type == 1) {
    //                 save_quadtree_binary(file, *quadtree);
    //             } else if (save_type == 2) {
    //                 save_quadtree_binary_rgba(file, *quadtree);
    //             } else {
    //                 printf("Invalid save type. Defaulting to binary.\n");
    //                 save_quadtree_binary(file, *quadtree);
    //             }
    //             fclose(file);
    //             printf("Quadtree saved to %s\n", file_path);
    //         } else {
    //             printf("Failed to open file for saving.\n");
    //         }
    //     } else {
    //         printf("Quadtree not built yet.\n");
    //     }
    } else if (x >= 10 && x <= 110 && y >= 130 && y <= 180) {
        printf("Loading quadtree from binary file...\n");
        snprintf(file_path_binary, sizeof(file_path_binary), "img/output/quadtree_binary.bin");
        FILE *file = fopen(file_path_binary, "rb");
        if (file) {
            *quadtree = load_quadtree_binary(file, 0);
            fclose(file);
            if (*quadtree != NULL) {
                MLV_clear_window(MLV_COLOR_BLACK);
                draw_quadtree(*quadtree, 0, 0, IMAGE_SIZE);
                MLV_actualise_window();
                MLV_wait_seconds(2);
                draw_menu();
                printf("Quadtree loaded from %s\n", file_path_binary);
            } else {
                printf("Failed to load quadtree from binary file.\n");
            }
        } else {
            printf("Failed to open file for loading.\n");
        }
    } else if (x >= 10 && x <= 110 && y >= 190 && y <= 240) {
        printf("Loading quadtree from binary file...\n");
        snprintf(file_path_rgba, sizeof(file_path_rgba), "img/output/quadtree_rgba.bin");
        FILE *file = fopen(file_path_rgba, "rb");
        if (file) {
            *quadtree = load_quadtree_binary_rgba(file, 0);
            fclose(file);
            if (*quadtree != NULL) {
                MLV_clear_window(MLV_COLOR_BLACK);
                draw_quadtree(*quadtree, 0, 0, IMAGE_SIZE);
                MLV_actualise_window();
                MLV_wait_seconds(2);
                draw_menu();
                printf("Quadtree loaded from %s\n", file_path_rgba);
            } else {
                printf("Failed to load quadtree from binary file.\n");
            }
        } else {
            printf("Failed to open file for loading.\n");
        }
    }
}







    // } else if (x >= 10 && x <= 110 && y >= 70 && y <= 120) {
    //     *quadtree = build_quadtree(image, 0, 0, IMAGE_SIZE, 0, MAX_DEPTH);
    //     if (*quadtree) {
    //         printf("Saving quadtree to binary file...\n");
    //         snprintf(file_path, sizeof(file_path), "img/output/quadtree.bin");
    //         FILE *file = fopen(file_path, "wb");
    //         if (file) {
    //             // save_quadtree_binary_rgba(file, *quadtree);
    //             save_quadtree_binary(file, *quadtree);
    //             fclose(file);
    //             printf("Quadtree saved to %s\n", file_path);
    //         } else {
    //             printf("Failed to open file for saving.\n");
    //         }
    //     } else {
    //         printf("Quadtree not built yet.\n");
    //     }