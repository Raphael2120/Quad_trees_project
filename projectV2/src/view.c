/**
 * @file view.c
 * @brief Implementation of functions for drawing and handling UI elements for quadtree operations.
 */

#include <MLV/MLV_all.h>
#include "../include/view.h"

/**
 * @brief Recursively draws the quadtree nodes on the window.
 *
 * @param node The root node of the quadtree.
 */
void draw_quadtree(QuadtreeNode *node) {
    if (!node) return;
    MLV_draw_filled_rectangle(node->x, node->y, node->size, node->size, node->color);
    for (int i = 0; i < 4; i++) {
        draw_quadtree(node->children[i]);
    }
}

/**
 * @brief Draws the entire quadtree and updates the window.
 *
 * @param node The root node of the quadtree.
 */
void draw_entire_quadtree(QuadtreeNode *node) {
    draw_quadtree(node);
    MLV_actualise_window();
}

/**
 * @brief Draws the UI buttons for the application.
 */
void draw_buttons() {
    int button_width = 300;
    int button_height = 30;
    int padding = 10;
    int window_width = 845;
    int window_height = IMAGE_SIZE;

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

/**
 * @brief Handles button click events and determines which button was clicked.
 *
 * @param x The x-coordinate of the mouse click.
 * @param y The y-coordinate of the mouse click.
 * @return An integer representing the button clicked (1-7), or 0 if no button was clicked.
 */
int handle_button_click(int x, int y) {
    int button_width = 300;
    int button_height = 30;
    int padding = 10;
    int window_width = 845;
    int window_height = IMAGE_SIZE;

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

/**
 * @brief Retrieves the red component of a color.
 *
 * @param color The MLV color.
 * @return The red component of the color.
 */
Uint8 MLV_get_red(MLV_Color color) {
    Uint8 r, g, b, a;
    MLV_convert_color_to_rgba(color, &r, &g, &b, &a);
    return r;
}

/**
 * @brief Retrieves the green component of a color.
 *
 * @param color The MLV color.
 * @return The green component of the color.
 */
Uint8 MLV_get_green(MLV_Color color) {
    Uint8 r, g, b, a;
    MLV_convert_color_to_rgba(color, &r, &g, &b, &a);
    return g;
}

/**
 * @brief Retrieves the blue component of a color.
 *
 * @param color The MLV color.
 * @return The blue component of the color.
 */
Uint8 MLV_get_blue(MLV_Color color) {
    Uint8 r, g, b, a;
    MLV_convert_color_to_rgba(color, &r, &g, &b, &a);
    return b;
}

/**
 * @brief Retrieves the alpha component of a color.
 *
 * @param color The MLV color.
 * @return The alpha component of the color.
 */
Uint8 MLV_get_alpha(MLV_Color color) {
    Uint8 r, g, b, a;
    MLV_convert_color_to_rgba(color, &r, &g, &b, &a);
    return a;
}
