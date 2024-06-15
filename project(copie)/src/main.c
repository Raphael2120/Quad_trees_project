#include <MLV/MLV_all.h>
#include "quadtree.h"
#include "controller.h"
#include "view.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    MLV_create_window("Quadtree Image Approximation", NULL, IMAGE_SIZE, IMAGE_SIZE);

    MLV_Image *image = MLV_load_image(argv[1]);
    if (image == NULL) {
        printf("Could not load image %s\n", argv[1]);
        return 1;
    }
    MLV_resize_image(image, IMAGE_SIZE, IMAGE_SIZE);

    MaxHeap* heap = create_max_heap(1024);
    QuadtreeNode *quadtree = build_quadtree(image, 0, 0, IMAGE_SIZE, heap);

    // Minimize quadtree with a loss
    minimize_with_loss(quadtree);
    draw_entire_quadtree(quadtree);

    draw_entire_quadtree(quadtree);

    subdivide_and_draw(image, heap, 1000);

    

    MLV_wait_seconds(10);

    free_quadtree(quadtree);
    free(heap->nodes);
    free(heap);
    MLV_free_image(image);
    MLV_free_window();

    return 0;
}
