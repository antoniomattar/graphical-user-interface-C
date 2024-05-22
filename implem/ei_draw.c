#include "ei_draw.h"
#include "ei_implementation.h"

void adjust_pixel_pointer(uint8_t** pixel, const ei_rect_t* rect, const ei_size_t size, int channels) {
    if (rect) {
        *pixel += (rect->top_left.y * size.width + rect->top_left.x) * channels;
    }
}

int ei_copy_surface(ei_surface_t destination, const ei_rect_t* dst_rect, ei_surface_t source, const ei_rect_t* src_rect, bool alpha) {
    int red_idx, green_idx, blue_idx, alpha_idx;
    hw_surface_get_channel_indices(destination, &red_idx, &green_idx, &blue_idx, &alpha_idx);

//     Lock surfaces and retrieve buffers and dimensions.
    hw_surface_lock(source);
    hw_surface_lock(destination);
    uint8_t* src_pixel = hw_surface_get_buffer(source);
    uint8_t* dst_pixel = hw_surface_get_buffer(destination);
    ei_size_t src_size = hw_surface_get_size(source);
    ei_size_t dst_size = hw_surface_get_size(destination);

//     Adjust source and destination pixel pointers.
    adjust_pixel_pointer(&src_pixel, src_rect, src_size, 4);
    adjust_pixel_pointer(&dst_pixel, dst_rect, dst_size, 4);

//     Calculate dimensions and verify matching sizes.
    ei_size_t real_src_size = src_rect ? src_rect->size : src_size;
    ei_size_t real_dst_size = dst_rect ? dst_rect->size : dst_size;
    if (real_src_size.width != real_dst_size.width || real_src_size.height != real_dst_size.height) {
        hw_surface_unlock(destination);
        hw_surface_unlock(source);
        return 1;
    }


//     Set stride to skip to the next line.
    uint32_t src_stride = (src_size.width - real_src_size.width) * 4;
    uint32_t dst_stride = (dst_size.width - real_dst_size.width) * 4;

//     Process each pixel for copy or alpha blend.
    for (uint32_t y = 0; y < real_src_size.height; y++) {
        for (uint32_t x = 0; x < real_src_size.width; x++) {
            if (alpha == true) {
                uint8_t src_alpha = src_pixel[alpha_idx];
                dst_pixel[red_idx] = (dst_pixel[red_idx] * (255 - src_alpha) + src_pixel[red_idx] * src_alpha) / 255;
                dst_pixel[green_idx] = (dst_pixel[green_idx] * (255 - src_alpha) + src_pixel[green_idx] * src_alpha) / 255;
                dst_pixel[blue_idx] = (dst_pixel[blue_idx] * (255 - src_alpha) + src_pixel[blue_idx] * src_alpha) / 255;
                dst_pixel[alpha_idx] = 255;
            } else {
                dst_pixel[red_idx] = src_pixel[red_idx];
                dst_pixel[green_idx] = src_pixel[green_idx];
                dst_pixel[blue_idx] = src_pixel[blue_idx];
                dst_pixel[alpha_idx] = src_pixel[alpha_idx];
            }
            src_pixel += 4;
            dst_pixel += 4;
        }
        src_pixel += src_stride;
        dst_pixel += dst_stride;
    }

//    Unlock the surfaces
    hw_surface_unlock(destination);
    hw_surface_unlock(source);
    return 0;
}

// Takes a rectangle and a surface and returns the intersection rectangle with the surface
ei_rect_t* intersection_rect_with_surface(
        const ei_rect_t* rect,
        ei_surface_t surface){
//     Get the size of the surface
    ei_size_t surface_size = hw_surface_get_size(surface);
//     Get the top left corner of the rectangle
    ei_point_t rect_top_left = rect->top_left;
//     Get the bottom right corner of the rectangle
    ei_point_t bottom_right = {rect_top_left.x + rect->size.width, rect_top_left.y + rect->size.height};
//     Get the top left corner of the surface
    ei_point_t surface_top_left = {0, 0};
//     Get the bottom right corner of the surface
    ei_point_t surface_bottom_right = {surface_size.width, surface_size.height};
//     Get the top left corner of the intersection rectangle
    ei_point_t intersection_top_left = {rect_top_left.x > surface_top_left.x ? rect_top_left.x : surface_top_left.x,
                                        rect_top_left.y > surface_top_left.y ? rect_top_left.y : surface_top_left.y};
//     Get the bottom right corner of the intersection rectangle
    ei_point_t intersection_bottom_right = {bottom_right.x < surface_bottom_right.x ? bottom_right.x : surface_bottom_right.x,
                                            bottom_right.y < surface_bottom_right.y ? bottom_right.y : surface_bottom_right.y};
//     Check if the intersection rectangle is valid
    if (intersection_top_left.x >= intersection_bottom_right.x || intersection_top_left.y >= intersection_bottom_right.y){
        return NULL;
    }
//     Create the intersection rectangle
    ei_rect_t* intersection_rect = malloc(sizeof(ei_rect_t));
    intersection_rect->top_left = intersection_top_left;
    intersection_rect->size.width = intersection_bottom_right.x - intersection_top_left.x;
    intersection_rect->size.height = intersection_bottom_right.y - intersection_top_left.y;
    //TODO: attention free this after usage
    return intersection_rect;
}

