#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <MLV/MLV_all.h>

#include "../include/quadtree.h"
#include "../include/heap.h"

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
