#include <MLV/MLV_all.h>
#include "quadtree.h"

void draw_quadtree(QuadtreeNode *node) {
    if (!node) return;
    MLV_draw_filled_rectangle(node->x, node->y, node->size, node->size, node->color);
    for (int i = 0; i < 4; i++) {
        draw_quadtree(node->children[i]);
    }
}

void draw_entire_quadtree(QuadtreeNode *node) {
    draw_quadtree(node);
    MLV_actualise_window();
}
