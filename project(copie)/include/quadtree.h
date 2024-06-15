#ifndef QUADTREE_H
#define QUADTREE_H

#include <MLV/MLV_all.h>
#include <math.h> // Inclure math.h pour INFINITY

#define IMAGE_SIZE 512

typedef struct QuadtreeNode {
    int x, y, size;
    MLV_Color color;
    double error;
    struct QuadtreeNode *children[4];
} QuadtreeNode;

typedef struct {
    QuadtreeNode** nodes;
    int size;
    int capacity;
} MaxHeap;

MaxHeap* create_max_heap(int capacity);
void swap(QuadtreeNode** a, QuadtreeNode** b);
void max_heapify(MaxHeap* heap, int idx);
void insert_max_heap(MaxHeap* heap, QuadtreeNode* node);
QuadtreeNode* extract_max(MaxHeap* heap);
MLV_Color average_color(MLV_Image *image, int x, int y, int size);
double calculate_error(MLV_Image *image, int x, int y, int size, MLV_Color avg_color);
QuadtreeNode* create_quadtree_node(int x, int y, int size, MLV_Color color, double error);
QuadtreeNode* build_quadtree(MLV_Image *image, int x, int y, int size, MaxHeap* heap);
void free_quadtree(QuadtreeNode *node);
double quadtree_distance(QuadtreeNode* t1, QuadtreeNode* t2); // Ajoutez cette déclaration

#endif // QUADTREE_H
