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

Quadtree* build_quadtree(MLV_Image *image, int x, int y, int size, int depth) {
    if (depth > MAX_DEPTH) return NULL;

    Quadtree *node = (Quadtree*)malloc(sizeof(Quadtree));
    node->color = average_color(image, x, y, size);
    node->is_leaf = (size <= 1) || (depth >= MAX_DEPTH);

    if (!node->is_leaf) {
        int half_size = size / 2;
        node->children[0] = build_quadtree(image, x, y, half_size, depth + 1);
        node->children[1] = build_quadtree(image, x + half_size, y, half_size, depth + 1);
        node->children[2] = build_quadtree(image, x, y + half_size, half_size, depth + 1);
        node->children[3] = build_quadtree(image, x + half_size, y + half_size, half_size, depth + 1);
    } else {
        for (int i = 0; i < 4; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

void free_quadtree(Quadtree *node) {
    if (node == NULL) {
        return;
    }
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

void save_quadtree_binary_rgba(FILE *file, Quadtree *node) {
    if (node == NULL) {
        return;
    }
    
    // Écrire si le noeud est une feuille
    int is_leaf = node->is_leaf ? 1 : 0;
    fwrite(&is_leaf, sizeof(int), 1, file);
    
    if (node->is_leaf) {
        // Écrire les valeurs RGBA
        Uint8 red, green, blue, alpha;
        MLV_convert_color_to_rgba(node->color, &red, &green, &blue, &alpha);
        
        unsigned char rgba[4] = {red, green, blue, alpha};
        fwrite(rgba, sizeof(unsigned char), 4, file);
        
        printf("Saving leaf node color: RGBA(%d, %d, %d, %d)\n", red, green, blue, alpha);
    } else {
        for (int i = 0; i < 4; i++) {
            save_quadtree_binary_rgba(file, node->children[i]);
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

Quadtree* load_quadtree_binary_rgba(FILE *file, int depth) {
    printf("Loading quadtree at depth %d\n", depth);
    if (depth > MAX_DEPTH) {
        printf("Exceeded max depth. Returning NULL.\n");
        return NULL;
    }

    int is_leaf;
    if (fread(&is_leaf, sizeof(int), 1, file) != 1) {
        printf("Failed to read is_leaf. Returning NULL.\n");
        return NULL;
    }

    printf("is_leaf: %d\n", is_leaf);

    Quadtree *node = (Quadtree*)malloc(sizeof(Quadtree));
    if (!node) {
        printf("Failed to allocate memory for node. Returning NULL.\n");
        return NULL;
    }
    node->is_leaf = (is_leaf == 1);

    if (node->is_leaf) {
        unsigned char rgba[4];
        if (fread(rgba, sizeof(unsigned char), 4, file) != 4) {
            printf("Failed to read RGBA values. Freeing node and returning NULL.\n");
            free(node);
            return NULL;
        }
        node->color = MLV_rgba(rgba[0], rgba[1], rgba[2], rgba[3]);
        printf("Leaf node color: RGBA(%d, %d, %d, %d)\n", rgba[0], rgba[1], rgba[2], rgba[3]);
        for (int i = 0; i < 4; i++) {
            node->children[i] = NULL;
        }
    } else {
        for (int i = 0; i < 4; i++) {
            printf("Loading child %d at depth %d\n", i, depth + 1);
            node->children[i] = load_quadtree_binary_rgba(file, depth + 1);
            if (!node->children[i]) {
                printf("Failed to load child %d. Freeing node and returning NULL.\n", i);
                free(node);
                return NULL;
            }
        }
    }
    printf("Successfully loaded node at depth %d\n", depth);
    return node;
}
