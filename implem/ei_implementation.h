/**
 * @file	ei_implementation.h
 *
 * @brief 	Private definitions.
 * 
 */

#ifndef EI_IMPLEMENTATION_H
#define EI_IMPLEMENTATION_H


#include "hw_interface.h"
#include "ei_event.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"



/**
 * \brief	Fields common to all types of widget. Every widget classes specializes this base
 *		class by adding its own fields.
 */
typedef struct ei_impl_widget_t {
	ei_widgetclass_t*	wclass;		///< The class of widget of this widget. Avoids the field name "class" which is a keyword in C++.
	uint32_t		pick_id;	///< Id of this widget in the picking offscreen.
	ei_color_t*		pick_color;	///< pick_id encoded as a color.
	void*			user_data;	///< Pointer provided by the programmer for private use. May be NULL.
	ei_widget_destructor_t	destructor;	///< Pointer to the programmer's function to call before destroying this widget. May be NULL.

	/* Widget Hierachy Management */
	ei_widget_t		parent;		///< Pointer to the parent of this widget.
	ei_widget_t		children_head;	///< Pointer to the first child of this widget.	Children are chained with the "next_sibling" field.
	ei_widget_t		children_tail;	///< Pointer to the last child of this widget.
	ei_widget_t		next_sibling;	///< Pointer to the next child of this widget's parent widget.

	/* Geometry Management */
	ei_geom_param_t		geom_params;	///< Pointer to the geometry management parameters for this widget. If NULL, the widget is not currently managed and thus, is not displayed on the screen.
	ei_size_t		requested_size;	///< See \ref ei_widget_get_requested_size.
	ei_rect_t*		screen_location;///< See \ref ei_widget_get_screen_location.
	ei_rect_t*		content_rect;	///< See ei_widget_get_content_rect. By defaults, points to the screen_location.
} ei_impl_widget_t;

/**
 * @brief   Checks if a given widget class is registered.
 *
 * This function traverses the list of registered widget classes and checks if the given widget class is in the list.
 *
 * @param   widgetclass  The widget class to check.
 *
 * @return  True if the widget class is registered, false otherwise.
 */
bool ei_widgetclass_is_registered(ei_widgetclass_t* widgetclass);

/**
 * @brief	Draws the children of a widget.
 * 		The children are draw withing the limits of the clipper and
 * 		the widget's content_rect.
 *
 * @param	widget		The widget which children are drawn.
 * @param	surface		A locked surface where to draw the widget's children.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */

void		ei_impl_widget_draw_children	(ei_widget_t		widget,
						 ei_surface_t		surface,
						 ei_surface_t		pick_surface,
						 ei_rect_t*		clipper);



/**
 * \brief	Converts the red, green, blue and alpha components of a color into a 32 bits integer
 * 		than can be written directly in the memory returned by \ref hw_surface_get_buffer.
 * 		The surface parameter provides the channel order.
 *
 * @param	surface		The surface where to store this pixel, provides the channels order.
 * @param	color		The color to convert.
 *
 * @return 			The 32 bit integer corresponding to the color. The alpha component
 *				of the color is ignored in the case of surfaces that don't have an
 *				alpha channel.
 */
uint32_t	ei_impl_map_rgba(ei_surface_t surface, ei_color_t color);


/**
 * \brief	Fields common to all geometry managers. Every geometry manager specializes this by adding its own fields.
 */

typedef struct ei_impl_geom_param_t {
	ei_geometrymanager_t*		manager;	///< The geometry managers that manages this widget.
} ei_impl_geom_param_t;

typedef struct ei_impl_placer_param_t {
	ei_impl_geom_param_t		mclass;
	ei_anchor_t					anchor;
	int							x;
	int							y;
	int							height;
	int							width;
	float						rel_x;
	float						rel_y;
	float						rel_height;
	float						rel_width;
} ei_impl_placer_param_t;

/**
 * @brief      Registers a geometry manager in the list of registered geometry managers.
 *
 * @param      geometrymanager  The geometry manager to register
 */

