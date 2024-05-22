/**
 * @file	ei_implem_frame.h
 *
 * @brief 	Private definitions about frames.
 *
 */

#ifndef EI_IMPLEM_FRAME_H
#define EI_IMPLEM_FRAME_H

#include "ei_implementation.h"

/**
 * \brief   Function that allocates a block of memory that is big enough to store the
 *         attributes of a frame.
 *
 * @return  A block of memory big enough to store a frame.
 */

ei_widget_t frame_allocfunc(void);

/**
 * \brief   Function that releases the memory used by a frame.
 *
 * @param  widget  The widget frame which resources are to be freed.
 */


void frame_releasefunc(ei_widget_t widget);

/**
 * \brief   Function that draws a frame.
 *
 * @param  widget  A pointer to the widget frame instance to draw.
 * @param  surface  A locked surface where to draw the widget.
 * @param  pick_surface  The picking offscreen.
 * @param  clipper  If not NULL, the drawing is restricted within this rectangle.
 */

void frame_drawfunc(ei_widget_t widget,
                     ei_surface_t surface,
                     ei_surface_t	pick_surface,
                     ei_rect_t* clipper);

/**
 * \brief   Function that sets the default values of a frame.
 */

void frame_setdefaultsfunc(ei_widget_t widget);

/**
 * \brief   Function that is called to notify the geometry manager
 *          that the frame geometry has been modified.
 */

void frame_geomnotifyfunc(ei_widget_t widget);

/**
 * \brief   Implementation of a frame type.
 */

typedef struct ei_impl_frame_t{
    ei_impl_widget_t widget;
    ei_size_t size;
    ei_color_t color;
    int border_width;
    ei_relief_t relief;
    ei_string_t text;
    ei_font_t text_font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    ei_surface_t img;
    ei_rect_ptr_t img_rect;
    ei_anchor_t img_anchor;
} ei_impl_frame_t;

/**
 * \brief   Definition of the frame class.
 */

extern ei_widgetclass_t ei_frameclass;

#endif