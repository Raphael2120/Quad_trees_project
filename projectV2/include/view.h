#ifndef VIEW_H
#define VIEW_H

#include "quadtree.h"

void draw_quadtree(QuadtreeNode *node);
void draw_entire_quadtree(QuadtreeNode *node);
void draw_buttons();
int handle_button_click(int x, int y);

#endif // VIEW_H
