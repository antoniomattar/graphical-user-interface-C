#include "ei_pick_id.h"
#include "ei_widget_attributes.h"

ei_widget_t ei_get_widget_on_cursor(ei_event_t* event){
    ei_size_t root_surface_size = hw_surface_get_size(root_surface);
    ei_point_t mouse_cursor_xy = event->param.mouse.where;
//  Mouse might be out of root surface (alt + tab for example)
    if (mouse_cursor_xy.x >= root_surface_size.width ||
        mouse_cursor_xy.y >= root_surface_size.height){
        return NULL;
    }
    hw_surface_lock(pick_surface);
    uint32_t* pixel_ptr = (uint32_t*) hw_surface_get_buffer(pick_surface);
    hw_surface_unlock(pick_surface);
    for (int i = 0; i < mouse_cursor_xy.x + root_surface_size.width *mouse_cursor_xy.y; i++){
        pixel_ptr++;
    }
//    printf("Widget selectionné de type : %s\n", ei_get_widget_from_id(root_widget,*pixel_ptr)->wclass->name);
//    printf("Son adresse est %p\n", ei_get_widget_from_id(root_widget,*pixel_ptr));
//    printf("Le pick id associé est %u\n", *pixel_ptr);
    return ei_get_widget_from_id(root_widget,*pixel_ptr);
}

ei_widget_t ei_get_widget_from_id(ei_widget_t root, uint32_t pick_id) {
    // TODO : Add comments
    if (pick_id == 0) {
        return root_widget;
    }
    if (root == NULL) {
        return NULL;
    }
    if (root->pick_id == pick_id) {
        return root;
    }
    ei_widget_t current_widget = root->children_head;
    while (current_widget != NULL) {
        ei_widget_t found_widget = ei_get_widget_from_id(current_widget, pick_id);
        if (found_widget != NULL) {
            return found_widget;
        }
        current_widget = current_widget->next_sibling;
    }
    return NULL;
}

void ei_update_pick_surface(ei_surface_t surface,
                            ei_widget_t widget){
//  Initialization
    uint32_t* pixel_ptr;
    uint32_t pick_id = widget->pick_id;
    ei_size_t root_surface_size = hw_surface_get_size(root_surface);
    const ei_rect_t* screen_location = ei_widget_get_screen_location(widget);
//  Lock the surface for editing
    hw_surface_lock(pick_surface);
//  Update the pick id values
    pixel_ptr = (uint32_t*)hw_surface_get_buffer(pick_surface);
    uint32_t* limit = pixel_ptr + root_surface_size.width * root_surface_size.height;
    pixel_ptr += (uint32_t)fmax(screen_location->top_left.x,0) + root_surface_size.width * (uint32_t)fmax(screen_location->top_left.y,0);
    uint32_t current_rel_x = 1;
    for (uint32_t i = 0; i < screen_location->size.width * screen_location->size.height && pixel_ptr < limit; i ++){
        if (current_rel_x == screen_location->size.width){
            pixel_ptr += root_surface_size.width-screen_location->size.width;
            current_rel_x = 0;
        }
        if (pixel_ptr < limit){
            *pixel_ptr = pick_id;
        }
        pixel_ptr++;
        current_rel_x++;
    }
    hw_surface_unlock(pick_surface);

}
