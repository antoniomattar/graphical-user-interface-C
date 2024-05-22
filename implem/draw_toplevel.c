//
// Created by matteo on 5/13/24.
//

#include "draw_toplevel.h"
#include "ei_implementation.h"
#include "ei_implem_toplevel.h"
#include "ei_draw.h"
#include "ei_draw_tools.h"


void draw_toplevel_background(ei_surface_t surface, ei_rect_t* rect, int border_thickness, ei_color_t color, ei_rect_t* clipper){

    // Draw the background
    ei_point_t background_top_left = {rect->top_left.x + border_thickness, rect->top_left.y + border_thickness};
    ei_size_t background_size = {rect->size.width - 2 * border_thickness, rect->size.height - 2 * border_thickness};
    ei_rect_t background_rect = {background_top_left, background_size};
    ei_fill(surface, &color, &background_rect);
}


void draw_toplevel_border(ei_surface_t surface,
                          ei_rect_t* rect, int border_thickness,
                          ei_color_t color, ei_rect_t* clipper){
    // Draw bottom border
//    ei_point_t bottom_left = {rect->top_left.x, rect->top_left.y + rect->size.height - border_thickness};
//    ei_rect_t bottom_border = {bottom_left, {rect->size.width, border_thickness}};
//    ei_fill(surface, &color, &bottom_border);

    // Draw a first background
    ei_rect_t left_border = {rect->top_left, {rect->size.width, rect->size.height}};
    ei_fill(surface, &color, &left_border);

    // Draw right border
//    ei_point_t top_right = {rect->top_left.x + rect->size.width - border_thickness, rect->top_left.y};
//    ei_rect_t right_border = {top_right, {border_thickness, rect->size.height}};
//    ei_fill(surface, &color, &right_border);
}


void draw_toplevel(ei_widget_t widget,
                   ei_surface_t surface,
                   ei_rect_t* clipper){
//     Get the toplevel attributes
    ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*) widget;
    char* title = toplevel->title;
    ei_rect_t rect = (ei_rect_t){{widget->content_rect->top_left.x - toplevel->border_width, widget->content_rect->top_left.y - HEADER_HEIGHT}, {widget->content_rect->size.width + 2 * toplevel->border_width, widget->content_rect->size.height + HEADER_HEIGHT + toplevel->border_width}};

    int border_thickness = toplevel->border_width;

//     Draw the border
    ei_rect_t background_rect = {{rect.top_left.x, rect.top_left.y + HEADER_HEIGHT}, {rect.size.width , rect.size.height - HEADER_HEIGHT}};
    ei_fill(surface, &BOARDER_COLOR, &background_rect);

//     Draw the background
    draw_toplevel_background(surface, widget->content_rect, border_thickness, toplevel->color, clipper);


//     Draw the header bar
    ei_rect_t header_rect = (ei_rect_t){rect.top_left, {rect.size.width, HEADER_HEIGHT}};
    struct ei_linked_point_t* header_points = rounded_frame(header_rect, 10, true, false);
    uint32_t header_points_size = size_of_linked_point(header_points);
    ei_point_t* header_points_array = linked_to_array(header_points, header_points_size);
    ei_draw_polygon(surface, header_points_array, header_points_size, BOARDER_COLOR, &header_rect);
    free_linked_points(header_points);
    free(header_points_array);


//     Draw the title
    ei_point_t text_point = {rect.top_left.x + 2 * PADDING + CLOSE_BUTTON_SIZE, rect.top_left.y};
    ei_draw_text(surface, &text_point, title, ei_default_font, (ei_color_t){0x00, 0x00, 0x00, 0xff}, &header_rect);

    // Draw the resize area
    ei_rect_t resize_rect = {{rect.top_left.x + rect.size.width - RESIZE_AREA_SIZE, rect.top_left.y + rect.size.height - RESIZE_AREA_SIZE}, {RESIZE_AREA_SIZE, RESIZE_AREA_SIZE}};
    ei_color_t resize_color = adjust_color_intensity(BOARDER_COLOR, 0.9);
    ei_fill(surface, &resize_color, &resize_rect);
}