ei_rect_t intersection_rect_with_rect(const ei_rect_t* rect1, const ei_rect_t* rect2){
    if(rect1 == NULL && rect2 != NULL) {
        return *rect2;
    }
    if(rect2 == NULL && rect1 != NULL) {
        return *rect1;
    }
    if(rect1 == NULL && rect2 == NULL) {
        return (ei_rect_t){{0,0},{0,0}};
    }
    // Get the top left corner of the rectangle 1
    ei_point_t rect1_top_left = rect1->top_left;
//     Get the bottom right corner of the rectangle 1
    ei_point_t rect1_bottom_right = {rect1_top_left.x + rect1->size.width, rect1_top_left.y + rect1->size.height};
//     Get the top left corner of the rectangle 2
    ei_point_t rect2_top_left = rect2->top_left;
//     Get the bottom right corner of the rectangle 2
    ei_point_t rect2_bottom_right = {rect2_top_left.x + rect2->size.width, rect2_top_left.y + rect2->size.height};
//     Get the top left corner of the intersection rectangle
    ei_point_t intersection_top_left = {rect1_top_left.x > rect2_top_left.x ? rect1_top_left.x : rect2_top_left.x,
                                        rect1_top_left.y > rect2_top_left.y ? rect1_top_left.y : rect2_top_left.y};
//     Get the bottom right corner of the intersection rectangle
    ei_point_t intersection_bottom_right = {rect1_bottom_right.x < rect2_bottom_right.x ? rect1_bottom_right.x : rect2_bottom_right.x,
                                            rect1_bottom_right.y < rect2_bottom_right.y ? rect1_bottom_right.y : rect2_bottom_right.y};
//     Check if the intersection rectangle is valid
    if (intersection_top_left.x >= intersection_bottom_right.x || intersection_top_left.y >= intersection_bottom_right.y){
        return (ei_rect_t){{0, 0}, {0, 0}};
    }
//     Create the intersection rectangle
    ei_rect_t intersection_rect;
    intersection_rect.top_left = intersection_top_left;
    intersection_rect.size.width = intersection_bottom_right.x - intersection_top_left.x;
    intersection_rect.size.height = intersection_bottom_right.y - intersection_top_left.y;
    return intersection_rect;
}

void ei_draw_text(ei_surface_t surface,
               const ei_point_t* where,
               ei_const_string_t text,
               ei_font_t font,
               ei_color_t color,
               const ei_rect_t* clipper){
//         Trivial case
    if (text == NULL || strcmp(text,"") == 0){
        return;
    }

//     If the text font is NULL, the default font is used
    if (font == NULL){
        font = ei_default_font;
    }

//     Create a text surface
    ei_surface_t text_surface = hw_text_create_surface(text, font, color);

//     Define the destination and source rectangles
    ei_rect_t destination_rect = {*where, hw_surface_get_size(text_surface)};
    ei_rect_t source_rect = hw_surface_get_rect(text_surface);

//     Calculate the size of the text
    ei_size_t text_size;
    hw_text_compute_size(text, font, &text_size.width, &text_size.height);

//     Clip the text surface if necessary
//    TODO: causing problem with text
//    if (clipper) {
//        destination_rect = intersection_rect_with_rect(&destination_rect,clipper);
//    }

//     Lock the surface
    hw_surface_lock(surface);
    hw_surface_lock(text_surface);

//     Copy text surface to the destination surface
    ei_copy_surface(surface, &destination_rect, text_surface, &source_rect, true);

//     Unlock the surface
    hw_surface_unlock(text_surface);
    hw_surface_unlock(surface);
//     Free the text surface
    hw_surface_free(text_surface);
}

void ei_fill(ei_surface_t surface,
             const ei_color_t* color,
             const ei_rect_t* clipper){
//     Lock the surface
    hw_surface_lock(surface);
//     Fill the surface with the color using draw_polygon to fill the whole surface
//     Create a point array with the 4 corners of the surface
    ei_point_t point_array[4] = {{0, 0},
                                 {0, hw_surface_get_size(surface).height},
                                 {hw_surface_get_size(surface).width, hw_surface_get_size(surface).height},
                                 {hw_surface_get_size(surface).width, 0}};
//     Call draw_polygon
    ei_draw_polygon(surface, point_array, 4, *color, clipper);
//     Unlock the surface
    hw_surface_unlock(surface);
}