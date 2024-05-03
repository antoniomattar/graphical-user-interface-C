#include "ei_button.h"

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
    //TODO
    return NULL;
}

void button_releasefunc(ei_widget_t	widget) {
    //TODO
}

void button_drawfunc(ei_widget_t widget,
                     ei_surface_t surface,
                     ei_surface_t	pick_surface,
                     ei_rect_t* clipper) {
    //TODO
}

void button_setdefaultsfunc() {
    //TODO
}

void button_geomnotifyfunc() {
    //TODO
}