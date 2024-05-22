#include "ei_callback.h"
#include <ei_placer.h>
#include "ei_class.h"
#include "ei_widget_attributes.h"
#include "ei_linked_struct.h"
#include "ei_application.h"
#include "ei_pick_id.h"

//  TODO : Add comments
//  TODO : Create an empty rect so each useless movement make a update on nothing :)
//  TODO : Optimize code somewhere to fix slowing
//  TODO : Fix all segfault when getting out of screen
//  TODO : Get rid of all useless variables
//  TODO : Pour Jayson : Relire BIEEEEN LE SUJET

ei_impl_button_t* current_pressed_button = NULL;
ei_impl_toplevel_t* current_selected_toplevel = NULL;
ei_impl_entry_t* current_selected_entry = NULL;
ei_point_t initial_mouse_position_drag;
ei_point_t initial_mouse_position_resize;
//ei_rect_t fusion_rectangle = {{0,0},{0,0}};

bool ei_press_button_down(ei_widget_t widget,
                          struct ei_event_t* event,
                          ei_user_param_t user_param){
    ei_impl_button_t* button = (ei_impl_button_t*) widget;
    button->relief = ei_relief_sunken;
    widget->wclass->drawfunc(widget,root_surface,pick_surface,NULL);
    current_pressed_button = button;
    ei_app_invalidate_rect(ei_widget_get_screen_location(widget));
    ei_bind(ei_ev_mouse_buttonup, NULL, "all",ei_close_toplevel,NULL);
    ei_bind(ei_ev_mouse_buttonup,NULL,"all",ei_press_button_up,NULL);
    ei_bind(ei_ev_mouse_move, NULL, "all", ei_move_back_on_button, NULL);
    ei_bind(ei_ev_mouse_move, NULL, "all", ei_move_out_of_button, NULL);
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
    return false;
}

bool ei_press_button_up(ei_widget_t widget,
                        struct ei_event_t* event,
                        ei_user_param_t user_param){
    if (current_pressed_button != NULL){
        if (strcmp((char*) widget->wclass,"button") == 0){
            current_pressed_button->relief = ei_relief_raised;
            widget->wclass->drawfunc(widget,root_surface,pick_surface,NULL);
            ei_app_invalidate_rect(ei_widget_get_screen_location(widget));
        }
        current_pressed_button = NULL;
        ei_unbind(ei_ev_mouse_buttonup,NULL,"all",ei_press_button_up,NULL);
        ei_unbind(ei_ev_mouse_move, NULL, "all", ei_move_back_on_button, NULL);
        ei_unbind(ei_ev_mouse_move, NULL, "all", ei_move_out_of_button, NULL);
    }
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
    return false;
}

bool ei_move_back_on_button(ei_widget_t widget,
                            struct ei_event_t* event,
                            ei_user_param_t user_param) {
    if (current_pressed_button != NULL){
        if (on_cursor_widget == (ei_widget_t) current_pressed_button){
            current_pressed_button->relief = ei_relief_sunken;
            widget->wclass->drawfunc(widget,root_surface,pick_surface,NULL);
            ei_app_invalidate_rect(ei_widget_get_screen_location(widget));
            return true;
        }
    }
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
    return false;
}

bool ei_move_out_of_button(ei_widget_t widget,
                           struct ei_event_t* event,
                           ei_user_param_t user_param){
//  TODO : Verify documentation (see WhatsApp conversation around 21h 12/05)
    if (current_pressed_button != NULL){
        if (on_cursor_widget != (ei_widget_t) current_pressed_button){
            current_pressed_button->relief = ei_relief_raised;
            widget->wclass->drawfunc(widget,root_surface,pick_surface,NULL);
            ei_app_invalidate_rect(ei_widget_get_screen_location(widget));
            return true;
        }
    }
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
    return false;
}

bool ei_press_entry(ei_widget_t widget,
                    struct ei_event_t* event,
                    ei_user_param_t user_param){
    current_selected_entry = (ei_impl_entry_t*) widget;
    ei_bind(ei_ev_mouse_buttondown,NULL,"all",ei_end_of_write,NULL);
    ei_bind(ei_ev_keydown,NULL,"all",ei_write_entry,NULL);
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
}

bool ei_write_entry(ei_widget_t widget,
                    struct ei_event_t* event,
                    ei_user_param_t user_param){
    current_selected_entry->text += event->param.text;
    ei_app_invalidate_rect(ei_widget_get_screen_location((ei_widget_t) current_selected_entry));
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
    return false;
}

