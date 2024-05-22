#include "ei_implem_button.h"
#include "ei_types.h"
#include "ei_widget_attributes.h"
#include "draw_button.h"
#include "ei_pick_id.h"

ei_widgetclass_t ei_buttonclass = {
        "button",
        button_allocfunc,
        button_releasefunc,
        button_drawfunc,
        button_setdefaultsfunc,
        button_geomnotifyfunc,
        NULL
};


ei_widget_t button_allocfunc() {
    return calloc(1,sizeof(ei_impl_button_t));
}

void button_releasefunc(ei_widget_t	widget) {
    //  Free of widget parameters
    ei_free_widget_attributes(widget);
//  Free of button parameters
    ei_impl_button_t* button = (ei_impl_button_t*) widget;
//    TODO: check why can't free text
//    if (button->text != NULL) {
//        free(button->text);
//    }
    if(button->img_rect != NULL) {
        free(button->img_rect);
    }
    if(button->img != NULL) {
        hw_surface_free(button->img);
    }
    free(button);
}



void button_drawfunc(ei_widget_t widget,
                     ei_surface_t surface,
                     ei_surface_t pick_surface,
                     ei_rect_t* clipper) {
    if(widget->screen_location == NULL) {
        return; // Defensive programming
    }

//    Draw the button and its text
    ei_rect_t* intersection_clipper = widget->screen_location;
    if(clipper != NULL) {
        ei_rect_t temp = intersection_rect_with_rect(intersection_clipper, clipper);
        intersection_clipper = &temp;
    }else {
        intersection_clipper = clipper;
    }
    draw_button(widget,surface,intersection_clipper);
    //  Editing of pick_surface
    ei_update_pick_surface(surface,widget);
    for (ei_widget_t current_widget = widget->children_head; current_widget != NULL; current_widget = current_widget->next_sibling){
        current_widget->wclass->drawfunc(current_widget,surface,pick_surface,clipper);
    }

}

void button_setdefaultsfunc(ei_widget_t widget) {
//     Fix the widget attributes by default
    ei_impl_button_t *button = (ei_impl_button_t*) widget;
    button->is_toplevel_close_button = false;
    button ->corner_radius = 10;
    button->text_font = ei_default_font;
    button->img = NULL;
    button->img_rect = NULL;
    button->text = "";
}

void button_geomnotifyfunc() {
    //TODO
}