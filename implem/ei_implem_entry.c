#include "ei_implem_entry.h"
#include "ei_widget_attributes.h"
#include "ei_draw.h"
#include "draw_entry.h"

ei_widgetclass_t ei_entryclass = {
        "entry",
        entry_allocfunc,
        entry_releasefunc,
        entry_drawfunc,
        entry_setdefaultsfunc,
        entry_geomnotifyfunc,
        NULL
};

ei_widget_t entry_allocfunc(){
    return calloc(1,sizeof(ei_impl_entry_t));
}

void entry_releasefunc(ei_widget_t widget){
//      Free of widget parameters
    ei_free_widget_attributes(widget);

//      Free of entry parameters
    ei_impl_entry_t* entry = (ei_impl_entry_t*) widget;
    free(entry);

}

void entry_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper){

    draw_entry(widget,surface,clipper);

//      Call of drawfunc on every child
    ei_rect_t new_clipper = intersection_rect_with_rect(clipper,widget->parent->content_rect);
    for (ei_widget_t current_widget = widget->children_head; current_widget != NULL; current_widget = current_widget->next_sibling){

        current_widget->wclass->drawfunc(current_widget,surface,pick_surface,&new_clipper);
    }
    //  TODO : Editing of pick_surface
    //    ei_draw_polygon(pick_surface,point_array,4,*frame->widget->pick_color,clipper);

}

void entry_setdefaultsfunc(ei_widget_t widget){
//        Fix the widget attributes by default
//      TODO : Put other default settings
    widget->wclass = ei_widgetclass_from_name("entry");
//        widget->pick_id = 0;
//        widget->pick_color = (ei_color_t*){0x00,0x00,0x00,0x00};
//
//        widget->requested_size = (ei_size_t){0,0};
//        widget->screen_location = (ei_rect_t){{0,0},{0,0}};
//        widget->content_rect = NULL;
//        widget->geom_params = NULL;



    //    Fix the entry attributes by default

}

void entry_geomnotifyfunc(ei_widget_t widget){
    //  TODO : Understand what to do

}