//
// Created by matteo on 5/13/24.
//


#ifndef DRAW_TOPLEVEL_H
#define DRAW_TOPLEVEL_H

#include "ei_types.h"
#include "ei_implementation.h"

/**
 * @brief Draws the background of a toplevel widget.
 *
 * This function draws the background of a toplevel widget on a given surface. The background is drawn acordingly to the border thickness.
 *
 * @param surface The surface to draw on.
 *
 * @param rect A pointer to the rectangle representing the area where the background should be drawn.
 *
 * @param border_thickness The thickness of the border around the background.
 *
 * @param color The color of the background.
 *
 * @param clipper A pointer to a clipping rectangle. Pixels outside this rectangle won't be modified by the drawing. If NULL, no clipping will be done.
 */

void draw_toplevel_background(ei_surface_t surface,
                              ei_rect_t* rect,
                              int border_thickness,
                              ei_color_t color,
                              ei_rect_t* clipper);

/**
 * @brief Draws the border of a toplevel widget.
 *
 * @param surface The surface to draw on.
 *
 * @param rect A pointer to the rectangle representing the area where the border should be drawn.
 *
 * @param border_thickness The thickness of the border.
 *
 * @param color The color of the border.
 *
 * @param clipper A pointer to a clipping rectangle. Pixels outside this rectangle won't be modified by the drawing. If NULL, no clipping will be done.
 */
void draw_toplevel_border(ei_surface_t surface,
                          ei_rect_t* rect,
                          int border_thickness,
                          ei_color_t color,
                          ei_rect_t* clipper);

/**
 * @brief	Draws a toplevel
 *
 * @param  widget  A pointer to the widget toplevel instance to draw.
 *
 * @param  surface  A locked surface where to draw the widget.
 *
 * @param  clipper  If not NULL, the drawing is restricted within this rectangle.
 */

void draw_toplevel(ei_widget_t widget,
                   ei_surface_t surface,
                   ei_rect_t* clipper);

#endif //DRAW_TOPLEVEL_H
