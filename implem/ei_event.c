#include "ei_event.h"
#include "ei_callback.h"

ei_linked_bound_eventtype* bound_eventtype_list = NULL;

bool equal_bound_eventtype(ei_bound_eventtype a,
                           ei_bound_eventtype b){
    return(a.eventtype == b.eventtype &&
           a.widget == b.widget &&
           a.tag == b.tag &&
           a.callback == b.callback &&
           a.user_param == b.user_param);
}

void add_bound_eventtype(ei_linked_bound_eventtype** l,
                         ei_bound_eventtype bound_eventtype){
    ei_linked_bound_eventtype* new_cell = malloc(sizeof(ei_linked_bound_eventtype));
    new_cell->next = *l;
    new_cell->bound_eventtype = bound_eventtype;
    *l = new_cell;
}

void remove_bound_eventtype(ei_linked_bound_eventtype** l,
                            ei_bound_eventtype bound_eventtype){
    if (*l == NULL){
        return;
    }
    ei_linked_bound_eventtype* previous_cell = NULL;
    ei_linked_bound_eventtype* current_cell = *l;
    while(current_cell != NULL && !equal_bound_eventtype(current_cell->bound_eventtype,bound_eventtype)){
        previous_cell = current_cell;
        current_cell = current_cell->next;
    }
    if (current_cell == NULL){
        return;
    }
    if (previous_cell == NULL){
        *l = current_cell;
        return;
    }
    previous_cell->next = current_cell->next;
    free(current_cell);

}

void ei_bind(ei_eventtype_t eventtype,
             ei_widget_t widget,
             ei_tag_t tag,
             ei_callback_t callback,
             void* user_param){
    ei_bound_eventtype bound_eventtype = {eventtype,
                                          widget,
                                          tag,
                                          callback,
                                          user_param};
    add_bound_eventtype(&bound_eventtype_list,bound_eventtype);
}

void ei_unbind(ei_eventtype_t eventtype,
               ei_widget_t widget,
               ei_tag_t	tag,
               ei_callback_t callback,
               void* user_param){
    ei_bound_eventtype bound_eventtype = {eventtype,
                                          widget,
                                          tag,
                                          callback,
                                          user_param};
    remove_bound_eventtype(&bound_eventtype_list,bound_eventtype);
}
