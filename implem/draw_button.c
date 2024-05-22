#include "draw_button.h"
#include "ei_widget_attributes.h"
#include "ei_draw_tools.h"

int get_half_smaller_dimension(ei_size_t size) {
    int smaller_dimension = size.height < size.width ? size.height : size.width;
    return smaller_dimension / 2;
}



struct ei_linked_point_t* init_linked_point(int x, int y) {
    struct ei_linked_point_t* point = malloc(sizeof(struct ei_linked_point_t));
    point->point = (ei_point_t){x, y};
    point->next = NULL;
    return point;
}



void append_arc(struct ei_linked_point_t** current_point, struct ei_linked_point_t* arc) {
    while (*current_point && (*current_point)->next) {
        current_point = &((*current_point)->next);
    }
    (*current_point)->next = arc;
    while ((*current_point)->next) {
        current_point = &((*current_point)->next);
    }
}

void draw_button(ei_widget_t widget, ei_surface_t surface, ei_rect_t* clipper) {
//     Get the rectangle of the button
    const ei_rect_t* screen_location = ei_widget_get_screen_location(widget);
//     Get the button attributes
    ei_impl_button_t* button = (ei_impl_button_t*) widget;
//     Get the button color
    ei_color_t button_color = button->color;
//     Get the corner radius
    int corner_radius = button->corner_radius;
//     Get the button relief
    ei_relief_t relief = button->relief;
//     Get the button border width
    int border_width = button->border_width;


//     Set the difference of color for the relief
//     Adjust color based on relief
    double difference = 0.3;
    ei_color_t upper_color = adjust_color_intensity(button->color, button->relief == ei_relief_raised ? 1 + difference : 1 - difference);
    ei_color_t lower_color = adjust_color_intensity(button->color, button->relief == ei_relief_sunken ? 1 + difference : 1 - difference);

//     Value h : minimum between the height of the button and the width of the button
    int h = get_half_smaller_dimension(screen_location->size);

    ei_point_t center_point_right= translate_point(screen_location->top_left, screen_location->size.width - h, h);
    ei_point_t center_point_left = translate_point(screen_location->top_left, h,  h);
    ei_point_t center_point = (ei_point_t) {
            screen_location->top_left.x + screen_location->size.width / 2,
            screen_location->top_left.y + screen_location->size.height / 2
    };


//     Create the linked list of points that represent the upper frame
//     This list is in order the top left arc, half of the top right arc, middle right point, middle left point, half of the bottom left arc
    struct ei_linked_point_t* upper_linked_points = malloc(sizeof(struct ei_linked_point_t));
    struct ei_linked_point_t* current_point = upper_linked_points;
    struct ei_linked_point_t* top_left_arc = arc((ei_point_t) {screen_location->top_left.x + button->corner_radius, screen_location->top_left.y + button->corner_radius}, button->corner_radius, M_PI, 1.5 * M_PI);
    current_point->point = top_left_arc->point;
    current_point->next = top_left_arc->next;
//     Move current_point to the end of the linked list
    while (current_point->next != NULL) {
        current_point = current_point->next;
    }

    struct ei_linked_point_t* half_top_right_arc = arc(translate_point(screen_location->top_left, screen_location->size.width - button->corner_radius, button->corner_radius), button->corner_radius, 1.5 * M_PI, 1.75 * M_PI);
    current_point->point = half_top_right_arc->point;
    current_point->next = half_top_right_arc->next;
//     Move current_point to the end of the linked list
    while (current_point->next != NULL) {
        current_point = current_point->next;
    }
    current_point->point = center_point_right;
    current_point->next = malloc(sizeof(struct ei_linked_point_t));
    current_point = current_point->next;

    current_point->point = center_point_left;
    current_point->next = malloc(sizeof(struct ei_linked_point_t));
    current_point = current_point->next;

    struct ei_linked_point_t* half_bottom_left_arc = arc(translate_point(screen_location->top_left, button->corner_radius,screen_location->size.height - button->corner_radius), button->corner_radius, 0.75 * M_PI, M_PI);
    current_point->point = half_bottom_left_arc->point;
    current_point->next = half_bottom_left_arc->next;
//     Move current_point to the end of the linked list
    while (current_point->next != NULL) {
        current_point = current_point->next;
    }
//     Close the frame by adding the first point of the linked list
    current_point->point = top_left_arc->point;
    current_point->next = NULL;
//     Free the memory allocated for the arcs
    free(top_left_arc);
    free(half_top_right_arc);
    free(half_bottom_left_arc);
//     Draw the upper frame
    uint32_t n_upper = size_of_linked_point(upper_linked_points);
    ei_point_t* upper_point_array = linked_to_array(upper_linked_points, n_upper);
    ei_draw_polygon(surface, upper_point_array, n_upper, upper_color, clipper);

//     For the lower frame, we use the symmetrical points of the upper frame
    struct ei_linked_point_t* lower_linked_points = symmetrical_linked_points(upper_linked_points, center_point);
//     Draw the lower frame
    uint32_t n_lower = size_of_linked_point(lower_linked_points);
    ei_point_t* lower_point_array = linked_to_array(lower_linked_points, n_lower);
    ei_draw_polygon(surface, lower_point_array, n_lower, lower_color, clipper);

//         Free the memory allocated for the linked list of points
    free_linked_points(upper_linked_points);
    free_linked_points(lower_linked_points);
//         Free the memory allocated for the array of points
    free(upper_point_array);
    free(lower_point_array);

//    Create the linked list of points that represent the rounded frame
//    First, create a small rectangle to draw the button with the border width
    ei_rect_t border_rect = *screen_location;

    if (border_width) {
        border_rect.top_left.x += border_width;
        border_rect.top_left.y += border_width;
        border_rect.size.width -= 2 * border_width;
        border_rect.size.height -= 2 * border_width;
    }
    struct ei_linked_point_t* border_linked_points = rounded_frame(border_rect, corner_radius, true, true);
//     Get the number of points in the linked list
    uint32_t n = size_of_linked_point(border_linked_points);
//     Convert the linked list of points to an array of points
    ei_point_t* border_point_array = linked_to_array(border_linked_points, n);

//     Draw the button
    ei_draw_polygon(surface, border_point_array, n, button_color, clipper);

//     Draw the text if there is one

    if (button->text != NULL != 0) {
        ei_point_t text_point = calculate_text_position(screen_location, button->text, button->text_font, button->text_anchor);
        if (button->relief == ei_relief_sunken) {
            text_point = translate_point(text_point, -2, -2);
        }
        ei_draw_text(surface, &text_point, button->text, button->text_font, button->text_color, clipper);
    }

//    Draw the image if there is one
    if (button->img != NULL) {
        ei_rect_t real_clipper = *widget->content_rect;
        if (clipper) {
            real_clipper = intersection_rect_with_rect(clipper, screen_location);
        }
        draw_image(surface, (ei_rect_t*)screen_location, button->img, button->img_rect, border_width, &real_clipper);
    }
//     Free the memory allocated for the linked list of points
    free_linked_points(border_linked_points);
//     Free the memory allocated for the array of points
    free(border_point_array);

}
