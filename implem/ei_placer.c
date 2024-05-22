#include "ei_placer.h"
#include "ei_implementation.h"
#include "ei_geometrymanager.h"
#include "ei_class.h"
#include "ei_application.h"
#include "ei_widget_attributes.h"

char* get_size_of_requested_char_size(ei_impl_entry_t* entry) {
    char* res = malloc((entry->requested_char_size + 1) * sizeof(char));
    for(int i = 0; i < entry->requested_char_size; i++) {
        res[i] = 'm';
    }
    res[entry->requested_char_size] = '\0'; // terminate the string
    return res;
}

void ei_placer_runfunc(ei_widget_t widget) {
    ei_impl_placer_param_t* param = (ei_impl_placer_param_t*)ei_widget_get_geom_params(widget);
    ei_widget_t parent = widget->parent;

//Set the screen location of the widget according to the placer parameters
    int abs_x = param->x;
    int abs_y = param->y;
    int abs_width = param->width;
    int abs_height = param->height;

// First, check if the widget is a close button of a toplevel
    if(strcmp(widget->wclass->name,"button") == 0) {
        ei_impl_button_t* button = (ei_impl_button_t*)widget;
        if(button->is_toplevel_close_button) {
            ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*)parent;
            if(toplevel->closable && parent->screen_location != NULL) {
// The close button is always at the top right corner of the toplevel
                abs_x = parent->screen_location->top_left.x + PADDING;
                abs_y = parent->screen_location->top_left.y + PADDING;
                abs_width = CLOSE_BUTTON_SIZE;
                abs_height = CLOSE_BUTTON_SIZE;

                ei_rect_t screen_location = (ei_rect_t){{abs_x,abs_y},{abs_width,abs_height}};

                ei_geometry_run_finalize(widget, &screen_location);

                return;
            }

        }
    }

    if(parent != NULL && parent->content_rect != NULL) {
        abs_x += parent->content_rect->top_left.x + parent->content_rect->size.width * param->rel_x;
        abs_y += parent->content_rect->top_left.y + parent->content_rect->size.height * param->rel_y;
// Absolute values have priority over relative values
        abs_width = abs_width ? abs_width : (int)(parent->content_rect->size.width * param->rel_width);
        abs_height = abs_height ? abs_height : (int)(parent->content_rect->size.height * param->rel_height);
    }

//If some values are still not set, use the requested size of the widget
    abs_width = abs_width ? abs_width : widget->requested_size.width;
    abs_height = abs_height ? abs_height : widget->requested_size.height;

//Some widgets have specific behavior
    if(widget->wclass == &ei_toplevelclass) {
        ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*)widget;
        abs_width = fmax(abs_width,toplevel->min_size->width);
        abs_height = fmax(abs_height,toplevel->min_size->height);

        abs_height += HEADER_HEIGHT + toplevel->border_width;
        abs_width += 2 * toplevel->border_width;

    }
    if(widget->wclass == &ei_entryclass) {
        ei_impl_entry_t *entry = (ei_impl_entry_t*)widget;
        if(entry->text_font != NULL) {
            int text_width = 0, text_height = 0;
            char* text = get_size_of_requested_char_size(entry);
            hw_text_compute_size(text, entry->text_font, &text_width, &text_height);
            free(text);
            abs_width = fmax(abs_width, text_width + 2 * entry->border_width);
            abs_height = fmax(abs_height, text_height + 2 * entry->border_width);
        }

    }
    if(widget->wclass == &ei_buttonclass) {
        ei_impl_button_t* button = (ei_impl_button_t*)widget;
        if(button->text != NULL && button->text_font != NULL) {
            int text_width = 0, text_height = 0;
            hw_text_compute_size(button->text, button->text_font, &text_width, &text_height);
            abs_width = fmax(abs_width, text_width);
            abs_height = fmax(abs_height, text_height);
        }
    }
    if(widget->wclass == &ei_frameclass) {
        ei_impl_frame_t* frame = (ei_impl_frame_t*)widget;
        if(frame->text != NULL && frame->text_font != NULL) {
            int text_width = 0, text_height = 0;
            hw_text_compute_size(frame->text, frame->text_font, &text_width, &text_height);
            abs_width = fmax(abs_width, text_width);
            abs_height = fmax(abs_height, text_height);
        }
    }
