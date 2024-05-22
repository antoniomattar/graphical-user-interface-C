/**
 * @file	ei_implem_entry.h
 *
 * @brief 	Private definitions about entries.
 *
 */

#ifndef EI_IMPLEM_ENTRY_H
#define EI_IMPLEM_ENTRY_H

#include "ei_implementation.h"

/**
 * \brief   Function that allocates a block of memory that is big enough to store the
 *         attributes of a entry.
 *
 * @return  A block of memory big enough to store a entry.
 */

ei_widget_t entry_allocfunc(void);

/**
 * \brief   Function that releases the memory used by a entry.
 *
 * @param  widget  The widget entry which resources are to be freed.
 */


void entry_releasefunc(ei_widget_t widget);

/**
 * \brief   Function that draws a entry.
 *
 * @param  widget  A pointer to the widget entry instance to draw.
 * @param  surface  A locked surface where to draw the widget.
 * @param  pick_surface  The picking offscreen.
 * @param  clipper  If not NULL, the drawing is restricted within this rectangle.
 */

void entry_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t	pick_surface,
                    ei_rect_t* clipper);

/**
 * \brief   Function that sets the default values of a entry.
 */

void entry_setdefaultsfunc(ei_widget_t widget);

/**
 * \brief   Function that is called to notify the geometry manager
 *          that the entry geometry has been modified.
 */

void entry_geomnotifyfunc(ei_widget_t widget);

/**
 * \brief   Implementation of a entry type.
 */

typedef struct ei_impl_entry_t{
    ei_impl_widget_t	widget;
    int					requested_char_size;
    ei_color_t	color;
    int					border_width;
    ei_font_t			text_font;
    ei_color_t			text_color;
    ei_const_string_t	text;
    ei_anchor_t			text_anchor;
} ei_impl_entry_t;

/**
 * \brief   Definition of the entry class.
 */

extern ei_widgetclass_t ei_entryclass;

#endif
