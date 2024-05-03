#include "ei_frame.h"
#include "ei_widget_attributes.h"

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
    //TODO
    return NULL;
}

void frame_releasefunc(ei_widget_t widget){
    //TODO
}

void frame_drawfunc(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper){
    //TODO
}

void frame_setdefaultsfunc(ei_widget_t widget){
    //TODO
}

void frame_geomnotifyfunc(ei_widget_t widget){
    //TODO
}