bool ei_end_of_write(ei_widget_t widget,
                     struct ei_event_t* event,
                     ei_user_param_t user_param){
    if (on_cursor_widget != widget){
        current_selected_entry = NULL;
        ei_unbind(ei_ev_mouse_buttondown,NULL,"all",ei_end_of_write,NULL);
        ei_unbind(ei_ev_keydown,NULL,"all",ei_write_entry,NULL);
    }
}

bool ei_press_toplevel(ei_widget_t widget,
                       struct ei_event_t* event,
                       ei_user_param_t user_param){
//  TODO : Changes to do
    if (widget == root_widget){
        return false;
    }
    if (strcmp((char*) widget->wclass,"toplevel") == 0){
        current_selected_toplevel = (ei_impl_toplevel_t*) widget;
        ei_app_invalidate_rect(ei_widget_get_screen_location(widget));
        current_selected_toplevel->widget.wclass->drawfunc((ei_widget_t) current_selected_toplevel,root_surface,pick_surface,
                                                           (ei_rect_t*) ei_widget_get_screen_location(widget));
//      Put the toplevel as the last sibling to be the last drawn
        if (widget != widget->parent->children_tail){
            ei_widget_t previous_widget = NULL;
            ei_widget_t current_widget = widget->parent->children_head;
            while (current_widget != widget){
                previous_widget = current_widget;
                current_widget = current_widget->next_sibling;
            }
            if (widget != widget->parent->children_head){
                previous_widget->next_sibling = current_widget->next_sibling;
            }
            else{
                widget->parent->children_head = current_widget->next_sibling;
            }
            current_widget->next_sibling = NULL;
            widget->parent->children_tail->next_sibling = current_widget;
            widget->parent->children_tail = widget;
        }
    }
    else{
        ei_press_toplevel(widget->parent, event, user_param);
    }
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
    return false;
}

bool ei_close_toplevel(ei_widget_t widget,
                       struct ei_event_t* event,
                       ei_user_param_t user_param){
//  TODO : Fix segmentation fault
    if (strcmp((char*) widget->wclass, "button") == 0){
        ei_impl_button_t* button = (ei_impl_button_t*) widget;
        if (button->is_toplevel_close_button){
            ei_widget_t toplevel = widget->parent;
            ei_app_invalidate_rect(ei_widget_get_screen_location(toplevel));
            ei_widget_destroy(toplevel);
            current_selected_toplevel = NULL;
            ei_unbind(ei_ev_mouse_buttonup,NULL,"all",ei_close_toplevel,NULL);
            return true;
        }
    }
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
    return false;
}

// TODO : Handle case when getting out of screen : widget == NULL

bool point_in_rect(ei_point_t point,
                   ei_rect_t rect){
    return (rect.top_left.x <= point.x && point.x <= rect.top_left.x + rect.size.width &&
            rect.top_left.y <= point.y && point.y <= rect.top_left.y + rect.size.height);
}

bool detect_drag(ei_widget_t widget,
                 ei_event_t event){
    ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*) widget;
    ei_rect_t* rect = widget->screen_location;;
    ei_rect_t header_rect = {rect->top_left, {rect->size.width, HEADER_HEIGHT}};
    return point_in_rect(event.param.mouse.where,header_rect);
}


bool ei_drag_toplevel(ei_widget_t widget,
                      struct ei_event_t* event,
                      ei_user_param_t user_param){
    if (detect_drag(widget,*event)){
        initial_mouse_position_drag = event->param.mouse.where;
        ei_bind(ei_ev_mouse_move,NULL,"all",ei_move_toplevel,NULL);
        ei_bind(ei_ev_mouse_buttonup,NULL,"all",ei_release_toplevel,NULL);
    }
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
    return false;
}

uint32_t test(ei_linked_rect_t* rects){
    uint32_t n = 0;
    while (rects != NULL){
        n++;
        rects = rects->next;
    }
    return n;
}

