#ifndef CONFIG_H
#define CONFIG_H

/* Image Configuration */
#define DEFAULT_IMAGE_SIZE 512

/* Heap Configuration */
#define DEFAULT_HEAP_CAPACITY 1024
#define HEAP_GROWTH_FACTOR 2

/* Quadtree Configuration */
#define MIN_NODE_SIZE 1
#define MERGE_THRESHOLD 25.0
#define GRAPH_NODE_CAPACITY_INITIAL 10000

/* UI Configuration */
#define WINDOW_WIDTH 860
#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 30
#define BUTTON_PADDING 10
#define NUM_BUTTONS 7

/* File Paths */
#define OUTPUT_DIR "img/output/"
#define MAX_FILENAME_LENGTH 256

#endif // CONFIG_H
