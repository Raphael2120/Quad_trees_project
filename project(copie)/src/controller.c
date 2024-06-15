#include "controller.h"
#include "view.h"
#include "quadtree.h"

void subdivide_and_draw(MLV_Image *image, MaxHeap* heap, int threshold) {
    while (heap->size > 0) {
        QuadtreeNode* node = extract_max(heap);
        if (node->error < threshold || node->size <= 1) {
            continue;
        }

        int half_size = node->size / 2;

        node->children[0] = build_quadtree(image, node->x, node->y, half_size, heap);
        node->children[1] = build_quadtree(image, node->x + half_size, node->y, half_size, heap);
        node->children[2] = build_quadtree(image, node->x, node->y + half_size, half_size, heap);
        node->children[3] = build_quadtree(image, node->x + half_size, node->y + half_size, half_size, heap);

        draw_entire_quadtree(node);
        printf("Subdivided node at (%d, %d) with size %d\n", node->x, node->y, node->size);
        MLV_wait_milliseconds(100); // Pause to visualize the steps
    }
}

void minimize_with_loss(QuadtreeNode* root) {
    if (!root) return;

    // Recursively minimize all children first
    for (int i = 0; i < 4; i++) {
        if (root->children[i]) {
            minimize_with_loss(root->children[i]);
        }
    }

    // Compare each pair of children to find the closest pair
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (root->children[i] && root->children[j]) {
                double distance = quadtree_distance(root->children[i], root->children[j]);
                // Merge the children if they are close enough
                if (distance < INFINITY) { // Using distance < INFINITY to always attempt merge
                    free_quadtree(root->children[j]);
                    root->children[j] = NULL;
                    root->color = average_color(NULL, root->x, root->y, root->size);
                    root->error = 0.0;
                    printf("Minimized node at (%d, %d) with size %d\n", root->x, root->y, root->size);
                }
            }
        }
    }
}
