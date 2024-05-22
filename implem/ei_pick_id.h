/**
 * @file	ei_pick_id.h
 *
 * @brief 	Functions managing offscreen pick surfaces.
 *
 */

#ifndef EI_PICK_ID_H
#define EI_PICK_ID_H

#include "ei_implementation.h"

/**
 * \brief   Function that returns the widget under the cursor, after an event for example.
 *
 * @parameters  The event for which we want to know the cursor position when it occured.
 *
 * @returns  The widget beneath the cursor.
 *
 */

ei_widget_t ei_get_widget_on_cursor(ei_event_t* event);

/**
 * \brief   Function that returns the widget based on its pick id.
 *
 * @parameters  The root widget.
 *
 * @parameters  The widget pick id.
 *
 * @returns  The widget.
 *
 */

ei_widget_t ei_get_widget_from_id(ei_widget_t root,
                                  uint32_t pick_id);

/**
 * \brief   Function that updates a part of the offscreen surface with a pick id value.
 *
 * @parameters  The surface updated on the offscreen pick surface.
 *
 * @parameters  The widget we want to update on the offscreen surface.
 *
 */

void ei_update_pick_surface(ei_surface_t surface,
                            ei_widget_t widget);

#endif