/**
 * @file heap.c
 * @brief Implementation of a max-heap for handling quadtree nodes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <MLV/MLV_all.h>

#include "../include/quadtree.h"
#include "../include/heap.h"

/**
 * @brief Creates a max-heap with a given capacity.
 *
 * This function allocates memory for a max-heap and initializes its attributes.
 *
 * @param capacity The initial capacity of the heap.
 * @return A pointer to the newly created MaxHeap.
 */
MaxHeap* create_max_heap(int capacity) {
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    heap->nodes = (QuadtreeNode**)malloc(sizeof(QuadtreeNode*) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

/**
 * @brief Swaps two quadtree nodes.
 *
 * This utility function swaps the positions of two quadtree nodes in memory.
 *
 * @param a Pointer to the first quadtree node.
 * @param b Pointer to the second quadtree node.
 */
void swap(QuadtreeNode** a, QuadtreeNode** b) {
    QuadtreeNode* temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Ensures the max-heap property at a given index.
 *
 * This function fixes the max-heap property violations at a specified index by
 * comparing the node with its children and swapping if necessary.
 *
 * @param heap Pointer to the max-heap.
 * @param idx The index to heapify.
 */
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

/**
 * @brief Inserts a quadtree node into the max-heap.
 *
 * This function inserts a new quadtree node into the max-heap, expanding the heap's
 * capacity if necessary, and maintaining the max-heap property.
 *
 * @param heap Pointer to the max-heap.
 * @param node Pointer to the quadtree node to be inserted.
 */
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

/**
 * @brief Extracts the maximum element from the max-heap.
 *
 * This function removes and returns the root of the max-heap, which is the element
 * with the highest error value, and then maintains the max-heap property.
 *
 * @param heap Pointer to the max-heap.
 * @return A pointer to the extracted quadtree node, or NULL if the heap is empty.
 */
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
