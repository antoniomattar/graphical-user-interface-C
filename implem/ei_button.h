/**
 * @file	ei_button.h
 *
 * @brief 	Private definitions about buttons.
 *
 */

#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include "ei_implementation.h"

/**
 * \brief   Function that allocates a block of memory that is big enough to store the
 *         attributes of a button.
 *
 * @return  A block of memory big enough to store a button.
 */

ei_widget_t button_allocfunc(void);

/**
 * \brief   Function that releases the memory used by a button.
 *
 * @param  widget  The widget button which resources are to be freed.
 */

void button_releasefunc(ei_widget_t widget);

/**
 * \brief   Function that draws a button.
 *
 * @param  widget  A pointer to the widget button instance to draw.
 * @param  surface  A locked surface where to draw the widget.
 * @param  pick_surface  The picking offscreen.
 * @param  clipper  If not NULL, the drawing is restricted within this rectangle.
 */

void button_drawfunc(ei_widget_t widget,
                     ei_surface_t surface,
                     ei_surface_t	pick_surface,
                     ei_rect_t* clipper);

/**
 * \brief   Function that sets the default values of a button.
 */

void button_setdefaultsfunc();

/**
 * \brief   Function that is called to notify the geometry manager
 *          that the button geometry has been modified.
 */

void button_geomnotifyfunc();

/**
 * \brief   Implementation of a button type.
 */

typedef struct ei_impl_button_t{  //Implementation of a button
    ei_widget_t widget;
    ei_size_t size;
    ei_color_t color;
    int border_width;
    int corner_radius;
    ei_relief_t relief;
    ei_string_t text;
    ei_font_t text_font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    ei_surface_t img;
    ei_rect_ptr_t img_rect;
    ei_anchor_t img_anchor;
    ei_callback_t callback;
    ei_user_param_t user_param;
} ei_impl_button_t;


/**
 * \brief   Definition of the button class.
 */

extern ei_widgetclass_t ei_buttonclass;

#endif

