//
// Created by matteo on 5/15/24.
//

#include "draw_frame.h"
#include "ei_draw_tools.h"
#include "ei_implementation.h"
#include "ei_implem_frame.h"
#include "ei_draw.h"

#define DARKENING_FACTOR 5

void move_triangle(ei_point_t* point, int dx, int dy){
    for(int i = 0; i < 3; i++){
        point[i].x += dx;
        point[i].y += dy;
    }
}

void move_square(ei_point_t* point, int dx, int dy){
    for(int i = 0; i < 4; i++){
        point[i].x += dx;
        point[i].y += dy;
    }
}

void draw_border(ei_surface_t surface, ei_rect_t* rect, int border_thickness, ei_color_t color, float darkening_factor, ei_rect_t* clipper) {
    ei_color_t dark_color = adjust_color_intensity(color, -darkening_factor);
    ei_color_t light_color = adjust_color_intensity(color, darkening_factor);
    // Define all part of the border
    // Top border
    ei_rect_t top_border = {{rect->top_left.x + border_thickness, rect->top_left.y }, {rect->size.width - 2 * border_thickness, border_thickness}};
    // Bottom border
    ei_rect_t bottom_border = {{rect->top_left.x + border_thickness, rect->top_left.y + rect->size.height - border_thickness}, {rect->size.width - 2 * border_thickness, border_thickness}};
    // Left border
    ei_rect_t left_border = {{rect->top_left.x, rect->top_left.y + border_thickness}, {border_thickness, rect->size.height - 2 * border_thickness}};
    // Right border
    ei_rect_t right_border = {{rect->top_left.x + rect->size.width - border_thickness, rect->top_left.y + border_thickness}, {border_thickness, rect->size.height - 2 * border_thickness}};

    // Define geometric shapes that will be used to draw the corners
    // Upper triangle
    ei_point_t upper_triangle_1 = {rect->top_left.x, rect->top_left.y};
    ei_point_t upper_triangle_2 = {rect->top_left.x, rect->top_left.y + border_thickness};
    ei_point_t upper_triangle_3 = {rect->top_left.x + border_thickness, rect->top_left.y};
    ei_point_t upper_triangle[3] = {upper_triangle_1, upper_triangle_2, upper_triangle_3};
    // Lower triangle
    ei_point_t lower_triangle_1 = {rect->top_left.x + border_thickness, rect->top_left.y};
    ei_point_t lower_triangle_2 = {rect->top_left.x, rect->top_left.y + border_thickness};
    ei_point_t lower_triangle_3 = {rect->top_left.x + border_thickness, rect->top_left.y + border_thickness};
    ei_point_t lower_triangle[3] = {lower_triangle_1, lower_triangle_2, lower_triangle_3};
    // Square
    ei_point_t square[4] = {upper_triangle_1, lower_triangle_2, lower_triangle_3, upper_triangle_3};

    ei_fill(surface, &light_color, &top_border);
    ei_fill(surface, &light_color, &left_border);
    ei_fill(surface, &dark_color, &bottom_border);
    ei_fill(surface, &dark_color, &right_border);
    //top left corner
    ei_draw_polygon(surface, square, 4, light_color, clipper);
    //bottom right corner
    move_square(square,rect->size.width - border_thickness - 1, rect->size.height - border_thickness - 1);
    ei_draw_polygon(surface, square, 4, dark_color, clipper);

    if(darkening_factor > 0) { // We draw the light triangle last so that it is "on top" on the diagonal
        //top right corner
        move_triangle(upper_triangle, rect->size.width - border_thickness - 1, 0);
        move_triangle(lower_triangle, rect->size.width - border_thickness - 1, 0);
        ei_draw_polygon(surface, lower_triangle, 3, dark_color, clipper);
        ei_draw_polygon(surface, upper_triangle, 3, light_color, clipper);

        //bottom left corner
        move_triangle(upper_triangle, -(rect->size.width - border_thickness - 1), rect->size.height - border_thickness - 1);
        move_triangle(lower_triangle, -(rect->size.width - border_thickness - 1), rect->size.height - border_thickness - 1);
        ei_draw_polygon(surface, lower_triangle, 3, dark_color, clipper);
        ei_draw_polygon(surface, upper_triangle, 3, light_color, clipper);
    }else {
        //top right corner
        move_triangle(upper_triangle, rect->size.width - border_thickness - 1, 0);
        move_triangle(lower_triangle, rect->size.width - border_thickness - 1, 0);
        ei_draw_polygon(surface, upper_triangle, 3, light_color, clipper);
        ei_draw_polygon(surface, lower_triangle, 3, dark_color, clipper);

        //bottom left corner
        move_triangle(upper_triangle, -(rect->size.width - border_thickness - 1), rect->size.height - border_thickness - 1);
        move_triangle(lower_triangle, -(rect->size.width - border_thickness - 1), rect->size.height - border_thickness - 1);
        ei_draw_polygon(surface, upper_triangle, 3, light_color, clipper);
        ei_draw_polygon(surface, lower_triangle, 3, dark_color, clipper);

    }
}

