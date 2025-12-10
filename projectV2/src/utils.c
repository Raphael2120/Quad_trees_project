#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/utils.h"

/* Color component extraction functions */
Uint8 get_red_component(MLV_Color color) {
    Uint8 r, g, b, a;
    MLV_convert_color_to_rgba(color, &r, &g, &b, &a);
    return r;
}

Uint8 get_green_component(MLV_Color color) {
    Uint8 r, g, b, a;
    MLV_convert_color_to_rgba(color, &r, &g, &b, &a);
    return g;
}

Uint8 get_blue_component(MLV_Color color) {
    Uint8 r, g, b, a;
    MLV_convert_color_to_rgba(color, &r, &g, &b, &a);
    return b;
}

Uint8 get_alpha_component(MLV_Color color) {
    Uint8 r, g, b, a;
    MLV_convert_color_to_rgba(color, &r, &g, &b, &a);
    return a;
}

/* Safe memory allocation functions */
void* safe_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed (malloc %zu bytes)\n", size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* safe_realloc(void* ptr, size_t size) {
    void* new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        fprintf(stderr, "Error: Memory reallocation failed (realloc %zu bytes)\n", size);
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

/* File validation functions */
bool file_exists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

bool validate_quadtree_file(const char* filename) {
    if (!file_exists(filename)) {
        fprintf(stderr, "Error: File does not exist: %s\n", filename);
        return false;
    }
    
    /* Check file extension */
    const char* ext = strrchr(filename, '.');
    if (ext == NULL) {
        fprintf(stderr, "Error: File has no extension: %s\n", filename);
        return false;
    }
    
    if (strcmp(ext, ".qtc") != 0 && strcmp(ext, ".qtn") != 0) {
        fprintf(stderr, "Warning: File extension is not .qtc or .qtn: %s\n", filename);
        /* Not an error, might be a regular image */
    }
    
    return true;
}
