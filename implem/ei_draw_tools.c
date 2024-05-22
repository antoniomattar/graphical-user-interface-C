#include "ei_draw_tools.h"

struct ei_linked_point_t* arc(ei_point_t center, int radius, float start_angle, float end_angle) {
//     Trivial case
    if (radius == 0) {
//         Return the center
        struct ei_linked_point_t* center_point = malloc(sizeof(struct ei_linked_point_t));
        center_point->point = center;
        center_point->next = NULL;
        return center_point;
    }
//     Find number of points to draw
    double number_of_points = ceil( (1.5*(end_angle - start_angle) * radius) + 1);
//     Find angle increment
    double angle_increment = ((end_angle - start_angle) / number_of_points) ;
//     Create the linked list of points
    struct ei_linked_point_t* linked_points = malloc(sizeof(struct ei_linked_point_t));
    double current_angle = start_angle;
    struct ei_linked_point_t* current_pointer = linked_points;
    for (int i = 1; i <= number_of_points; i++) {
//         Calculate the current point
        ei_point_t current_point = (ei_point_t) {
                center.x + (int) round(radius * cos(current_angle)),
                center.y + (int) round(radius * sin(current_angle))
        };

//         Add the current point to the linked list
        current_pointer->point = current_point;
        if (i == number_of_points) {
            current_pointer->next = NULL;
        }
        else {
            current_pointer->next = malloc(sizeof(struct ei_linked_point_t));
            current_pointer = current_pointer->next;
        }


//         Increment the angle
        current_angle += angle_increment;
    }
//     Return the linked list of points
    return linked_points;

}

struct ei_linked_point_t * rounded_frame(ei_rect_t rect, int radius, bool top_rounded, bool bottom_rounded) {
    // Create the linked list of points
    struct ei_linked_point_t* linked_points = malloc(sizeof(struct ei_linked_point_t));
    struct ei_linked_point_t* current_point = linked_points;
    // Top

    // If the top corners are rounded, add the top left corner with the top left arc and move current_point to the end of the linked list then add the top right corner with the top right arc
    if (top_rounded == true) {
        struct ei_linked_point_t* top_left_arc = arc((ei_point_t) {rect.top_left.x + radius, rect.top_left.y + radius}, radius, M_PI,  1.5*M_PI);
        current_point->point = top_left_arc->point;
        current_point->next = top_left_arc->next;
        // Move current_point to the end of the linked list
        while (current_point->next != NULL) {
            current_point = current_point->next;
        }
        struct ei_linked_point_t* top_right_arc = arc((ei_point_t) {rect.top_left.x + rect.size.width - radius, rect.top_left.y + radius}, radius, 1.5 * M_PI, 2 * M_PI);
        current_point->point = top_right_arc->point;
        current_point->next = top_right_arc->next;
        // Move current_point to the end of the linked list
        while (current_point->next != NULL) {
            current_point = current_point->next;
        }
        free(top_left_arc);
        free(top_right_arc);
    }
    else {
        // If the top corners are not rounded, add the top left corner and the top right corner
        current_point->point = rect.top_left;
        current_point->next = malloc(sizeof(struct ei_linked_point_t));
        current_point = current_point->next;

        current_point->point = (ei_point_t) {rect.top_left.x + rect.size.width, rect.top_left.y};
        current_point->next = malloc(sizeof(struct ei_linked_point_t));
        current_point = current_point->next;
    }

    // Bottom
    // If the bottom corners are rounded, add the bottom right corner with the bottom right arc and move current_point to the end of the linked list then add the bottom left corner with the bottom left arc
    if (bottom_rounded == true) {
        struct ei_linked_point_t* bottom_right_arc = arc((ei_point_t) {rect.top_left.x + rect.size.width - radius, rect.top_left.y + rect.size.height - radius}, radius, 0, 0.5 * M_PI);
        current_point->point = bottom_right_arc->point;
        current_point->next = bottom_right_arc->next;
        // Move current_point to the end of the linked list
        while (current_point->next != NULL) {
            current_point = current_point->next;
        }
        struct ei_linked_point_t* bottom_left_arc = arc((ei_point_t) {rect.top_left.x + radius, rect.top_left.y + rect.size.height - radius}, radius, 0.5 * M_PI, M_PI);
        current_point->point = bottom_left_arc->point;
        current_point->next = bottom_left_arc->next;
        // Move current_point to the end of the linked list
        while (current_point->next != NULL) {
            current_point = current_point->next;
        }
        free(bottom_left_arc);
        free(bottom_right_arc);
    }
    else {
        // If the bottom corners are not rounded, add the bottom right corner and the bottom left corner
        current_point->point = (ei_point_t) {rect.top_left.x + rect.size.width, rect.top_left.y + rect.size.height};
        current_point->next = malloc(sizeof(struct ei_linked_point_t));
        current_point = current_point->next;

        current_point->point = (ei_point_t) {rect.top_left.x, rect.top_left.y + rect.size.height};
        current_point->next = malloc(sizeof(struct ei_linked_point_t));
        current_point = current_point->next;
    }

    // Close the frame by adding the first point of the linked list
    current_point->point = linked_points->point;
    current_point->next = NULL;
    return linked_points;
}


ei_point_t translate_point(ei_point_t point, int x, int y) {
    return (ei_point_t) {point.x + x, point.y + y};
}