void draw_frame(ei_widget_t widget, ei_surface_t surface, ei_rect_t* clipper){
//     Cast and Fetch the frame attributes
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;
    ei_rect_t* entire_widget_rect = widget->screen_location;
    int border_thickness = frame->border_width;
    ei_color_t frame_color = frame->color;
    ei_relief_t relief = frame->relief;
    ei_surface_t img = frame->img;

//  ======= The Frame has NO relief =============================================
    if (relief == ei_relief_none || border_thickness == 0){
//         Manage image
        if(img != NULL) {
//            Draw the image while using the screen_location as the clipper
            draw_image(surface,entire_widget_rect, img, frame->img_rect, 0, entire_widget_rect);
        }else {
//             Draw the background (inside the entire widget)
            ei_fill(surface, &frame_color, entire_widget_rect);
//             Draw the text
            if(frame->text != NULL) {
                ei_rect_t* text_clipper = intersect_rect(entire_widget_rect, root_widget->screen_location);
                ei_point_t text_anchor = calculate_text_position(entire_widget_rect, frame->text, frame->text_font, frame->text_anchor);
                //TODO check if text dont overflow
                ei_draw_text(surface, &text_anchor, frame->text, frame->text_font, frame->text_color, text_clipper);
                free(text_clipper);
            }

        }
        return;
    }

//  ======= The Frame has a relief =============================================
//     Draw the border
    if(relief == ei_relief_raised) {
        draw_border(surface, entire_widget_rect, border_thickness, frame_color, DARKENING_FACTOR, clipper);
    }else {
        draw_border(surface, entire_widget_rect, border_thickness, frame_color, -DARKENING_FACTOR, clipper);
    }
    if(img != NULL) {
//        Draw the image while using the content_rect as the clipper
        draw_image(surface,widget->content_rect, img, frame->img_rect, border_thickness, widget->content_rect);
    }else {
        // Draw the background (inside the border)
        ei_point_t background_top_left = {entire_widget_rect->top_left.x + border_thickness, entire_widget_rect->top_left.y + border_thickness};
        ei_size_t background_size = {entire_widget_rect->size.width - 2 * border_thickness, entire_widget_rect->size.height - 2 * border_thickness};
        ei_rect_t background_rect = {background_top_left, background_size};
        ei_fill(surface, &frame_color, &background_rect);

//         Draw the text
        if(frame->text != NULL) {
//          Calculate the text position
            ei_point_t text_anchor = calculate_text_position(entire_widget_rect, frame->text, frame->text_font, frame->text_anchor);
            ei_draw_text(surface, &text_anchor, frame->text, frame->text_font, frame->text_color, &background_rect);
        }

    }
}