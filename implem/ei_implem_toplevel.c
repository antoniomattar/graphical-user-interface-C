//
// Created by matteo on 5/13/24.
//

#include "ei_implem_toplevel.h"


#include "ei_implementation.h"
#include "draw_toplevel.h"
#include "ei_pick_id.h"
#include "ei_draw.h"
//TODO create an actual button to close the toplevel

ei_widget_t toplevel_allocfunc(){
    return calloc(1,sizeof(ei_impl_toplevel_t));
}

void toplevel_releasefunc(ei_widget_t widget){
    //  Free of widget parameters
    ei_free_widget_attributes(widget);
    //  Free of toplevel parameters
    ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*) widget;
    if(toplevel->min_size) {
        free(toplevel->min_size);
    }

    //Free the widget
    free(toplevel);
}

void toplevel_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper){
    //TODO Call to the draw function
    ei_rect_t* new_clipper = clipper;
    if(clipper == NULL) {
        if(widget->parent != NULL && widget->parent->content_rect != NULL){
            new_clipper = widget->parent->content_rect;
        }else {
            new_clipper = root_widget->screen_location;
        }
    }else {
        if(widget->parent != NULL && widget->parent->content_rect != NULL) {
            *new_clipper = intersection_rect_with_rect(clipper, widget->parent->content_rect);
        }else {
            *new_clipper = intersection_rect_with_rect(clipper,root_widget->content_rect);
        }
    }
    double avant_toplevel = hw_now();
    draw_toplevel(widget,surface,new_clipper);
    double apres_toplevel = hw_now();
    printf("Temps pour dessiner toplevel de la pick id %u : %f\n", widget->pick_id,apres_toplevel-avant_toplevel);

    double avant_pick = hw_now();
    ei_update_pick_surface(surface,widget);
    double apres_pick = hw_now();
    printf("Temps pour mettre à jour la pick surface de la pick id %u : %f\n", widget->pick_id,apres_pick-avant_pick);
    for (ei_widget_t current_widget = widget->children_head; current_widget != NULL; current_widget = current_widget->next_sibling){
        current_widget->wclass->drawfunc(current_widget,surface,pick_surface,new_clipper);
    }
//  TODO : Editing of pick_surface
//    ei_draw_polygon(pick_surface,point_array,4,*frame->widget->pick_color,clipper);

}

void toplevel_setdefaultsfunc(ei_widget_t widget){
//    Fix the widget attributes by default
//  TODO : Put other default settings
    widget->wclass = ei_widgetclass_from_name("toplevel");
//    widget->pick_id = 0;
//    widget->pick_color = (ei_color_t*){0x00,0x00,0x00,0x00};

//    widget->requested_size = (ei_size_t){0,0};
//    widget->screen_location = (ei_rect_t){{0,0},{0,0}};
// widget->content_rect = NULL;
//    widget->geom_params = NULL;



//    Fix the toplevel attributes by default
    ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*) widget;
}

void toplevel_geomnotifyfunc(ei_widget_t widget){
//  TODO : Understand what to do

}

ei_widgetclass_t ei_toplevelclass = {
        "toplevel",
        toplevel_allocfunc,
        toplevel_releasefunc,
        toplevel_drawfunc,
        toplevel_setdefaultsfunc,
        toplevel_geomnotifyfunc,
        NULL
};