ei_point_t symmetrical_point(ei_point_t point, ei_point_t center) {
    return (ei_point_t) {2 * center.x - point.x, 2 * center.y - point.y};
}

struct ei_linked_point_t* symmetrical_linked_points(struct ei_linked_point_t* linked_points, ei_point_t center) {
    struct ei_linked_point_t* symmetrical_linked_points = malloc(sizeof(struct ei_linked_point_t));
    struct ei_linked_point_t* current_point = symmetrical_linked_points;
    while (linked_points != NULL) {
        current_point->point = symmetrical_point(linked_points->point, center);
        if (linked_points->next != NULL) {
            current_point->next = malloc(sizeof(struct ei_linked_point_t));
            current_point = current_point->next;
        }
        linked_points = linked_points->next;
    }
    current_point->next = NULL;
    return symmetrical_linked_points;
}

ei_color_t adjust_color_intensity(ei_color_t color, double factor) {
    return (ei_color_t) {(uint8_t) color.red * factor,
                         (uint8_t) color.green * factor,
                         (uint8_t) color.blue * factor,
                         color.alpha};
}

void draw_image(ei_surface_t surface, ei_rect_t* rect, ei_surface_t img, ei_rect_ptr_t img_rect, int border_thickness, ei_rect_t* clipper) {
    // TODO get this working
    // Define the destination rectangle for the image
    ei_point_t img_dest_top_left = {rect->top_left.x + border_thickness, rect->top_left.y + border_thickness};
    ei_size_t img_dest_size = {rect->size.width - 2 * border_thickness, rect->size.height - 2 * border_thickness};
    ei_rect_t img_dest_rect = {img_dest_top_left, img_dest_size};

    // Define the source rectangle for the image
    ei_rect_t img_src_rect;
    if (img_rect != NULL) {
        img_src_rect = *(img_rect);
    } else {
        ei_size_t img_size = hw_surface_get_size(img);
        img_src_rect = (ei_rect_t){{0, 0}, img_size};
    }

//    Clip the image if necessary
    if (clipper) {
        img_dest_rect = intersection_rect_with_rect(&img_dest_rect, clipper);
    }

//     Copy the image onto the surface of the frame
    ei_copy_surface(surface, &img_dest_rect, img, &img_src_rect, true);
}

ei_point_t calculate_text_position( const ei_rect_t* screen_location, const char* text, ei_font_t font, ei_anchor_t text_anchor) {
    int text_width, text_height;
    hw_text_compute_size(text, font, &text_width, &text_height);
    ei_point_t text_point = screen_location->top_left;
    if (text_anchor == ei_anc_center || text_anchor == ei_anc_none) {
        text_point.x += (screen_location->size.width - text_width) / 2;
        text_point.y += (screen_location->size.height - text_height) / 2;
    }
    else if (text_anchor == ei_anc_north) {
        text_point.x += (screen_location->size.width - text_width) / 2;
    }
    else if (text_anchor == ei_anc_northeast) {
        text_point.x += screen_location->size.width - text_width;
    }
    else if (text_anchor == ei_anc_east) {
        text_point.x += screen_location->size.width - text_width;
        text_point.y += (screen_location->size.height - text_height) / 2;
    }
    else if (text_anchor == ei_anc_southeast) {
        text_point.x += screen_location->size.width - text_width;
        text_point.y += screen_location->size.height - text_height;
    }
    else if (text_anchor == ei_anc_south) {
        text_point.x += (screen_location->size.width - text_width) / 2;
        text_point.y += screen_location->size.height - text_height;
    }
    else if (text_anchor == ei_anc_southwest) {
        text_point.y += screen_location->size.height - text_height;
    }
    else if (text_anchor == ei_anc_west) {
        text_point.y += (screen_location->size.height - text_height) / 2;
    }
    else if (text_anchor == ei_anc_northwest) {
//         Nothing to do
    }
    return text_point;
}

bool is_rect_in_surface(const ei_rect_t* rect, ei_surface_t surface){
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
//     Check if the rectangle is in the surface
    if (rect_top_left.x < surface_top_left.x || rect_top_left.y < surface_top_left.y ||
        bottom_right.x > surface_bottom_right.x || bottom_right.y > surface_bottom_right.y){
        return false;
    }
    return true;
}

bool is_rect_in_rect(const ei_rect_t* rect1, const ei_rect_t* rect2){
//     Get the top left corner of the rectangle 1
    ei_point_t rect1_top_left = rect1->top_left;
//     Get the bottom right corner of the rectangle 1
    ei_point_t rect1_bottom_right = {rect1_top_left.x + rect1->size.width, rect1_top_left.y + rect1->size.height};
//     Get the top left corner of the rectangle 2
    ei_point_t rect2_top_left = rect2->top_left;
//     Get the bottom right corner of the rectangle 2
    ei_point_t rect2_bottom_right = {rect2_top_left.x + rect2->size.width, rect2_top_left.y + rect2->size.height};
//     Check if the rectangle 1 is in the rectangle 2
    if (rect1_top_left.x < rect2_top_left.x || rect1_top_left.y < rect2_top_left.y ||
        rect1_bottom_right.x > rect2_bottom_right.x || rect1_bottom_right.y > rect2_bottom_right.y){
        return false;
    }
    return true;
}