bool ei_geometrymanager_is_registered(ei_geometrymanager_t* geometrymanager);

/**
* @brief   Executes the run runs the geometry computation for a widget.
 * 		Must call \ref ei_placer_runfinalize before returning.
 *
 * @param   widget  The widget for which the placer geometry manager's run function is to be executed.
 */
void ei_placer_runfunc(ei_widget_t widget);

/**
 * @brief   Releases the resources associated with the placer geometry manager for the given widget.
 *
 * @param   widget  The widget for which the placer geometry manager's resources are to be released.
 */
void ei_placer_releasefunc(ei_widget_t widget);

/**
 * @brief   Definition of the placer geometry manager.
 *
 * This structure represents the placer geometry manager. It contains function pointers to the
 * run and release functions of the placer geometry manager. The run function is responsible for
 * setting the screen location of a widget according to the placer parameters. The release function
 * is responsible for freeing all specific parameters of the placer geometry manager.
 */
extern ei_geometrymanager_t placer_geometrymanager;

/**
 * @brief	Update screen location of a widget.
 *
 * @param	widget		The widget which children are drawn.
 *
 * @param	size		The size of the rectangle.

 *				(expressed in the surface reference frame).
 */

void ei_set_screen_location(ei_widget_t widget,
                            ei_size_t size);

/**
 * @brief	Update content rectangle of a widget.
 *
 * @param	widget		The widget which children are drawn.
 *
 * @param	size		The size of the rectangle.

 *				(expressed in the surface reference frame).
 */



void ei_set_content_rect(ei_widget_t widget,
                         ei_size_t size);
/**
 * @brief	calculate the intersection of two surfaces.
 *
 * @param	rect1		the first surface.
 *
 * @param	rect2		The second surface.

 *				(expressed in the surface reference frame).
 */

ei_rect_t* intersect_rect(ei_rect_t* rect1, ei_rect_t* rect2) ;

/**
 * @brief	Free the common attributes of all widgets.
 *
 * @param	widget		The widget which attributes are freed.
 *
 */
void ei_free_widget_attributes(ei_widget_t widget);

/**
 * @brief	Set default values for the common attributes of all widgets.
 *
 * @param	widget		The widget which attributes are set.
 *
 */
void ei_widget_set_default_parms(ei_widget_t widget);

/**
 * @brief   Calculates the intersection of two rectangles.
 *
 * This function takes two rectangles as input and calculates their intersection.
 * If the rectangles do not intersect, the function returns NULL.
 *
 * @param   rect1  The first rectangle.
 * @param   rect2  The second rectangle.
 *
 * @return  A pointer to a new rectangle that represents the intersection of rect1 and rect2.
 *          If the rectangles do not intersect, the function returns {{0,0},{0,0}}.
 */
ei_rect_t intersection_rect_with_rect(
		const ei_rect_t* rect1,
		const ei_rect_t* rect2);

/**
 * @brief Returns the smallest rectangle containing the two input rectangles.
 * @param rect1 The first input rectangle.
 * @param rect2 The second input rectangle.
 * @return The smallest rectangle that contains both input rectangles.
 */
ei_rect_t smallest_containing_rect(
        ei_rect_t rect1,
        ei_rect_t rect2);

extern ei_widget_t root_widget;
extern ei_surface_t root_surface;
extern ei_surface_t pick_surface;
extern ei_linked_rect_t* rects;
extern ei_widget_t on_cursor_widget;

void ei_handle_current_event(struct ei_event_t* event);

// Drawing constants
//TODO maybe put those in the files that use them
#define HEADER_HEIGHT 30
#define PADDING 5
#define CLOSE_BUTTON_SIZE 20
#define RESIZE_AREA_SIZE 20
#define BOARDER_COLOR (ei_color_t){0x88, 0x88, 0x88, 0xff}
#define CLOSE_BUTTON_COLOR (ei_color_t){0xff, 0x00, 0x00, 0xff}
#define CLOSE_BUTTON_BORDER_WIDTH 2

#endif
