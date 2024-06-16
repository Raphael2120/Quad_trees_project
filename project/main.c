#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <MLV/MLV_all.h>

#define IMAGE_SIZE 512
#define MAX_FILENAME_LENGTH 256

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

void minimize_with_loss(QuadtreeNode* root, MLV_Image *image) {
    if (!root) return;

    // Recursively minimize all children first
    for (int i = 0; i < 4; i++) {
        if (root->children[i]) {
            minimize_with_loss(root->children[i], image);
        }
    }

    // Compare each pair of children to find the closest pair
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (root->children[i] && root->children[j]) {
                double distance = quadtree_distance(root->children[i], root->children[j]);
                // Merge the children if they are close enough
                if (distance < 25.0) { // Using a threshold for the color distance
                    free_quadtree(root->children[j]);
                    root->children[j] = NULL;
                    root->color = average_color(image, root->x, root->y, root->size);
                    root->error = 0.0;
                    printf("Minimized node at (%d, %d) with size %d\n", root->x, root->y, root->size);
                }
            }
        }
    }
}

void draw_quadtree(QuadtreeNode *node) {
    if (!node) return;
    MLV_draw_filled_rectangle(node->x, node->y, node->size, node->size, node->color);
    for (int i = 0; i < 4; i++) {
        draw_quadtree(node->children[i]);
    }
}

void draw_entire_quadtree(QuadtreeNode *node) {
    draw_quadtree(node);
    MLV_actualise_window();
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

const char* get_file_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
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

void draw_buttons() {
    int button_width = 300;
    int button_height = 30;
    int padding = 10;
    int x = padding;
    int y = padding;

    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 1: Construct Quadtree", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 2: Save as QTN (BW)", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 2: Save as QTC (Color)", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 3: Minimize Quadtree", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 3: Save Minimized QTN (BW)", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 3: Save Minimized QTC (Color)", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    y += button_height + padding;
    MLV_draw_text_box(x, y, button_width, button_height, "NIVEAU 3: Load Image", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);

    MLV_actualise_window();
}

int handle_button_click(int x, int y) {
    int button_width = 300;
    int button_height = 30;
    int padding = 10;
    int bx = padding;
    int by = padding;

    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 1; // Construct Quadtree
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 2; // Save as QTN (BW)
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 3; // Save as QTC (Color)
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 4; // Minimize Quadtree
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 5; // Save Minimized QTN (BW)
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 6; // Save Minimized QTC (Color)
    by += button_height + padding;
    if (x > bx && x < bx + button_width && y > by && y < by + button_height) return 7; // Load Image

    return 0;
}

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

    QuadtreeNode* quadtree = NULL;

    while (1) {
        draw_buttons();
        int x, y;
        MLV_wait_mouse(&x, &y);

        int button = handle_button_click(x, y);

        switch (button) {
            case 1:
                if (quadtree) free_quadtree(quadtree);
                quadtree = draw_quadtree_no_loss(image);
                break;
            case 2:
                if (quadtree) {
                    char file_path[MAX_FILENAME_LENGTH];
                    snprintf(file_path, sizeof(file_path), "quadtree.qtn");
                    save_image_quadtree_bw(file_path, quadtree);
                }
                break;
            case 3:
                if (quadtree) {
                    char file_path[MAX_FILENAME_LENGTH];
                    snprintf(file_path, sizeof(file_path), "quadtree.qtc");
                    save_image_quadtree(file_path, quadtree);
                }
                break;
            case 4:
                if (quadtree) {
                    draw_quadtree_with_loss(quadtree, image);
                }
                break;
            case 5:
                if (quadtree) {
                    char file_path[MAX_FILENAME_LENGTH];
                    snprintf(file_path, sizeof(file_path), "minimized_quadtree.qtn");
                    save_image_quadtree_bw(file_path, quadtree);
                }
                break;
            case 6:
                if (quadtree) {
                    char file_path[MAX_FILENAME_LENGTH];
                    snprintf(file_path, sizeof(file_path), "minimized_quadtree.qtc");
                    save_image_quadtree(file_path, quadtree);
                }
                break;
            case 7:
                {
                    char image_name[MAX_FILENAME_LENGTH];
                    printf("Enter the image filename: ");
                    scanf("%s", image_name);
                    
                    const char* ext = get_file_extension(image_name);

                    if (strcmp(ext, "qtn") == 0) {
                        quadtree = load_image_quadtree_bw(image_name);
                        if (quadtree) {
                            MLV_clear_window(MLV_COLOR_BLACK);
                            draw_entire_quadtree(quadtree);
                        }
                    } else if (strcmp(ext, "qtc") == 0) {
                        quadtree = load_image_quadtree(image_name);
                        if (quadtree) {
                            MLV_clear_window(MLV_COLOR_BLACK);
                            draw_entire_quadtree(quadtree);
                        }
                    } else {
                        MLV_Image *new_image = MLV_load_image(image_name);
                        if (new_image) {
                            MLV_resize_image(new_image, IMAGE_SIZE, IMAGE_SIZE);
                            MLV_clear_window(MLV_COLOR_BLACK);
                            MLV_draw_image(new_image, 0, 0);
                            MLV_actualise_window();
                            MLV_free_image(new_image);
                        } else {
                            printf("Could not load image %s\n", image_name);
                        }
                    }
                }
                break;
            default:
                break;
        }
    }

    if (quadtree) free_quadtree(quadtree);
    MLV_free_image(image);
    MLV_free_window();

    return 0;
}
