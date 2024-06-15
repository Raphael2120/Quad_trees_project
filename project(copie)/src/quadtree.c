#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <MLV/MLV_all.h>
#include "quadtree.h"

MaxHeap* create_max_heap(int capacity) {
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    heap->nodes = (QuadtreeNode**)malloc(sizeof(QuadtreeNode*) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(QuadtreeNode** a, QuadtreeNode** b) {
    QuadtreeNode* temp = *a;
    *a = *b;
    *b = temp;
}

void max_heapify(MaxHeap* heap, int idx) {
    int largest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left]->error > heap->nodes[largest]->error)
        largest = left;

    if (right < heap->size && heap->nodes[right]->error > heap->nodes[largest]->error)
        largest = right;

    if (largest != idx) {
        swap(&heap->nodes[largest], &heap->nodes[idx]);
        max_heapify(heap, largest);
    }
}

void insert_max_heap(MaxHeap* heap, QuadtreeNode* node) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->nodes = (QuadtreeNode**)realloc(heap->nodes, heap->capacity * sizeof(QuadtreeNode*));
    }
    heap->nodes[heap->size] = node;
    int i = heap->size;
    heap->size++;

    while (i != 0 && heap->nodes[(i - 1) / 2]->error < heap->nodes[i]->error) {
        swap(&heap->nodes[(i - 1) / 2], &heap->nodes[i]);
        i = (i - 1) / 2;
    }
}

QuadtreeNode* extract_max(MaxHeap* heap) {
    if (heap->size <= 0) return NULL;
    if (heap->size == 1) {
        heap->size--;
        return heap->nodes[0];
    }

    QuadtreeNode* root = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    max_heapify(heap, 0);

    return root;
}

MLV_Color average_color(MLV_Image *image, int x, int y, int size) {
    int r = 0, g = 0, b = 0, a = 0, count = 0;
    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            int pr, pg, pb, pa;
            MLV_get_pixel_on_image(image, i, j, &pr, &pg, &pb, &pa);
            r += pr;
            g += pg;
            b += pb;
            a += pa;
            count++;
        }
    }
    return MLV_rgba(r / count, g / count, b / count, a / count);
}

double calculate_error(MLV_Image *image, int x, int y, int size, MLV_Color avg_color) {
    double error = 0.0;
    Uint8 ar, ag, ab, aa;
    MLV_convert_color_to_rgba(avg_color, &ar, &ag, &ab, &aa);

    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            int pr, pg, pb, pa;
            MLV_get_pixel_on_image(image, i, j, &pr, &pg, &pb, &pa);

            int dr = pr - ar;
            int dg = pg - ag;
            int db = pb - ab;
            int da = pa - aa;
            error += sqrt(dr * dr + dg * dg + db * db + da * da);
        }
    }
    return error;
}

QuadtreeNode* create_quadtree_node(int x, int y, int size, MLV_Color color, double error) {
    QuadtreeNode* node = (QuadtreeNode*)malloc(sizeof(QuadtreeNode));
    node->x = x;
    node->y = y;
    node->size = size;
    node->color = color;
    node->error = error;
    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
    }
    return node;
}

QuadtreeNode* build_quadtree(MLV_Image *image, int x, int y, int size, MaxHeap* heap) {
    MLV_Color avg_color = average_color(image, x, y, size);
    double error = calculate_error(image, x, y, size, avg_color);
    QuadtreeNode *node = create_quadtree_node(x, y, size, avg_color, error);

    Uint8 r, g, b, a;
    MLV_convert_color_to_rgba(avg_color, &r, &g, &b, &a);
    printf("Created node at (%d, %d) with size %d, avg color (%d, %d, %d, %d), error %.2f\n",
           x, y, size, r, g, b, a, error);

    insert_max_heap(heap, node);
    return node;
}

void free_quadtree(QuadtreeNode *node) {
    if (!node) return;
    for (int i = 0; i < 4; i++) {
        free_quadtree(node->children[i]);
    }
    free(node);
}

double color_distance(MLV_Color c1, MLV_Color c2) {
    Uint8 r1, g1, b1, a1;
    Uint8 r2, g2, b2, a2;
    MLV_convert_color_to_rgba(c1, &r1, &g1, &b1, &a1);
    MLV_convert_color_to_rgba(c2, &r2, &g2, &b2, &a2);

    return sqrt((r1 - r2) * (r1 - r2) +
                (g1 - g2) * (g1 - g2) +
                (b1 - b2) * (b1 - b2) +
                (a1 - a2) * (a1 - a2));
}

double quadtree_distance(QuadtreeNode* t1, QuadtreeNode* t2) {
    if (t1 == NULL && t2 == NULL) return 0.0;
    if (t1 == NULL || t2 == NULL) return INFINITY;
    
    if (t1->children[0] == NULL && t2->children[0] == NULL) {
        return color_distance(t1->color, t2->color);
    } else {
        double distance = 0.0;
        for (int i = 0; i < 4; i++) {
            distance += quadtree_distance(t1->children[i], t2->children[i]);
        }
        return distance / 4.0;
    }
}
