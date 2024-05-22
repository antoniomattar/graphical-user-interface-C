#include "ei_geometrymanager.h"

#include <ei_application.h>
#include <ei_placer.h>

#include "ei_implementation.h"
#include "ei_implem_toplevel.h"
#include "ei_implem_entry.h"
#include "ei_implem_frame.h"
#include "ei_implem_button.h"

void update_content_rect(ei_widget_t widget) {
//    Check the widget type and correspondingly update the content rect
    if(strcmp(widget->wclass->name,"toplevel") == 0){
        ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*)widget;
        if(widget->content_rect == NULL) {
            widget->content_rect = calloc(1,sizeof(ei_rect_t));
        }
        *widget->content_rect = (ei_rect_t){{widget->screen_location->top_left.x + toplevel->border_width,widget->screen_location->top_left.y + HEADER_HEIGHT},{widget->screen_location->size.width - 2 * toplevel->border_width,widget->screen_location->size.height - toplevel->border_width - HEADER_HEIGHT}};
    }

    if (strcmp(widget->wclass->name,"button") == 0) {
        ei_impl_button_t* button = (ei_impl_button_t*)widget;
        if(widget->content_rect == NULL) {
            widget->content_rect = calloc(1,sizeof(ei_rect_t));
        }
        *widget->content_rect = (ei_rect_t){{widget->screen_location->top_left.x + button->border_width,widget->screen_location->top_left.y + button->border_width},{widget->screen_location->size.width - 2 * button->border_width,widget->screen_location->size.height - 2 * button->border_width}};
    }

    if (strcmp(widget->wclass->name,"frame") == 0) {
        ei_impl_frame_t* frame = (ei_impl_frame_t*)widget;
        if(widget->content_rect == NULL) {
            widget->content_rect = calloc(1,sizeof(ei_rect_t));
        }
        *widget->content_rect = (ei_rect_t){{widget->screen_location->top_left.x + frame->border_width,widget->screen_location->top_left.y + frame->border_width},{widget->screen_location->size.width - 2 * frame->border_width,widget->screen_location->size.height - 2 * frame->border_width}};
    }

    if (strcmp(widget->wclass->name,"entry") == 0) {
        ei_impl_entry_t* entry = (ei_impl_entry_t*)widget;
        if(widget->content_rect == NULL) {
            widget->content_rect = calloc(1,sizeof(ei_rect_t));
        }
        *widget->content_rect = (ei_rect_t){{widget->screen_location->top_left.x + entry->border_width,widget->screen_location->top_left.y + entry->border_width},{widget->screen_location->size.width - 2 * entry->border_width,widget->screen_location->size.height - 2 * entry->border_width}};
    }
}



ei_geometrymanager_t* registered_geometrymanager = NULL;

bool ei_geometrymanager_is_registered(ei_geometrymanager_t* geometrymanager) {
    ei_geometrymanager_t* current = registered_geometrymanager;
    while(current != NULL){
        if(current == geometrymanager){
            return true;
        }
        current = current->next;
    }
    return false;
}

size_t ei_geom_param_size() {
    return sizeof(ei_impl_geom_param_t);
}

void ei_geometry_run_finalize(ei_widget_t widget, ei_rect_t* new_screen_location) {
    if (widget->screen_location == NULL) {
        widget->screen_location = calloc(1, sizeof(ei_rect_t));
    }
//    ei_app_invalidate_rect(new_screen_location);

    if (new_screen_location->size.height != widget->screen_location->size.height
        || new_screen_location->size.width != widget->screen_location->size.width
        || new_screen_location->top_left.x != widget->screen_location->top_left.x
        || new_screen_location->top_left.y != widget->screen_location->top_left.y) {
//        ei_app_invalidate_rect(widget->screen_location);

        *(widget->screen_location) = *new_screen_location;

        update_content_rect(widget);

//        Adjust geometry of all children with their own geometry manager
        ei_widget_t current = widget->children_head;
        while (current != NULL) {
            (*(current->geom_params->manager->runfunc))(current);
            current = current->next_sibling;
        }
    }
}
void ei_geometrymanager_register(ei_geometrymanager_t* geometrymanager) {
    geometrymanager->next = NULL;
    if(registered_geometrymanager == NULL) {
        registered_geometrymanager = geometrymanager;
    }else {
        ei_geometrymanager_t* current = registered_geometrymanager;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = geometrymanager;
    }
}

ei_geometrymanager_t* ei_geometrymanager_from_name(ei_geometrymanager_name_t name) {
    ei_geometrymanager_t* current = registered_geometrymanager;
    while(current != NULL){
        if(strcmp(current->name,name) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void ei_geometrymanager_unmap(ei_widget_t widget) {
    if(widget->geom_params != NULL) {
        (*widget->geom_params->manager->releasefunc)(widget);
    }
    widget->geom_params = NULL;
    if(widget->screen_location != NULL) {
        ei_app_invalidate_rect(widget->screen_location);
    }
}

ei_geometrymanager_t* ei_widget_get_geom_manager(ei_widget_t widget) {
    return widget->geom_params ? widget->geom_params->manager : NULL;
}

void ei_widget_set_geom_manager(ei_widget_t widget, ei_geometrymanager_t* manager) {
    widget->geom_params->manager = manager;
}

ei_geom_param_t ei_widget_get_geom_params(ei_widget_t widget) {
    return widget->geom_params;
}

void ei_widget_set_geom_params(ei_widget_t widget, ei_geom_param_t geom_param) {
    widget->geom_params = geom_param;
}
