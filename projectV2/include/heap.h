#ifndef HEAP_H
#define HEAP_H

#include "quadtree.h"

typedef struct {
    QuadtreeNode** nodes;
    int size;
    int capacity;
} MaxHeap;

MaxHeap* create_max_heap(int capacity);
void max_heapify(MaxHeap* heap, int idx);
void insert_max_heap(MaxHeap* heap, QuadtreeNode* node);
QuadtreeNode* extract_max(MaxHeap* heap);

#endif // HEAP_H
