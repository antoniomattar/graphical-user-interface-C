#include "ei_class.h"


void ei_widgetclass_register(ei_widgetclass_t* widgetclass){
    if(registered_widget_classes == NULL){
        registered_widget_classes = widgetclass;
    }
    else{
        ei_widgetclass_t* current = registered_widget_classes;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = widgetclass;
    }
}

ei_widgetclass_t* ei_widgetclass_from_name(ei_const_string_t name){
    ei_widgetclass_t* current = registered_widget_classes;
    while(current != NULL){
        if(strcmp(current->name,name) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

size_t ei_widget_struct_size(){
    return sizeof(ei_widget_t);
}