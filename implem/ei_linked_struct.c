#include "ei_linked_struct.h"

uint32_t size_of_linked_point(struct ei_linked_point_t* linked_points){
    if (linked_points == NULL){
        return 0;
    }
    return 1 + size_of_linked_point(linked_points->next);
}

void free_linked_points(struct ei_linked_point_t* points) {
    struct ei_linked_point_t* current_point = points;
    while (current_point != NULL) {
        struct ei_linked_point_t* next_point = current_point->next;
        free(current_point);
        current_point = next_point;
    }
}

ei_point_t* linked_to_array(struct ei_linked_point_t* linked_points, uint32_t n) {
    ei_point_t* point_array = malloc(n * sizeof(ei_point_t));
    for (int i = 0; i < n; i++) {
        point_array[i] = linked_points->point;
        linked_points = linked_points->next;
    }
    return point_array;
}

void free_linked_rect(ei_linked_rect_t** rects){
    struct ei_linked_rect_t* current_rect = *rects;
    while (current_rect != NULL) {
        struct ei_linked_rect_t* next_rect = current_rect->next;
        free(current_rect);
        current_rect = next_rect;
        *rects = current_rect;
    }
}