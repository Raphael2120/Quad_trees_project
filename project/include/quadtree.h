#ifndef QUADTREE_H
#define QUADTREE_H

#include <MLV/MLV_all.h>

#define IMAGE_SIZE 512
#define MAX_DEPTH 5

typedef struct Quadtree {
    int is_leaf;
    MLV_Color color;
    struct Quadtree *children[4];
} Quadtree;

MLV_Color average_color(MLV_Image *image, int x, int y, int size);
Quadtree* build_quadtree(MLV_Image *image, int x, int y, int size, int depth);
void free_quadtree(Quadtree *node);
void save_quadtree_binary(FILE *file, Quadtree *node);
void save_quadtree_binary_rgba(FILE *file, Quadtree *node);
Quadtree* load_quadtree_binary(FILE *file, int depth);
Quadtree* load_quadtree_binary_rgba(FILE *file, int depth);

#endif // QUADTREE_H