//Change absolute coordinate according to the anchor
    switch(param->anchor) {
        case ei_anc_north:
            abs_x -= abs_width / 2;
            break;
        case ei_anc_northeast:
            abs_x -= abs_width;
            break;
        case ei_anc_east:
            abs_x -= abs_width;
            abs_y -= abs_height / 2;
            break;
        case ei_anc_southeast:
            abs_x -= abs_width;
            abs_y -= abs_height;
            break;
        case ei_anc_south:
            abs_x -= abs_width / 2;
            abs_y -= abs_height;
            break;
        case ei_anc_southwest:
            abs_y -= abs_height;
            break;
        case ei_anc_west:
            abs_y -= abs_height / 2;
            break;
        case ei_anc_center:
            abs_x -= abs_width / 2;
            abs_y -= abs_height / 2;
            break;
        default: // ei_anc_northwest or ei_anc_none
            break;
    }

//At this point, the widget's screen location is set if there are no conflicts with other widgets
//Now we need to check for conflicts with his parent content_rect
    if (widget->parent != NULL && parent->content_rect != NULL && strcmp(widget->wclass->name,"toplevel") != 0){
        if (abs_x + abs_width > parent->content_rect->top_left.x + parent->content_rect->size.width) {
            abs_width = parent->content_rect->top_left.x + parent->content_rect->size.width - abs_x;
        }
        if (abs_y + abs_height > parent->content_rect->top_left.y + parent->content_rect->size.height) {
            abs_height = parent->content_rect->top_left.y + parent->content_rect->size.height - abs_y;
        }
    }

//Define the screen location of the widget
    ei_rect_t screen_location = (ei_rect_t){{abs_x,abs_y},{abs_width,abs_height}};

    ei_geometry_run_finalize(widget, &screen_location);


}

void ei_placer_releasefunc(ei_widget_t widget) {
//Free all specifics parameters of placer geometry manager
    free((ei_impl_placer_param_t*)ei_widget_get_geom_params(widget));

//Maybe call geometry_manager of his brothers in case some drawing space is now available for them


}

void ei_place(ei_widget_t widget,
              ei_anchor_t* anchor,
              int* x,
              int* y,
              int* width,
              int* height,
              float* rel_x,
              float* rel_y,
              float* rel_width,
              float* rel_height){

    ei_impl_placer_param_t* param =  (ei_impl_placer_param_t*)ei_widget_get_geom_params(widget);

    if(param == NULL) { //Then the widget is not being managed
// Default values
        param = (ei_impl_placer_param_t*)malloc(sizeof(ei_impl_placer_param_t));
        param->anchor = anchor ? *anchor : ei_anc_northwest;
        param->x = x ? *x : 0;
        param->y = y ? *y : 0;
        param->width = width ? *width : (widget->requested_size.width ? widget->requested_size.width : 0);
        param->height = height ? *height : (widget->requested_size.height ? widget->requested_size.height : 0);
        param->rel_x = rel_x ? *rel_x : 0.0;
        param->rel_y = rel_y ? *rel_y : 0.0;
        param->rel_width = rel_width ? *rel_width : 0.0;
        param->rel_height = rel_height ? *rel_height : 0.0;
    }
    else {
// Changes geom parameter if asked
        param->anchor = anchor ? *anchor : (param->anchor ? param->anchor : ei_anc_northwest);
        param->x = x ? *x : (param->x ? param->x : 0);
        param->y = y ? *y : (param->y ? param->y : 0);
        param->height = height ? *height : (param->height ? param->height :(widget->requested_size.height ? widget->requested_size.height : 0));
        param->width = width ? *width : (param->width ? param->width :(widget->requested_size.width ? widget->requested_size.width : 0));
        param->rel_x = rel_x ? *rel_x : (param->rel_x ? param->rel_x : 0.0);
        param->rel_y = rel_y ? *rel_y : (param->rel_y ? param->rel_y : 0.0);
        param->rel_width = rel_width ? *rel_width : (param->rel_width ? param->rel_width : 0.0);
        param->rel_height = rel_height ? *rel_height : (param->rel_height ? param->rel_height : 0.0);
    }
    ei_widget_set_geom_params(widget, (ei_geom_param_t)param);

    ei_geometrymanager_name_t name = "placer";
    widget->geom_params->manager = ei_geometrymanager_from_name(name);
    ei_placer_runfunc(widget);
    ei_app_invalidate_rect(ei_widget_get_screen_location(widget));
}

ei_geometrymanager_t placer_geometrymanager = { // Definition of the placer geometrymanager
    "placer",
    ei_placer_runfunc,
    ei_placer_releasefunc,
    NULL};