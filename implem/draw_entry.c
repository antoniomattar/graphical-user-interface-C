//
// Created by matteo on 5/13/24.
//

#include "draw_entry.h"
#include "ei_implementation.h"
#include "ei_implem_entry.h"
#include "ei_draw.h"

#define ENTRY_BORDER_COLOR (ei_color_t){0x50, 0x50, 0x50, 0xff}

void draw_entry(ei_widget_t widget, ei_surface_t surface, ei_rect_t* clipper){
//     Get the entry attributes
    ei_impl_entry_t* entry = (ei_impl_entry_t*) widget;
    ei_rect_t* rect = widget->screen_location;
    int border_thickness = entry->border_width;

//     Draw the border
    ei_rect_t border_rect = *rect;
    ei_fill(surface, &ENTRY_BORDER_COLOR, &border_rect);

//     Draw the background
    ei_point_t background_top_left = {rect->top_left.x + border_thickness, rect->top_left.y + border_thickness};
    ei_size_t background_size = {rect->size.width - 2 * border_thickness, rect->size.height - 2 * border_thickness};
    ei_rect_t background_rect = {background_top_left, background_size};
    ei_fill(surface, &entry->color, &background_rect);

//     Draw the text
    ei_point_t text_point = {rect->top_left.x + border_thickness, rect->top_left.y + border_thickness};
    ei_draw_text(surface, &text_point, entry->text, entry->text_font, entry->text_color, &background_rect);
}