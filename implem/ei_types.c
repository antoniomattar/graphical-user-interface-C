#include "ei_types.h"
#include "ei_widget.h"

/**
 * \brief   Adds a rectangle to the list of rectangles that must be updated on screen. The real
 *         update on the screen will be done at the right moment in the main loop.
 *
 * @param linked_rects The list of rectangles to add the rectangle to.
 * @param rect         The rectangle to add, expressed in the root window coordinates.
 */
 void ei_linked_rect_t_add(ei_linked_rect_t** linked_rects, ei_rect_t rect) {
// Create the new linked_rect_t
    ei_linked_rect_t* new_linked_rect = malloc(sizeof(ei_linked_rect_t));
    new_linked_rect->rect = rect;
    new_linked_rect->next = NULL;
// Add the new linked_rect_t to the list
// If the list is empty, the new linked_rect_t is the first element
    if (*linked_rects == NULL) {
        *linked_rects = new_linked_rect;
    }
// Else, we add the new linked_rect_t at the end of the list
    else {
        ei_linked_rect_t* current_linked_rect = *linked_rects;
        while (current_linked_rect->next != NULL) {
            current_linked_rect = current_linked_rect->next;
        }
        current_linked_rect->next = new_linked_rect;
    }
}
