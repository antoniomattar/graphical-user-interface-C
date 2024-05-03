#include "ei_class.h"


bool ei_widgetclass_is_registered(ei_widgetclass_t* widgetclass){
    ei_widgetclass_t* current = registered_widget_classes;
    while(current != NULL){
        if(current == widgetclass){
            return true;
        }
        current = current->next;
    }
    return false;
}

ei_widget_t ei_widget_create(ei_const_string_t class_name,
                             ei_widget_t parent,
                             ei_user_param_t user_data,
                             ei_widget_destructor_t destructor){
    //TODO
}

void ei_widget_destroy(ei_widget_t widget){
    //TODO
}
