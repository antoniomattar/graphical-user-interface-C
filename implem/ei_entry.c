#include "ei_entry.h"
#include "ei_class.h"


void ei_entry_configure(ei_widget_t widget,
                        int* requested_char_size,
                        const ei_color_t* color,
                        int* border_width,
                        ei_font_t* text_font,
                        ei_color_t* text_color){
	ei_impl_entry_t* entry = (ei_impl_entry_t*)widget;
	entry->requested_char_size = requested_char_size ? *requested_char_size : 10;
	entry->color = color ? *color : ei_default_background_color;
	entry->border_width = border_width ? *border_width : 2;
	entry->text_font = text_font ? *text_font : ei_default_font;
	entry->text_color = text_color ? *text_color : ei_font_default_color;
}

void			ei_entry_set_text		(ei_widget_t		widget,
							 ei_const_string_t 	text) {
	ei_impl_entry_t* entry = (ei_impl_entry_t*)widget;
	entry->text = text;
}

ei_const_string_t 	ei_entry_get_text		(ei_widget_t		widget) {
	return ((ei_impl_entry_t*)widget)->text;
}

void			ei_entry_give_focus		(ei_widget_t		widget) {
	//TODO
}
