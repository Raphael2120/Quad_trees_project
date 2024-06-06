#include <MLV/MLV_all.h>
#include "view.h"

void draw_menu() {
    MLV_draw_filled_rectangle(10, 10, 100, 50, MLV_COLOR_GREEN);
    MLV_draw_text(20, 25, "Build Quadtree", MLV_COLOR_BLACK);
    MLV_draw_filled_rectangle(10, 70, 100, 50, MLV_COLOR_BLUE);
    MLV_draw_text(20, 85, "Save Binary", MLV_COLOR_BLACK);
    MLV_draw_filled_rectangle(10, 130, 100, 50, MLV_COLOR_RED);
    MLV_draw_text(20, 145, "Load Binary", MLV_COLOR_BLACK);
    MLV_actualise_window();
}

void draw_quadtree(Quadtree *node, int x, int y, int size) {
    if (node == NULL) {
        return;
    }
    if (node->is_leaf) {
        MLV_draw_filled_rectangle(x, y, size, size, node->color);
    } else {
        int half_size = size / 2;
        draw_quadtree(node->children[0], x, y, half_size);
        draw_quadtree(node->children[1], x + half_size, y, half_size);
        draw_quadtree(node->children[2], x, y + half_size, half_size);
        draw_quadtree(node->children[3], x + half_size, y + half_size, half_size);
    }
}
