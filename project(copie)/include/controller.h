#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "quadtree.h"

void subdivide_and_draw(MLV_Image *image, MaxHeap* heap, int threshold);
void minimize_with_loss(QuadtreeNode* root);

#endif // CONTROLLER_H
