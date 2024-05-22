#include "ei_widget_configure.h"

#include <ei_placer.h>

#include "ei_class.h"
#include <stdio.h>

void ei_frame_configure(ei_widget_t widget,
                        ei_size_t* requested_size,
                        const ei_color_t* color,
                        int* border_width,
                        ei_relief_t* relief,
                        ei_string_t* text,
                        ei_font_t* text_font,
                        ei_color_t* text_color,
                        ei_anchor_t* text_anchor,
                        ei_surface_t* img,
                        ei_rect_ptr_t* img_rect,
                        ei_anchor_t* img_anchor ){
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;
    frame->color = color ? *color : ei_default_background_color;
//    Allocate memory for the text because it may be freed later
    if (text != NULL) {
        frame->text = malloc(strlen(*text)+1);
        strcpy(frame->text, *text);
    }
    else {
        frame->text = NULL;
    }

    frame->text_font = text_font ? *text_font : ei_default_font;
    frame->text_color = text_color ? *text_color : ei_font_default_color;
    frame->text_anchor = text_anchor ? *text_anchor : ei_anc_center;

    frame->border_width = border_width ? *border_width : 0;
    frame->relief = relief ? *relief : ei_relief_none;

    if(requested_size != NULL) {
        ei_set_screen_location(widget, *requested_size);
        ei_set_content_rect(widget, *requested_size);
        widget->requested_size = *requested_size;
    }else {
        widget->requested_size = (ei_size_t){0,0};
    }

//    If there is an image given, we copy it to the widget's image
    if(img != NULL) {
        frame->img = malloc(sizeof (ei_surface_t));
        frame->img = *img;
    }

//    If there is an image rect given, we copy it to the widget's image rect
    if(img_rect != NULL) {
        frame->img_rect = malloc(sizeof(ei_rect_t));
        frame->img_rect = *img_rect;
    }else {
        if(frame->img != NULL) {
            *frame->img_rect = hw_surface_get_rect(frame->img);
        }else {
            frame->img_rect = NULL;
        }
    }

    frame->img_anchor = img_anchor ? *img_anchor : ei_anc_center;

}

void ei_button_configure(ei_widget_t widget,
                         ei_size_t* requested_size,
                         const ei_color_t* color,
                         int* border_width,
                         int* corner_radius,
                         ei_relief_t* relief,
                         ei_string_t* text,
                         ei_font_t*	text_font,
                         ei_color_t* text_color,
                         ei_anchor_t* text_anchor,
                         ei_surface_t* img,
                         ei_rect_ptr_t* img_rect,
                         ei_anchor_t* img_anchor,
                         ei_callback_t* callback,
                         ei_user_param_t* user_param) {
    ei_impl_button_t *button = (ei_impl_button_t *) widget;
    button->text = text ? *text : NULL;
    button->text_font = text_font ? *text_font : ei_default_font;
    button->text_color = text_color ? *text_color : ei_font_default_color;
    button->text_anchor = text_anchor ? *text_anchor : ei_anc_center;

//    If there is an image given, we copy it to the widget's image
    if(img != NULL) {
        button->img = hw_surface_create(root_surface, hw_surface_get_size(*img), true);
        ei_copy_surface(button->img, NULL, *img, NULL,true);
    }

//    If there is an image rect given, we copy it to the widget's image rect
    if(img_rect != NULL) {
        button->img_rect = malloc(sizeof (ei_rect_t));
        button->img_rect->top_left = (*img_rect)->top_left;
        button->img_rect->size = (*img_rect)->size;
    }else {
        if(button->img != NULL) {
            *button->img_rect = hw_surface_get_rect(button->img);
        }else {
            button->img_rect = NULL;
        }
    }

    button->img_anchor = img_anchor ? *img_anchor : ei_anc_center;
    button->color = color ? *color : ei_default_background_color;

    button->border_width = border_width ? *border_width : k_default_button_border_width;
    button->corner_radius = corner_radius ? (*corner_radius ? *corner_radius : 1) : button->corner_radius;
    button->relief = relief ? *relief : ei_relief_raised;

    if(requested_size != NULL) {
        ei_set_screen_location(widget, *requested_size);
        ei_set_content_rect(widget, *requested_size);
        widget->requested_size = *requested_size;
    }

    if (user_param != NULL){
        button->user_param = *user_param;
    }

    if (callback != NULL){
        ei_bind(ei_ev_mouse_buttonup,widget,NULL,*callback,button->user_param);
    }


}

void			ei_toplevel_configure		(ei_widget_t		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 ei_string_t*		title,
							 bool*			closable,
							 ei_axis_set_t*		resizable,
						 	 ei_size_ptr_t*		min_size) {
    ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*) widget;
    toplevel->color = color ? *color : ei_default_background_color;
    toplevel->border_width = border_width ? *border_width : 4;
    toplevel->title = title ? *title : "Toplevel";
    toplevel->title_fontstyle = ei_default_font;
    toplevel->closable = closable ? *closable : true;
    toplevel->resizable = resizable ? *resizable : ei_axis_both;
    if(min_size == NULL) {
        toplevel->min_size = malloc(sizeof(ei_size_t));
        ei_size_t title_size;
        hw_text_compute_size(toplevel->title, toplevel->title_fontstyle, &title_size.width, &title_size.height);
        toplevel->min_size->width = fmax(title_size.width + 3 * PADDING + CLOSE_BUTTON_SIZE, 160); //To ensure that the title is always completely displayed
        toplevel->min_size->height = 120;
    }else {
        toplevel->min_size = *min_size;
    }

    if (requested_size != NULL) {
        if(widget->content_rect == NULL) {
            widget->content_rect = calloc(1,sizeof(ei_rect_t));
        }
        ei_set_screen_location(widget, *requested_size);
        *widget->content_rect = (ei_rect_t){{widget->screen_location->top_left.x + toplevel->border_width,widget->screen_location->top_left.y + HEADER_HEIGHT},{widget->screen_location->size.width - 2 * toplevel->border_width,widget->screen_location->size.height - toplevel->border_width - HEADER_HEIGHT}};
        widget->requested_size = *requested_size;
    }else {
        if(widget->content_rect == NULL) {
            widget->content_rect = calloc(1,sizeof(ei_rect_t));
        }
        ei_set_screen_location(widget, (ei_size_t){320,240});
        *widget->content_rect = (ei_rect_t){{widget->screen_location->top_left.x + toplevel->border_width,widget->screen_location->top_left.y + HEADER_HEIGHT},{widget->screen_location->size.width - 2 * toplevel->border_width,widget->screen_location->size.height - toplevel->border_width - HEADER_HEIGHT}};
        widget->requested_size = (ei_size_t){320,240};
    }

    if(toplevel->closable) {
        ei_widget_t close_button = ei_widget_create("button",(ei_widget_t)toplevel, NULL, NULL);
        ei_button_configure(close_button,
        &(ei_size_t){CLOSE_BUTTON_SIZE,CLOSE_BUTTON_SIZE},
        &CLOSE_BUTTON_COLOR,
        &(int){CLOSE_BUTTON_BORDER_WIDTH},
        &(int){10},
        &(ei_relief_t){ei_relief_raised},
        NULL,
        NULL,
        &(ei_color_t){0x00, 0x00, 0x00, 0xff},
        &(ei_anchor_t){ei_anc_center},
        NULL, NULL, NULL,
        NULL,
        NULL);
        ei_impl_button_t* actual_button = (ei_impl_button_t*) close_button;
        actual_button->is_toplevel_close_button = true;
        ei_place(close_button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
}