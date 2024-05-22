/**
 * @file	ei_implem_toplevel.h
 *
 * @brief 	Private definitions about top levels.
 *
 */

#ifndef EI_IMPLEM_TOPLEVEL_H
#define EI_IMPLEM_TOPLEVEL_H

#include "ei_implementation.h"
#include "ei_types.h"

/**
 * \brief   Function that allocates a block of memory that is big enough to store the
 *         attributes of a toplevel.
 *
 * @return  A block of memory big enough to store a toplevel.
 */

ei_widget_t toplevel_allocfunc(void);

/**
 * \brief   Function that releases the memory used by a toplevel.
 *
 * @param  widget  The widget toplevel which resources are to be freed.
 */


void toplevel_releasefunc(ei_widget_t widget);

/**
 * \brief   Function that draws a toplevel.
 *
 * @param  widget  A pointer to the widget toplevel instance to draw.
 * @param  surface  A locked surface where to draw the widget.
 * @param  pick_surface  The picking offscreen.
 * @param  clipper  If not NULL, the drawing is restricted within this rectangle.
 */

void toplevel_drawfunc(ei_widget_t widget,
                     ei_surface_t surface,
                     ei_surface_t	pick_surface,
                     ei_rect_t* clipper);

/**
 * \brief   Function that sets the default values of a toplevel.
 */

void toplevel_setdefaultsfunc(ei_widget_t widget);

/**
 * \brief   Function that is called to notify the geometry manager
 *          that the toplevel geometry has been modified.
 */

void toplevel_geomnotifyfunc(ei_widget_t widget);

/**
 * \brief   Implementation of a toplevel type.
 */

typedef struct ei_impl_toplevel_t {
    ei_impl_widget_t    widget;
    ei_color_t      	color;
    int 			    border_width;
    ei_string_t 		title;
    ei_font_t           title_fontstyle;
    ei_color_t          title_color;
    bool			    closable;
    ei_axis_set_t		resizable;
    ei_size_ptr_t		min_size;
    ei_color_t          header_color;

} ei_impl_toplevel_t;

/**
 * \brief   Definition of the toplevel class.
 */

extern ei_widgetclass_t ei_toplevelclass;

#endif //EI_TOPLEVEL_H
