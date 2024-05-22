/**
 * @file	ei_callback.h
 *
 * @brief 	Definitions of intern callbacks.
 *
 */

#ifndef EI_CALLBACK_H
#define EI_CALLBACK_H

#include "ei_implementation.h"
#include "ei_implem_toplevel.h"

typedef struct ei_bound_eventtype {
    ei_eventtype_t eventtype;
    ei_widget_t widget;
    ei_tag_t tag;
    ei_callback_t callback;
    void* user_param;

} ei_bound_eventtype;

typedef struct ei_linked_bound_eventtype {
    ei_bound_eventtype bound_eventtype;
    struct ei_linked_bound_eventtype* next;
} ei_linked_bound_eventtype;

extern ei_linked_bound_eventtype* bound_eventtype_list;

/**
 * @brief	Called in response to pressing the mouse down on a button. Used to display the effect
 *          of pressing a button.
 *
 * @param	widget		The widget for which the event was generated.
 * *
 * @param	event		The event containing all its parameters (type, etc.)
 *
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return  A boolean telling if the event was consumed by the callback or not.
 */

bool ei_press_button_down(ei_widget_t widget,
                          struct ei_event_t* event,
                          ei_user_param_t user_param);

/**
 * @brief	Called in response to releasing the mouse off a button. Used to display the effect
 *          of releasing a button.
 *
 * @param	widget		The widget for which the event was generated.
 *
 * @param	event		The event containing all its parameters (type, etc.)
 *
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return  A boolean telling if the event was consumed by the callback or not.
 */

bool ei_press_button_up(ei_widget_t widget,
                        struct ei_event_t* event,
                        ei_user_param_t user_param);

/**
 * @brief	Called in response to moving the mouse on the button when it was moved away from it
 *          with the mouse click still held. Used to display the effect of releasing a button.
 *
 * @param	widget		The widget for which the event was generated.
 *
 * @param	event		The event containing all its parameters (type, etc.)
 *
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return  A boolean telling if the event was consumed by the callback or not.
 */

bool ei_move_back_on_button(ei_widget_t widget,
                            struct ei_event_t* event,
                            ei_user_param_t user_param);

/**
 * @brief	Called in response to moving the mouse off a button. Used to display the effect
 *          of releasing a button.
 *
 * @param	widget		The widget for which the event was generated.
 *
 * @param	event		The event containing all its parameters (type, etc.)
 *
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return  A boolean telling if the event was consumed by the callback or not.
 */

bool ei_move_out_of_button(ei_widget_t widget,
                           struct ei_event_t* event,
                           ei_user_param_t user_param);

bool ei_press_entry(ei_widget_t widget,
                    struct ei_event_t* event,
                    ei_user_param_t user_param);

bool ei_write_entry(ei_widget_t widget,
                    struct ei_event_t* event,
                    ei_user_param_t user_param);

bool ei_end_of_write(ei_widget_t widget,
                     struct ei_event_t* event,
                     ei_user_param_t user_param);

/**
 * @brief	Called in response to pressing on a top-level. Used to display the main
 *          top-level the user clicked on.
 *
 * @param	widget		The widget for which the event was generated.
 *
 * @param	event		The event containing all its parameters (type, etc.)
 *
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return  A boolean telling if the event was consumed by the callback or not.
 */

bool ei_press_toplevel(ei_widget_t widget,
                       struct ei_event_t* event,
                       ei_user_param_t user_param);

bool ei_close_toplevel(ei_widget_t widget,
                       struct ei_event_t* event,
                       ei_user_param_t user_param);

/**
 * @brief	Called in response to pressing the top of a top-level. Used to lock the current
 *          top-level we want to drag.
 *
 * @param	widget		The widget for which the event was generated.
 *
 * @param	event		The event containing all its parameters (type, etc.)
 *
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return  A boolean telling if the event was consumed by the callback or not.
 */

bool ei_drag_toplevel(ei_widget_t widget,
                       struct ei_event_t* event,
                       ei_user_param_t user_param);

/**
 * @brief	Called in response to moving mouse while a top-level is locked. Used to drag the current
 *          top-level.
 *
 * @param	widget		The widget for which the event was generated.
 *
 * @param	event		The event containing all its parameters (type, etc.)
 *
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return  A boolean telling if the event was consumed by the callback or not.
 */

bool ei_move_toplevel(ei_widget_t widget,
                      struct ei_event_t* event,
                      ei_user_param_t user_param);

/**
 * @brief	Called in response to releasing the top of a top-level. Used to unlock the current
 *          top-level we wanted to drag.
 *
 * @param	widget		The widget for which the event was generated.
 *
 * @param	event		The event containing all its parameters (type, etc.)
 *
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return  A boolean telling if the event was consumed by the callback or not.
 */

bool ei_release_toplevel(ei_widget_t widget,
                      struct ei_event_t* event,
                      ei_user_param_t user_param);

//  TODO : Write doc of functions to handle resizing

bool detect_drag(ei_widget_t widget,
                   ei_event_t event);


bool detect_resize(ei_widget_t widget,
                   ei_event_t event);

bool ei_resize_toplevel(ei_widget_t widget,
                        struct ei_event_t* event,
                        ei_user_param_t user_param);

bool ei_resize_move_toplevel(ei_widget_t widget,
                             struct ei_event_t* event,
                             ei_user_param_t user_param);

bool ei_resize_release_toplevel(ei_widget_t widget,
                                struct ei_event_t* event,
                                ei_user_param_t user_param);
/**
 * @brief	Bind all library callbacks.
 */

void ei_bind_library(void);

/**
 * @brief	Unbind all library callbacks.
 */

void ei_unbind_library(void);

/**
 * @brief A pointer to the currently selected toplevel widget.
 *
 * This pointer is used to keep track of the toplevel widget that is currently selected by the user.
 */
extern ei_impl_toplevel_t* current_selected_toplevel;

/**
 * @brief The surface that contain the old and new screen location of the widget that  was just redrawn
 *
 * This pointer is used to only draw what needs to be drawn.
 */
extern ei_rect_t fusion_rectangle;

#endif