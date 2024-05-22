/**
 * @file	ei_implementation.h
 *
 * @brief 	Private definitions.
 *
 */

#ifndef EI_CLASS_H
#define EI_CLASS_H


#include "ei_implem_frame.h"
#include "ei_implem_button.h"
#include "ei_implem_toplevel.h"
#include "ei_implem_entry.h"

/**
 * @brief	The pick_id that is currently being used.
 */

extern uint32_t current_pick_id;

/**
 * @brief	Store all the registered widget classes of the application.
 *
 */
extern ei_widgetclass_t* registered_widget_classes;

#endif