#include "ei_widget_attributes.h"
#include "ei_implementation.h"

ei_widgetclass_t* ei_widget_get_class(ei_widget_t widget){
    //TODO
    return NULL;
}

const ei_color_t* ei_widget_get_pick_color(ei_widget_t widget){
    //TODO
    return NULL;
}

ei_widget_t ei_widget_get_parent(ei_widget_t widget){
    //TODO
    return NULL;
}

ei_widget_t ei_widget_get_first_child(ei_widget_t widget){
    //TODO
    return NULL;
}

ei_widget_t ei_widget_get_last_child(ei_widget_t widget){
    //TODO
    return NULL;
}

ei_widget_t ei_widget_get_next_sibling(ei_widget_t widget){
    //TODO
    return NULL;
}

void* ei_widget_get_user_data(ei_widget_t widget){
    //TODO
    return NULL;
}

const ei_size_t* ei_widget_get_requested_size(ei_widget_t widget){
    //TODO
    return NULL;
}

void ei_widget_set_requested_size(ei_widget_t widget,
                                  ei_size_t requested_size){
    //TODO
}


const ei_rect_t* ei_widget_get_screen_location(ei_widget_t widget){
    return &(widget->screen_location);
}

const ei_rect_t* ei_widget_get_content_rect(ei_widget_t widget){
    return widget->content_rect;
}

void ei_widget_set_content_rect(ei_widget_t	widget,
                                const ei_rect_t* content_rect){
    widget->content_rect = (ei_rect_t*) content_rect;
}