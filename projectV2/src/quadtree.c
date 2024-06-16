#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <MLV/MLV_all.h>

#include "../include/quadtree.h"
#include "../include/heap.h"
#include "../include/view.h"

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

double calculate_error(MLV_Image *image, int x, int y, int size, MLV_Color avg_color) {
    double error = 0.0;
    Uint8 ar, ag, ab, aa;
    MLV_convert_color_to_rgba(avg_color, &ar, &ag, &ab, &aa);

    for (int i = x; i < x + size; i++) {
        for        (int j = y; j < y + size; j++) {
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
    node->id = -1; // Initialize id to -1
    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
    }
    return node;
}

QuadtreeNode* build_quadtree(MLV_Image *image, int x, int y, int size, MaxHeap* heap) {
    MLV_Color avg_color = average_color(image, x, y, size);
    double error = calculate_error(image, x, y, size, avg_color);
    QuadtreeNode *node = create_quadtree_node(x, y, size, avg_color, error);

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

void minimize_with_loss(QuadtreeNode* root, MLV_Image *image) {
    if (!root) return;

    for (int i = 0; i < 4; i++) {
        if (root->children[i]) {
            minimize_with_loss(root->children[i], image);
        }
    }

    double min_distance = INFINITY;
    int merge_index1 = -1, merge_index2 = -1;

    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (root->children[i] && root->children[j]) {
                double distance = quadtree_distance(root->children[i], root->children[j]);
                if (distance < min_distance) {
                    min_distance = distance;
                    merge_index1 = i;
                    merge_index2 = j;
                }
            }
        }
    }

    if (merge_index1 != -1 && merge_index2 != -1 && min_distance < 25.0) { // Adjust threshold as needed
        free_quadtree(root->children[merge_index2]);
        root->children[merge_index2] = NULL;
        root->color = average_color(image, root->x, root->y, root->size);
        root->error = 0.0;
        printf("Minimized node at (%d, %d) with size %d\n", root->x, root->y, root->size);
    }
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

QuadtreeNode* draw_quadtree_no_loss(MLV_Image *image) {
    MaxHeap* heap = create_max_heap(1024);
    QuadtreeNode *quadtree = build_quadtree(image, 0, 0, IMAGE_SIZE, heap);
    subdivide_and_draw(image, heap);
    free(heap->nodes);
    free(heap);
    return quadtree;
}

void draw_quadtree_with_loss(QuadtreeNode* quadtree, MLV_Image *image) {
    minimize_with_loss(quadtree, image);
    MLV_clear_window(MLV_COLOR_BLACK); // Clear the window before drawing the minimized quadtree
    draw_entire_quadtree(quadtree);
}

void subdivide_and_draw(MLV_Image *image, MaxHeap* heap) {
    while (heap->size > 0) {
        QuadtreeNode* node = extract_max(heap);
        if (node->size <= 1) {
            continue;
        }

        int half_size = node->size / 2;

        node->children[0] = build_quadtree(image, node->x, node->y, half_size, heap);
        node->children[1] = build_quadtree(image, node->x + half_size, node->y, half_size, heap);
        node->children[2] = build_quadtree(image, node->x, node->y + half_size, half_size, heap);
        node->children[3] = build_quadtree(image, node->x + half_size, node->y + half_size, half_size, heap);

        draw_entire_quadtree(node);
        printf("Subdivided node at (%d, %d) with size %d\n", node->x, node->y, node->size);
    }
}

void save_quadtree_binary(FILE *file, QuadtreeNode *node) {
    if (!node) return;

    if (node->children[0] == NULL) {
        // Leaf node
        int is_leaf = 1;
        fwrite(&is_leaf, sizeof(int), 1, file);
        Uint8 r, g, b, a;
        MLV_convert_color_to_rgba(node->color, &r, &g, &b, &a);
        fwrite(&r, sizeof(Uint8), 1, file);
        fwrite(&g, sizeof(Uint8), 1, file);
        fwrite(&b, sizeof(Uint8), 1, file);
        fwrite(&a, sizeof(Uint8), 1, file);
    } else {
        // Internal node
        int is_leaf = 0;
        fwrite(&is_leaf, sizeof(int), 1, file);
        for (int i = 0; i < 4; i++) {
            save_quadtree_binary(file, node->children[i]);
        }
    }
}

void save_image_quadtree(const char *filename, QuadtreeNode *quadtree) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Could not open file for writing: %s\n", filename);
        return;
    }
    save_quadtree_binary(file, quadtree);
    fclose(file);
}

const char* get_file_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void save_quadtree_binary_bw(FILE *file, QuadtreeNode *node) {
    if (!node) return;

    if (node->children[0] == NULL) {
        // Leaf node
        int is_leaf = 1;
        fwrite(&is_leaf, sizeof(int), 1, file);
        Uint8 r, g, b, a;
        MLV_convert_color_to_rgba(node->color, &r, &g, &b, &a);
        Uint8 gray = (r + g + b) / 3;
        fwrite(&gray, sizeof(Uint8), 1, file);
    } else {
        // Internal node
        int is_leaf = 0;
        fwrite(&is_leaf, sizeof(int), 1, file);
        for (int i = 0; i < 4; i++) {
            save_quadtree_binary_bw(file, node->children[i]);
        }
    }
}

void save_image_quadtree_bw(const char *filename, QuadtreeNode *quadtree) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Could not open file for writing: %s\n", filename);
        return;
    }
    save_quadtree_binary_bw(file, quadtree);
    fclose(file);
}

