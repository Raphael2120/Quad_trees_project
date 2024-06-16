#ifndef QUADTREE_H
#define QUADTREE_H

#include <MLV/MLV_all.h>

#define IMAGE_SIZE 512

typedef struct QuadtreeNode {
    int x, y, size;
    MLV_Color color;
    double error;
    struct QuadtreeNode *children[4];
    int id;
} QuadtreeNode;

#include "heap.h"

MLV_Color average_color(MLV_Image *image, int x, int y, int size);
double color_distance(MLV_Color c1, MLV_Color c2);
double calculate_error(MLV_Image *image, int x, int y, int size, MLV_Color avg_color);

QuadtreeNode* create_quadtree_node(int x, int y, int size, MLV_Color color, double error);
QuadtreeNode* build_quadtree(MLV_Image *image, int x, int y, int size, MaxHeap* heap);

void free_quadtree(QuadtreeNode *node);
void minimize_with_loss(QuadtreeNode* root, MLV_Image *image);
double quadtree_distance(QuadtreeNode* t1, QuadtreeNode* t2);

QuadtreeNode* draw_quadtree_no_loss(MLV_Image *image);
void draw_quadtree_with_loss(QuadtreeNode* quadtree, MLV_Image *image);
void subdivide_and_draw(MLV_Image *image, MaxHeap* heap);

void save_quadtree_binary(FILE *file, QuadtreeNode *node);
void save_image_quadtree(const char *filename, QuadtreeNode *quadtree);

const char* get_file_extension(const char *filename);

void save_quadtree_binary_bw(FILE *file, QuadtreeNode *node);
void save_image_quadtree_bw(const char *filename, QuadtreeNode *quadtree);
void save_quadtree_as_graph(FILE *file, QuadtreeNode *node);
void save_image_quadtree_graph(const char *filename, QuadtreeNode *quadtree);

QuadtreeNode* load_quadtree_binary(FILE *file, int size, int x, int y);
QuadtreeNode* load_quadtree_binary_bw(FILE *file, int size, int x, int y);
QuadtreeNode* load_image_quadtree(const char *filename);
QuadtreeNode* load_image_quadtree_bw(const char *filename);
QuadtreeNode* load_quadtree_graph(FILE *file);

void assign_ids(QuadtreeNode *node, int *current_id);

#endif // QUADTREE_H
