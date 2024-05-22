#include "ei_application.h"
#include "ei_class.h"
#include "ei_widget_attributes.h"
#include "ei_event.h"
#include "ei_callback.h"
#include "ei_pick_id.h"
#include "ei_utils.h"

ei_widget_t root_widget = NULL;
ei_surface_t root_surface = NULL;

ei_surface_t pick_surface = NULL;

ei_linked_rect_t* rects = NULL;

ei_widget_t on_cursor_widget;

bool leave_request = false;

void free_rects() {
    // Free the list of rects
        ei_linked_rect_t* current_rect = rects;
        ei_linked_rect_t* next_rect;
        while (current_rect != NULL){
            next_rect = current_rect->next;
            free(current_rect);
            current_rect = next_rect;
        }
        rects = NULL;
}

void ei_app_create(ei_size_t main_window_size,
                   bool fullscreen){
    // Init the hardware
    hw_init();
    // Register all classes of widget and all geometry managers needed by the application
    ei_widgetclass_register(&ei_frameclass);
    ei_widgetclass_register(&ei_buttonclass);
    ei_widgetclass_register(&ei_toplevelclass);
    ei_widgetclass_register(&ei_entryclass);
    // Register all geometry manager for the app
    ei_geometrymanager_register(&placer_geometrymanager);
    // Create the root window
    root_surface = hw_create_window(main_window_size, fullscreen);
//    Create the root widget to access to the root window
    root_widget = ei_app_root_widget();
    ei_impl_frame_t* root_frame = (ei_impl_frame_t*) root_widget;
    ei_set_screen_location(root_widget,main_window_size);
    ei_set_content_rect(root_widget,main_window_size);
    root_frame->size = main_window_size;
    ei_bind_library();
}

void ei_app_free(void){
//  Release all the resources of the application :
//  Release the root widget and all its children
    ei_widget_destroy(root_widget);
//  Release surface
    hw_surface_free(root_surface);
    hw_surface_free(pick_surface);
//  Free bindings from the library
    ei_unbind_library();
//  Release the hardware resources
    hw_quit();
}

void ei_app_run(void){
//  Initialization of parameters
    ei_widget_t root = ei_app_root_widget();
    ei_surface_t surface = ei_app_root_surface();
    pick_surface = hw_surface_create(root_surface,((ei_impl_frame_t*)root_widget)->size,true);
    ei_rect_t* root_clipper = (ei_rect_t*) ei_widget_get_content_rect(root_widget);
//  Events management
    ei_event_t* current_event = malloc(sizeof(ei_event_t));
    ei_app_invalidate_rect(root_widget->screen_location);
    root_widget->wclass->drawfunc(root_widget,root_surface,pick_surface,root_clipper);
    hw_surface_update_rects(root_surface,rects);
    while (leave_request == false){
//      Updating the graphical window*
        double debut_boucle = hw_now();
        double avant_attente = hw_now();
        hw_event_wait_next(current_event);
        double apres_attente = hw_now();
        printf("Temps pour obtenir l'évènement d'après : %f\n", apres_attente - avant_attente);
        double avant_handle = hw_now();
        ei_handle_current_event(current_event);
        double apres_handle = hw_now();
        printf("Temps pour handle l'event : %f\n", apres_handle - avant_handle);
        double avant_up_rects = hw_now();
//      TODO : Need to update rect correctly when ctrl w or ctrl n in entry
        if (rects != NULL){
            hw_surface_update_rects(surface,rects);
        }
        double apres_up_rects = hw_now();
        printf("Temps pour update les rects : %f\n", apres_up_rects - avant_up_rects);
        // Free the list of rects
        double avant_free = hw_now();
        free_rects();
        double apres_free = hw_now();
        printf("Temps pour free : %f\n", apres_free - avant_free);
        double fin_boucle = hw_now();
        printf("Temps de boucle : %f\n", fin_boucle - debut_boucle);

    }
    free(current_event);
}

void ei_app_invalidate_rect(const ei_rect_t* rect_to_add){
//    TODO: fix
    ei_linked_rect_t** rects_add = &rects;
    ei_linked_rect_t* new_cell = malloc(sizeof(ei_linked_rect_t));
    new_cell->rect = *rect_to_add;
    new_cell->next = NULL;
    if (new_cell->rect.top_left.x < 0){
        new_cell->rect.top_left.x = 0;
    }
    if (new_cell->rect.top_left.y < 0){
        new_cell->rect.top_left.y = 0;
    }
    if (new_cell->rect.top_left.x > root_widget->screen_location->size.width){
        new_cell->rect.top_left.x = root_widget->screen_location->size.width;
    }
    if (new_cell->rect.top_left.y > root_widget->screen_location->size.height){
        new_cell->rect.top_left.y = root_widget->screen_location->size.height;
    }
    ei_linked_rect_t* current_cell = *rects_add;
    if (current_cell == NULL){
        *rects_add = new_cell;
        return;
    }
    while (current_cell->next != NULL){
        current_cell = current_cell->next;
    }
    current_cell->next = new_cell;
}

void ei_handle_current_event(ei_event_t* event) {
    bool event_handled = false;
    on_cursor_widget = ei_get_widget_on_cursor(event);
    ei_linked_bound_eventtype* current_bound_eventtype = bound_eventtype_list;
    while (event_handled == false && current_bound_eventtype != NULL){
        ei_linked_bound_eventtype* next_eventtype = current_bound_eventtype->next;
        if (event->type == current_bound_eventtype->bound_eventtype.eventtype){


            if (current_bound_eventtype->bound_eventtype.tag == NULL){
                if (on_cursor_widget == current_bound_eventtype->bound_eventtype.widget){
                    event_handled = current_bound_eventtype->bound_eventtype.callback(on_cursor_widget,event,current_bound_eventtype->bound_eventtype.user_param);
                }
            }
            if (current_bound_eventtype->bound_eventtype.tag != NULL) {
                if (strcmp(current_bound_eventtype->bound_eventtype.tag, "all") == 0) {
                    event_handled = current_bound_eventtype->bound_eventtype.callback(on_cursor_widget, event,
                                                                                      event->param.application.user_param);
                }
                else if (strcmp(on_cursor_widget->wclass->name, current_bound_eventtype->bound_eventtype.tag) == 0) {
                    event_handled = current_bound_eventtype->bound_eventtype.callback(on_cursor_widget, event,
                                                                                      event->param.application.user_param);
                }

            }
            if (on_cursor_widget == NULL){
                return;
            }
        }
        if (event_handled){
            double avant_dessin = hw_now();
            root_widget->wclass->drawfunc(root_widget,root_surface,pick_surface,(ei_rect_t*)ei_widget_get_content_rect(root_widget));
            double apres_dessin = hw_now();
            printf("Temps de dessin : %f\n", apres_dessin - avant_dessin);
        }
        current_bound_eventtype = next_eventtype;
    }
}


void ei_app_quit_request(void){
    leave_request = true;
}

ei_widget_t ei_app_root_widget(void) {
    // Create the root widget if it doesn't exist else return it
    if (root_widget == NULL){
        root_widget = ei_widget_create("frame", NULL, NULL, NULL);
    }
    return root_widget;
}

ei_surface_t ei_app_root_surface(void){
    return root_surface;
}