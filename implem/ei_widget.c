#include "ei_class.h"
#include "ei_implementation.h"
#include "ei_application.h"
#include "ei_widget_attributes.h"

uint32_t current_pick_id = 0;

bool ei_widgetclass_is_registered(ei_widgetclass_t* widgetclass){
    ei_widgetclass_t* current = registered_widget_classes;
    while(current != NULL){
        if(current == widgetclass){
            return true;
        }
        current = current->next;
    }
    return false;
}

ei_widget_t ei_widget_create(ei_const_string_t class_name,
                             ei_widget_t parent,
                             ei_user_param_t user_data,
                             ei_widget_destructor_t destructor){
    ei_widgetclass_t* widgetclass = ei_widgetclass_from_name(class_name);
// Test if the class_name is registered in the application if not return NUL
    if(!ei_widgetclass_is_registered(widgetclass)){
        return NULL;
    }
// Allocate the needed memory for the widget
    ei_widget_t widget = widgetclass->allocfunc();
// Set the default values of the widget
    widgetclass->setdefaultsfunc(widget);
// Affect attributes to the widget
    widget->wclass = widgetclass;
    widget->pick_id = current_pick_id;
    current_pick_id++;
    widget->parent = parent;
    if(strcmp(widget->wclass->name,"toplevel") != 0){// If the widget is a toplevel the close button is already created
        widget->children_head = NULL;
        widget->children_tail = NULL;
    }
    widget-> next_sibling = NULL;
    widget->user_data = user_data;
    widget->destructor = destructor;
// Add the widget to the children of the parent if it exists
    if(parent != NULL){
        if(parent->children_head == NULL){
            parent->children_head = widget;
            parent->children_tail = widget;
        }
        else{
            parent->children_tail->next_sibling = widget;
            parent->children_tail = widget;
        }
    }
    return widget;
}

void ei_widget_destroy(ei_widget_t widget){
//  TODO : Need to update root
// Call the destructor of the widget if it exists
    if(widget->destructor != NULL){
        widget->destructor(widget);
//Mybe we should return here ?
    }
//  If the widget had a parent, notify they lost a children
    if (widget != root_widget){
//  Differents cases : the widget was children head
        if (widget == widget->parent->children_head){
            widget->parent->children_head = widget->next_sibling;
        }
//  The widget was children tail
        else if (widget == widget->parent->children_tail){
            ei_widget_t current_widget = widget->parent->children_head;
            ei_widget_t previous_widget = NULL;
            while (current_widget != widget){
                previous_widget = current_widget;
                current_widget = current_widget->next_sibling;
            }
            widget->parent->children_tail = previous_widget;
            widget->parent->children_tail->next_sibling = NULL;
        }
//  Other case
        else{
            ei_widget_t current_widget = widget->parent->children_head;
            ei_widget_t previous_widget = NULL;
            while (current_widget != widget){
                previous_widget = current_widget;
                current_widget = current_widget->next_sibling;
            }
            previous_widget->next_sibling = current_widget->next_sibling;
        }

    }
    ei_app_invalidate_rect(ei_widget_get_screen_location(widget));
// Destroy all the children of the widget
    ei_widget_t current_widget = widget->children_head;
    while(current_widget != NULL){
        ei_widget_t next_sibling = current_widget->next_sibling;
        ei_widget_destroy(current_widget);
        current_widget = next_sibling;
    }
// Free the memory used by the widget
    root_widget->wclass->drawfunc(root_widget,root_surface,pick_surface, (ei_rect_t*) ei_widget_get_screen_location(widget));
    widget->wclass->releasefunc(widget);
}

bool ei_widget_is_displayed(ei_widget_t widget){
    return widget->parent != NULL;
}
