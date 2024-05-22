//
// Created by matteo on 5/15/24.
//

#ifndef DRAW_FRAME_H
#define DRAW_FRAME_H

#include "ei_types.h"
#include "ei_implementation.h"

/**
 * @brief	Moves a triangle
 * @param  point  The point of the square the pointer must at least point to a tab of 3 points
 * @param  dx  The x offset
 * @param  dy  The y offset
 */
void move_triangle(ei_point_t* point, int dx, int dy);

/**
 * @brief	Moves a square
 * @param  point  The point of the square the pointer must at least point to a tab of 4 points
 * @param  dx  The x offset
 * @param  dy  The y offset
 */
void move_square(ei_point_t* point, int dx, int dy);

/**
 * @brief	Draws a border
 * @param  surface  The surface where to draw the border
 * @param  rect  The screen location of the frame
 * @param  border_thickness  The thickness of the border
 * @param  color  The color of the border
 * @param  darkening_factor  The factor of darkening (should be positive if the relief is raised, negative if the relief is sunken)
 * @param  clipper  The clipper
 */
void draw_border(ei_surface_t surface, ei_rect_t* rect, int border_thickness, ei_color_t color, float darkening_factor, ei_rect_t* clipper);


/**
 * @brief	Draws a frame
 * @param  widget  A pointer to the widget frame instance to draw.
 * @param  surface  A locked surface where to draw the widget.
 * @param  clipper  If not NULL, the drawing is restricted within this rectangle.
 */

void draw_frame(ei_widget_t widget, ei_surface_t surface, ei_rect_t* clipper);

#endif //DRAW_FRAME_H
