#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <MLV/MLV_all.h>
#include "quadtree.h"
#include "view.h"

void handle_event(MLV_Image *image, Quadtree **quadtree);

#endif // CONTROLLER_H
