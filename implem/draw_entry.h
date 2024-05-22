//
// Created by matteo on 5/13/24.
//

#ifndef DRAW_ENTRY_H
#define DRAW_ENTRY_H

#include "ei_types.h"
#include "ei_implementation.h"

/**
 * @brief	Draws an entry
 * @param  widget  A pointer to the widget entry instance to draw.
 * @param  surface  A locked surface where to draw the widget.
 * @param  clipper  If not NULL, the drawing is restricted within this rectangle.
 */

void draw_entry(ei_widget_t widget, ei_surface_t surface, ei_rect_t* clipper);

#endif //DRAW_ENTRY_H