bool ei_move_toplevel(ei_widget_t widget,
                      struct ei_event_t* event,
                      ei_user_param_t user_param){
    ei_point_t current_mouse_position_drag = event->param.mouse.where;
//    ei_rect_t* old_screen_location = (ei_rect_t*)ei_widget_get_screen_location((ei_widget_t) current_selected_toplevel);
    ei_app_invalidate_rect(ei_widget_get_screen_location((ei_widget_t) current_selected_toplevel));
    int new_x = current_selected_toplevel->widget.content_rect->top_left.x - current_selected_toplevel->border_width + (current_mouse_position_drag.x - initial_mouse_position_drag.x);
    int new_y = current_selected_toplevel->widget.content_rect->top_left.y - HEADER_HEIGHT + (current_mouse_position_drag.y - initial_mouse_position_drag.y);
    ei_place((ei_widget_t) current_selected_toplevel, NULL, &new_x, &new_y, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_app_invalidate_rect(ei_widget_get_screen_location((ei_widget_t) current_selected_toplevel));
    initial_mouse_position_drag = current_mouse_position_drag;
//    ei_rect_t* new_screen_location = (ei_rect_t*)ei_widget_get_screen_location((ei_widget_t) current_selected_toplevel);
//    fusion_rectangle = smallest_containing_rect(*old_screen_location,*new_screen_location);
    return true;
}

bool ei_release_toplevel(ei_widget_t widget,
                      struct ei_event_t* event,
                      ei_user_param_t user_param){
    ei_unbind(ei_ev_mouse_move,NULL,"all",ei_move_toplevel,NULL);
    ei_unbind(ei_ev_mouse_buttonup,NULL,"all",ei_release_toplevel,NULL);
    current_selected_toplevel = NULL;
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
    return true;
}

bool detect_resize(ei_widget_t widget,
                   ei_event_t event){
    ei_rect_t* rect = widget->screen_location;
    ei_rect_t resize_rect = {{rect->top_left.x + rect->size.width - RESIZE_AREA_SIZE, rect->top_left.y + rect->size.height - RESIZE_AREA_SIZE}, {RESIZE_AREA_SIZE, RESIZE_AREA_SIZE}};
    return point_in_rect(event.param.mouse.where,resize_rect);
}


bool ei_resize_toplevel(ei_widget_t widget,
                        struct ei_event_t* event,
                        ei_user_param_t user_param){
//  TODO : Write doc
    if (detect_resize((ei_widget_t) current_selected_toplevel,*event)){
        initial_mouse_position_resize = event->param.mouse.where;
        ei_bind(ei_ev_mouse_move,NULL,"all",ei_resize_move_toplevel,NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", ei_resize_release_toplevel,NULL);
    }
    return false;
}

bool ei_resize_move_toplevel(ei_widget_t widget,
                             struct ei_event_t* event,
                             ei_user_param_t user_param){
//  TODO : Write doc
//  TODO : Handle minimum size
    ei_point_t current_mouse_position_resize = event->param.mouse.where;
    ei_app_invalidate_rect(ei_widget_get_screen_location((ei_widget_t) current_selected_toplevel));
    int new_width = current_selected_toplevel->widget.content_rect->size.width + (current_mouse_position_resize.x - initial_mouse_position_resize.x);
    int new_height = current_selected_toplevel->widget.content_rect->size.height + (current_mouse_position_resize.y - initial_mouse_position_resize.y);
    ei_place((ei_widget_t) current_selected_toplevel, NULL, NULL, NULL, &new_width, &new_height, NULL, NULL, NULL, NULL);
    ei_app_invalidate_rect(ei_widget_get_screen_location((ei_widget_t) current_selected_toplevel));
    initial_mouse_position_resize = current_mouse_position_resize;
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
    return true;
}

bool ei_resize_release_toplevel(ei_widget_t widget,
                                struct ei_event_t* event,
                                ei_user_param_t user_param){
//  TODO : Write doc
    ei_unbind(ei_ev_mouse_move,NULL,"all",ei_resize_move_toplevel,NULL);
    ei_unbind(ei_ev_mouse_buttonup,NULL,"all",ei_resize_release_toplevel,NULL);
    current_selected_toplevel = NULL;
    // TODO : Free rects
//    fusion_rectangle = *ei_widget_get_screen_location(widget);
    return false;
}

void ei_bind_library(void){
    ei_bind(ei_ev_mouse_buttondown,NULL,"entry",ei_press_entry,NULL);
    // TODO : Check if you can optimize this binding
    ei_bind(ei_ev_mouse_buttondown,NULL,"toplevel",ei_resize_toplevel,NULL);
    ei_bind(ei_ev_mouse_buttondown,NULL,"toplevel",ei_drag_toplevel,NULL);
    ei_bind(ei_ev_mouse_buttondown,NULL,"all",ei_press_toplevel,NULL);
    ei_bind(ei_ev_mouse_buttondown,NULL,"button",ei_press_button_down,NULL);
//  TODO : Other bindingsei_ev_mouse_buttondown
}

void ei_unbind_library(void){
    ei_unbind(ei_ev_mouse_buttondown,NULL,"entry",ei_press_entry,NULL);
    ei_unbind(ei_ev_mouse_buttondown,NULL,"button",ei_press_button_down,NULL);
    ei_unbind(ei_ev_mouse_buttondown,NULL,"all",ei_press_toplevel,NULL);
    ei_unbind(ei_ev_mouse_buttondown,NULL,"toplevel",ei_drag_toplevel,NULL);
    ei_unbind(ei_ev_mouse_buttondown,NULL,"toplevel",ei_resize_toplevel,NULL);
//  TODO : Other unbindings
}