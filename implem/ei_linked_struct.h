/**
 * @file	ei_linked_struct.h
 *
 * @brief 	Private definitions about linked structures (linked_rect, linked_points).
 *
 */

#ifndef EI_IMPLEM_STRUCT_H
#define EI_IMPLEM_STRUCT_H

#include "ei_implementation.h"

/**
 * @brief	Linked list of points
 */

struct ei_linked_point_t{
    ei_point_t point;
    struct ei_linked_point_t* next;
};

/**
 * @brief	Returns the size of a linked list of points
 *
 * @param linked_points
 *
 * @return  The size of the linked list
 */

uint32_t size_of_linked_point(struct ei_linked_point_t* linked_points);

/**
 * @brief	Frees the memory allocated for a linked list of points
 *
 * @param	linked_points	The linked list of points to free
 */

void free_linked_points(struct ei_linked_point_t* linked_points);

/**
 * @brief	Convert a linked list of points into an array of points.
 *
 * @param  linked_points  Linked list of points.
 *
 * @param  n  Number of points in the linked list.
 *
 * @returns  Array of points based on the linked list of points.
 */

ei_point_t* linked_to_array(struct ei_linked_point_t* linked_points,
                            uint32_t n);

/**
 * @brief	Add a given rectangle to a linked rectangles list.
 *
 * @param  rects  Linked list of rectangles.
 */

void free_linked_rect(ei_linked_rect_t** rects);

#endif