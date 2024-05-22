#include "ei_implementation.h"
#include "ei_widget_attributes.h"
#include "ei_application.h"
#include "ei_implem_frame.h"
#include "ei_implem_toplevel.h"

void ei_impl_widget_draw_children(ei_widget_t widget,
                                  ei_surface_t surface,
                                  ei_surface_t pick_surface,
                                  ei_rect_t*	clipper){
//  Base case
    if (widget == NULL){
        return;
    }
    ei_widget_t current_widget = widget->children_head;
    while (current_widget != NULL){
        current_widget->wclass->drawfunc(current_widget,surface,pick_surface,clipper);
        ei_surface_t current_surface = hw_surface_create(ei_app_root_surface(),(*current_widget).requested_size,true);
        ei_rect_t* current_clipper = (ei_rect_t*) ei_widget_get_content_rect(current_widget);
        ei_impl_widget_draw_children(current_widget,current_surface,pick_surface,current_clipper);
        current_widget = widget->next_sibling;
        hw_surface_free(current_surface);
    }
}

void ei_set_screen_location(ei_widget_t widget,
                            ei_size_t size){
//  TODO : Write documentation of function
    ei_rect_t* new_screen_location = malloc(sizeof(ei_rect_t));
    if (widget->screen_location) {
        new_screen_location->top_left = widget->screen_location->top_left;
    }
    else {
        new_screen_location->top_left = (ei_point_t){0,0};
    }
    new_screen_location->size = size;
    widget->screen_location = new_screen_location;
}

void ei_set_content_rect(ei_widget_t widget,
                         ei_size_t size){
//  TODO : Write documentation of function
    ei_rect_t* new_content_rect = widget->content_rect;
    if(widget->content_rect == NULL) {
        new_content_rect = malloc(sizeof(ei_rect_t));
    }
    new_content_rect->top_left = widget->screen_location->top_left;
    new_content_rect->size = size;
    if(strcmp(widget->wclass->name,"toplevel") == 0 ) {
        ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*)widget;
        new_content_rect->top_left.x += toplevel->border_width;
        new_content_rect->top_left.y += HEADER_HEIGHT;
        new_content_rect->size.width -= 2 * toplevel->border_width;
        new_content_rect->size.height -= toplevel->border_width + HEADER_HEIGHT;
    }else if(strcmp(widget->wclass->name,"frame") == 0) {
        ei_impl_frame_t* frame = (ei_impl_frame_t*)widget;
        new_content_rect->top_left.x += frame->border_width;
        new_content_rect->top_left.y += frame->border_width;
        new_content_rect->size.width -= 2 * frame->border_width;
        new_content_rect->size.height -= 2 * frame->border_width;
    }
// Button and entry are not supposed to have other widgets in them
    widget->content_rect = new_content_rect;
}

ei_rect_t* intersect_rect(ei_rect_t* rect1, ei_rect_t* rect2) {
    ei_rect_t* intersect = malloc(sizeof(ei_rect_t));

// Calculate the top left point of the intersection rectangle
    intersect->top_left.x = rect1->top_left.x > rect2->top_left.x ? rect1->top_left.x : rect2->top_left.x;
    intersect->top_left.y = rect1->top_left.y > rect2->top_left.y ? rect1->top_left.y : rect2->top_left.y;

// Calculate the bottom right point of the intersection rectangle
    int rect1_bottom_right_x = rect1->top_left.x + rect1->size.width;
    int rect2_bottom_right_x = rect2->top_left.x + rect2->size.width;
    int rect1_bottom_right_y = rect1->top_left.y + rect1->size.height;
    int rect2_bottom_right_y = rect2->top_left.y + rect2->size.height;

// Calculate the size of the intersection rectangle
    intersect->size.width = (rect1_bottom_right_x < rect2_bottom_right_x ? rect1_bottom_right_x : rect2_bottom_right_x) - intersect->top_left.x;
    intersect->size.height = (rect1_bottom_right_y < rect2_bottom_right_y ? rect1_bottom_right_y : rect2_bottom_right_y) - intersect->top_left.y;

    // If the rectangles do not intersect, return a rectangle with zero width and height
    if (intersect->size.width < 0 || intersect->size.height < 0) {
        intersect->size.width = 0;
        intersect->size.height = 0;
    }

    return intersect;
}

void ei_free_widget_attributes(ei_widget_t widget){
    if(widget->screen_location != NULL) {
        free(widget->screen_location);
    }
    if(widget->content_rect != NULL) {
        free(widget->content_rect);
    }
    if(widget->geom_params != NULL) {
        widget->geom_params->manager->releasefunc(widget);
    }
    if(widget->pick_color != NULL) {
        free(widget->pick_color);
    }
//    if(widget->user_data != NULL) {
//        free(widget->user_data);
//    }
}

void ei_widget_set_default_parms(ei_widget_t widget){
    if(widget->content_rect == NULL){
        widget->content_rect = calloc(1,sizeof(ei_rect_t));
    }else{
        *widget->content_rect = (ei_rect_t){{0,0},{0,0}};
    }

    if(widget->screen_location == NULL){
        widget->screen_location = calloc(1,sizeof(ei_rect_t));
    }else{
        *widget->screen_location = (ei_rect_t){{0,0},{0,0}};
    }

}

ei_rect_t smallest_containing_rect(ei_rect_t rect1, ei_rect_t rect2) {
    ei_rect_t res;

    // Calculate the top left point of the resulting rectangle
    res.top_left.x = (rect1.top_left.x < rect2.top_left.x) ? rect1.top_left.x : rect2.top_left.x;
    res.top_left.y = (rect1.top_left.y < rect2.top_left.y) ? rect1.top_left.y : rect2.top_left.y;

    // Calculate the bottom right point of both rectangles
    ei_point_t bottom_right1 = {rect1.top_left.x + rect1.size.width, rect1.top_left.y + rect1.size.height};
    ei_point_t bottom_right2 = {rect2.top_left.x + rect2.size.width, rect2.top_left.y + rect2.size.height};

    // Calculate the size of the resulting rectangle
    res.size.width = ((bottom_right1.x > bottom_right2.x) ? bottom_right1.x : bottom_right2.x) - res.top_left.x;
    res.size.height = ((bottom_right1.y > bottom_right2.y) ? bottom_right1.y : bottom_right2.y) - res.top_left.y;

    return res;
}