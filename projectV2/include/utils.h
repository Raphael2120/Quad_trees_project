#ifndef UTILS_H
#define UTILS_H

#include <MLV/MLV_all.h>
#include <stdbool.h>

/* Color component extraction functions */
Uint8 get_red_component(MLV_Color color);
Uint8 get_green_component(MLV_Color color);
Uint8 get_blue_component(MLV_Color color);
Uint8 get_alpha_component(MLV_Color color);

/* Safe memory allocation functions */
void* safe_malloc(size_t size);
void* safe_realloc(void* ptr, size_t size);

/* File validation functions */
bool file_exists(const char* filename);
bool validate_quadtree_file(const char* filename);

#endif // UTILS_H
