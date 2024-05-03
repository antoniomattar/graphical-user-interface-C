/**
 * @file	ei_frame.h
 *
 * @brief 	Private definitions about frames.
 *
 */

#ifndef EI_FRAME_H
#define EI_FRAME_H

#include "ei_implementation.h"

/**
 * \brief   TODO
 *
 * @return  TODO
 */

ei_widget_t frame_allocfunc(void);

/**
 * \brief   TODO
 *
 * @param  TODO
 */

void frame_releasefunc(ei_widget_t widget);

/**
 * \brief   TODO
 *
 * @param  TODO
 */

void frame_drawfunc(ei_widget_t widget,
                     ei_surface_t surface,
                     ei_surface_t	pick_surface,
                     ei_rect_t* clipper);

/**
 * \brief   TODO
 *
 * @param  TODO
 */

void frame_setdefaultsfunc(ei_widget_t widget);

/**
 * \brief   TODO
 *
 * @param  TODO
 */

void frame_geomnotifyfunc(ei_widget_t widget);

/**
 * \brief   TO WRITE
 */

typedef struct ei_impl_frame_t{
    ei_widget_t widget;
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
 * \brief   TO WRITE
 */

extern ei_widgetclass_t ei_frameclass;

#endif