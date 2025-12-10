
#include <MLV/MLV_all.h>
#include "../include/view.h"
#include "../include/config.h"

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

void draw_buttons() {
    int button_width = BUTTON_WIDTH;
    int button_height = BUTTON_HEIGHT;
    int padding = BUTTON_PADDING;
    int window_width = WINDOW_WIDTH - 15; /* Adjust for actual window size */
    int window_height = DEFAULT_IMAGE_SIZE;

    int x = window_width - button_width - padding;
    int y = (window_height / 2) - (7 * button_height / 2) - (7 * padding / 2);

    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 1: Construct Quadtree", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 2: Save as QTN (BW)", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 2: Save as QTC (Color)", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 3: Minimize Quadtree", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 3: Save Minimized QTN (BW)", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 3: Save Minimized QTC (Color)", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 3: Load Image", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);

    MLV_actualise_window();
}

int handle_button_click(int x, int y) {
    int button_width = BUTTON_WIDTH;
    int button_height = BUTTON_HEIGHT;
    int padding = BUTTON_PADDING;
    int window_width = WINDOW_WIDTH - 15; /* Adjust for actual window size */
    int window_height = DEFAULT_IMAGE_SIZE;

    int bx = window_width - button_width - padding;
    int by = (window_height / 2) - (7 * button_height / 2) - (7 * padding / 2);

    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 1; // Construct Quadtree
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 2; // Save as QTN (BW)
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 3; // Save as QTC (Color)
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 4; // Minimize Quadtree
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 5; // Save Minimized QTN (BW)
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 6; // Save Minimized QTC (Color)
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 7; // Load Image

    return 0;
}

/* Color extraction functions moved to utils.c */