// Fonction pour sauvegarder le quadtree en tant que graphe minimisé
void save_quadtree_as_graph(FILE *file, QuadtreeNode *node) {
    if (!node) return;
    
    if (node->children[0] == NULL) {
        // Leaf node
        fprintf(file, "%df %d %d %d %d\n", node->id, MLV_get_red(node->color), MLV_get_green(node->color), MLV_get_blue(node->color), MLV_get_alpha(node->color));
    } else {
        // Internal node
        fprintf(file, "%d %d %d %d %d\n", node->id, node->children[0] ? node->children[0]->id : -1,
                                           node->children[1] ? node->children[1]->id : -1,
                                           node->children[2] ? node->children[2]->id : -1,
                                           node->children[3] ? node->children[3]->id : -1);
        for (int i = 0; i < 4; i++) {
            save_quadtree_as_graph(file, node->children[i]);
        }
    }
}

void save_image_quadtree_graph(const char *filename, QuadtreeNode *quadtree) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Could not open file for writing: %s\n", filename);
        return;
    }
    int current_id = 0;
    assign_ids(quadtree, &current_id);
    save_quadtree_as_graph(file, quadtree);
    fclose(file);
}

QuadtreeNode* load_quadtree_binary(FILE *file, int size, int x, int y) {
    int is_leaf;
    if (fread(&is_leaf, sizeof(int), 1, file) != 1) {
        return NULL;
    }

    if (is_leaf) {
        // Leaf node
        Uint8 r, g, b, a;
        fread(&r, sizeof(Uint8), 1, file);
        fread(&g, sizeof(Uint8), 1, file);
        fread(&b, sizeof(Uint8), 1, file);
        fread(&a, sizeof(Uint8), 1, file);
        MLV_Color color = MLV_rgba(r, g, b, a);
        return create_quadtree_node(x, y, size, color, 0.0);
    } else {
        // Internal node
        QuadtreeNode *node = create_quadtree_node(x, y, size, MLV_COLOR_BLACK, 0.0);
        int half_size = size / 2;
        node->children[0] = load_quadtree_binary(file, half_size, x, y);
        node->children[1] = load_quadtree_binary(file, half_size, x + half_size, y);
        node->children[2] = load_quadtree_binary(file, half_size, x, y + half_size);
        node->children[3] = load_quadtree_binary(file, half_size, x + half_size, y + half_size);
        return node;
    }
}

QuadtreeNode* load_quadtree_binary_bw(FILE *file, int size, int x, int y) {
    int is_leaf;
    if (fread(&is_leaf, sizeof(int), 1, file) != 1) {
        return NULL;
    }

    if (is_leaf) {
        // Leaf node
        Uint8 gray;
        fread(&gray, sizeof(Uint8), 1, file);
        MLV_Color color = MLV_rgba(gray, gray, gray, 255);
        return create_quadtree_node(x, y, size, color, 0.0);
    } else {
        // Internal node
        QuadtreeNode *node = create_quadtree_node(x, y, size, MLV_COLOR_BLACK, 0.0);
        int half_size = size / 2;
        node->children[0] = load_quadtree_binary_bw(file, half_size, x, y);
        node->children[1] = load_quadtree_binary_bw(file, half_size, x + half_size, y);
        node->children[2] = load_quadtree_binary_bw(file, half_size, x, y + half_size);
        node->children[3] = load_quadtree_binary_bw(file, half_size, x + half_size, y + half_size);
        return node;
    }
}

QuadtreeNode* load_image_quadtree(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Could not open file for reading: %s\n", filename);
        return NULL;
    }
    QuadtreeNode *quadtree = load_quadtree_binary(file, IMAGE_SIZE, 0, 0);
    fclose(file);
    return quadtree;
}

QuadtreeNode* load_image_quadtree_bw(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Could not open file for reading: %s\n", filename);
        return NULL;
    }
    QuadtreeNode *quadtree = load_quadtree_binary_bw(file, IMAGE_SIZE, 0, 0);
    fclose(file);
    return quadtree;
}

QuadtreeNode* load_quadtree_graph(FILE *file) {
    int id, c0, c1, c2, c3;
    int capacity = 10000;
    QuadtreeNode** nodes = (QuadtreeNode**)malloc(capacity * sizeof(QuadtreeNode*));
    int node_count = 0;

    while (fscanf(file, "%d", &id) != EOF) {
        if (id >= capacity) {
            capacity *= 2;
            nodes = (QuadtreeNode**)realloc(nodes, capacity * sizeof(QuadtreeNode*));
        }
        
        char c;
        fscanf(file, "%c", &c);
        if (c == 'f') {
            int r, g, b, a;
            fscanf(file, "%d %d %d %d", &r, &g, &b, &a);
            nodes[id] = create_quadtree_node(0, 0, 0, MLV_rgba(r, g, b, a), 0.0);
        } else {
            ungetc(c, file);
            fscanf(file, "%d %d %d %d", &c0, &c1, &c2, &c3);
            nodes[id] = create_quadtree_node(0, 0, 0, MLV_COLOR_BLACK, 0.0);
            nodes[id]->children[0] = c0 == -1 ? NULL : nodes[c0];
            nodes[id]->children[1] = c1 == -1 ? NULL : nodes[c1];
            nodes[id]->children[2] = c2 == -1 ? NULL : nodes[c2];
            nodes[id]->children[3] = c3 == -1 ? NULL : nodes[c3];
        }
        nodes[id]->id = id;
        node_count++;
    }

    QuadtreeNode* root = nodes[0];
    free(nodes);
    return root;
}

void assign_ids(QuadtreeNode *node, int *current_id) {
    if (!node) return;

    if (node->children[0] == NULL) {
        node->id = (*current_id)++;
    } else {
        node->id = (*current_id)++;
        for (int i = 0; i < 4; i++) {
            assign_ids(node->children[i], current_id);
        }
    }
}