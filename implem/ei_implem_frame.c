#include "ei_implem_frame.h"

#include "draw_frame.h"
#include "ei_widget_attributes.h"
#include "ei_pick_id.h"

ei_widgetclass_t ei_frameclass = {
        "frame",
        frame_allocfunc,
        frame_releasefunc,
        frame_drawfunc,
        frame_setdefaultsfunc,
        frame_geomnotifyfunc,
        NULL
};

ei_widget_t frame_allocfunc(){
    return calloc(1,sizeof(ei_impl_frame_t));
}

void frame_releasefunc(ei_widget_t widget){
//  Free of widget parameters
    ei_free_widget_attributes(widget);
//  Free of frame parameters
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;
    if(frame->img_rect != NULL) {
        free(frame->img_rect);
    }
    if(frame->img != NULL) {
        hw_surface_free(frame->img);
    }
    free(frame);
}

void frame_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper){
    double avant_draw_frame = hw_now();
    draw_frame(widget,surface,clipper);
    double apres_draw_frame = hw_now();
    printf("Temps pour dessiner frame : %f\n", apres_draw_frame - avant_draw_frame);
//  Editing of pick_surface
    ei_update_pick_surface(surface,widget);
//  Call of drawfunc on every child
    double avant_child = hw_now();
    for (ei_widget_t current_widget = widget->children_head; current_widget != NULL; current_widget = current_widget->next_sibling){
        current_widget->wclass->drawfunc(current_widget,surface,pick_surface,clipper);
    }
    double apres_child = hw_now();
    printf("Temps pour dessiner tous les enfants de la root : %f\n", apres_child - avant_child);
//
//  TODO : Draw image
}

void frame_setdefaultsfunc(ei_widget_t widget){
//    Fix the widget attributes by default
//  TODO : Put other default settings
    widget->wclass = ei_widgetclass_from_name("frame");
//    widget->pick_id = 0;
//    widget->pick_color = (ei_color_t*){0x00,0x00,0x00,0x00};

//    widget->requested_size = (ei_size_t){0,0};
//    widget->screen_location = (ei_rect_t){{0,0},{0,0}};
//    widget->content_rect = NULL;
//    widget->geom_params = NULL;



//    Fix the frame attributes by default
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;
    frame->color = (ei_color_t){0xff,0x00,0x00,0xff};
    frame->text = NULL;
    frame->img = NULL;
    frame->img_rect = NULL;
    frame->text_font = ei_default_font;
    frame->text_color = (ei_color_t){0xff,0xff,0xff,0xff};
    frame->text_anchor = ei_anc_center;
    frame->relief = ei_relief_none;
    frame->border_width = 0;
    frame->size = (ei_size_t){0,0};
    frame->img_anchor = ei_anc_center;

}

void frame_geomnotifyfunc(ei_widget_t widget){
//  TODO : Understand what to do

}
