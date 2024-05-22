/**
 * @file	draw_button.h
 *
 * @brief 	Definitions of button drawing functions
 *
 */

#ifndef DRAW_BUTTON_H
#define DRAW_BUTTON_H

#include "ei_implementation.h"
#include "ei_implem_button.h"
#include "ei_draw_tools.h"

/**
 * Initializes a linked list node for given point coordinates.
 *
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 *
 * @return The linked list node.
 */
struct ei_linked_point_t* init_linked_point(int x, int y);

/**
 * Appends an arc to the linked point list and updates the current pointer.
 *
 * @param current_point The current point in the linked list.
 * @param arc The arc to append.
 */
void append_arc(struct ei_linked_point_t** current_point, struct ei_linked_point_t* arc);

/**
 * @brief	Draws a rounded button depending on parameters.
 *
 * @param  widget  A pointer to the widget button instance to draw.
 * @param  surface  A locked surface where to draw the widget.
 * @param  clipper  If not NULL, the drawing is restricted within this rectangle.
 */

void draw_button(ei_widget_t widget, ei_surface_t surface, ei_rect_t* clipper);
#endif