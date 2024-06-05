// #include <MLV/MLV_all.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include "quadtree.h"

// MLV_Color average_color(MLV_Image *image, int x, int y, int size) {
//     int r = 0, g = 0, b = 0, a = 0, count = 0;
//     for (int i = x; i < x + size; i++) {
//         for (int j = y; j < y + size; j++) {
//             int pr, pg, pb, pa;
//             MLV_get_pixel_on_image(image, i, j, &pr, &pg, &pb, &pa);
//             r += pr;
//             g += pg;
//             b += pb;
//             a += pa;
//             count++;
//         }
//     }
//     return MLV_rgba(r / count, g / count, b / count, a / count);
// }

// Quadtree* build_quadtree(MLV_Image *image, int x, int y, int size, int depth) {
//     if (depth > MAX_DEPTH) return NULL;

//     Quadtree *node = (Quadtree*)malloc(sizeof(Quadtree));
//     node->color = average_color(image, x, y, size);
//     node->is_leaf = (size <= 1) || (depth >= MAX_DEPTH);

//     if (!node->is_leaf) {
//         int half_size = size / 2;
//         node->children[0] = build_quadtree(image, x, y, half_size, depth + 1);
//         node->children[1] = build_quadtree(image, x + half_size, y, half_size, depth + 1);
//         node->children[2] = build_quadtree(image, x, y + half_size, half_size, depth + 1);
//         node->children[3] = build_quadtree(image, x + half_size, y + half_size, half_size, depth + 1);
//     } else {
//         for (int i = 0; i < 4; i++) {
//             node->children[i] = NULL;
//         }
//     }
//     return node;
// }

// void free_quadtree(Quadtree *node) {
//     if (node == NULL) {
//         return;
//     }
//     if (!node->is_leaf) {
//         for (int i = 0; i < 4; i++) {
//             free_quadtree(node->children[i]);
//         }
//     }
//     free(node);
// }

// void save_quadtree_binary(FILE *file, Quadtree *node) {
//     if (node == NULL) {
//         return;
//     }
//     if (node->is_leaf) {
//         int is_leaf = 1;
//         fwrite(&is_leaf, sizeof(int), 1, file);
//         Uint8 gray;
//         MLV_convert_color_to_rgba(node->color, &gray, &gray, &gray, &gray);
//         int bit = gray > 128 ? 1 : 0;
//         fwrite(&bit, sizeof(int), 1, file);
//     } else {
//         int is_leaf = 0;
//         fwrite(&is_leaf, sizeof(int), 1, file);
//         for (int i = 0; i < 4; i++) {
//             save_quadtree_binary(file, node->children[i]);
//         }
//     }
// }

// Quadtree* load_quadtree_binary(FILE *file, int depth) {
//     if (depth > MAX_DEPTH) return NULL;

//     int is_leaf;
//     if (fread(&is_leaf, sizeof(int), 1, file) != 1) {
//         return NULL;
//     }

//     Quadtree *node = (Quadtree*)malloc(sizeof(Quadtree));
//     node->is_leaf = (is_leaf == 1);

//     if (node->is_leaf) {
//         int bit;
//         if (fread(&bit, sizeof(int), 1, file) != 1) {
//             free(node);
//             return NULL;
//         }
//         int gray = bit ? 255 : 0;
//         node->color = MLV_rgba(gray, gray, gray, 255);
//         for (int i = 0; i < 4; i++) {
//             node->children[i] = NULL;
//         }
//     } else {
//         for (int i = 0; i < 4; i++) {
//             node->children[i] = load_quadtree_binary(file, depth + 1);
//         }
//     }
//     return node;
// }
#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <stdio.h>
#include "quadtree.h"

MLV_Color average_color(MLV_Image *image, int x, int y, int size) {
    int r = 0, g = 0, b = 0, a = 0, count = 0;
    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            int pr, pg, pb, pa;
            MLV_get_pixel_on_image(image, i, j, &pr, &pg, &pb, &pa);
            r += pr;
            g += pg;
            b += pb;
            a += pa;
            count++;
        }
    }
    return MLV_rgba(r / count, g / count, b / count, a / count);
}

Quadtree* build_quadtree(MLV_Image *image, int x, int y, int size, int depth, int max_depth) {
    Quadtree *node = (Quadtree*)malloc(sizeof(Quadtree));
    node->color = average_color(image, x, y, size);
    node->is_leaf = (size <= 1) || (depth >= max_depth);

    if (!node->is_leaf) {
        int half_size = size / 2;
        node->children[0] = build_quadtree(image, x, y, half_size, depth + 1, max_depth);
        node->children[1] = build_quadtree(image, x + half_size, y, half_size, depth + 1, max_depth);
        node->children[2] = build_quadtree(image, x, y + half_size, half_size, depth + 1, max_depth);
        node->children[3] = build_quadtree(image, x + half_size, y + half_size, half_size, depth + 1, max_depth);
    } else {
        for (int i = 0; i < 4; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

void free_quadtree(Quadtree *node) {
    if (!node->is_leaf) {
        for (int i = 0; i < 4; i++) {
            free_quadtree(node->children[i]);
        }
    }
    free(node);
}

void save_quadtree_binary(FILE *file, Quadtree *node) {
    if (node == NULL) {
        return;
    }
    if (node->is_leaf) {
        int is_leaf = 1;
        fwrite(&is_leaf, sizeof(int), 1, file);
        Uint8 gray;
        MLV_convert_color_to_rgba(node->color, &gray, &gray, &gray, &gray);
        int bit = gray > 128 ? 1 : 0;
        fwrite(&bit, sizeof(int), 1, file);
    } else {
        int is_leaf = 0;
        fwrite(&is_leaf, sizeof(int), 1, file);
        for (int i = 0; i < 4; i++) {
            save_quadtree_binary(file, node->children[i]);
        }
    }
}

Quadtree* load_quadtree_binary(FILE *file, int depth) {
    if (depth > MAX_DEPTH) return NULL;

    int is_leaf;
    if (fread(&is_leaf, sizeof(int), 1, file) != 1) {
        return NULL;
    }

    Quadtree *node = (Quadtree*)malloc(sizeof(Quadtree));
    node->is_leaf = (is_leaf == 1);

    if (node->is_leaf) {
        int bit;
        if (fread(&bit, sizeof(int), 1, file) != 1) {
            free(node);
            return NULL;
        }
        int gray = bit ? 255 : 0;
        node->color = MLV_rgba(gray, gray, gray, 255);
        for (int i = 0; i < 4; i++) {
            node->children[i] = NULL;
        }
    } else {
        for (int i = 0; i < 4; i++) {
            node->children[i] = load_quadtree_binary(file, depth + 1);
        }
    }
    return node;
}
