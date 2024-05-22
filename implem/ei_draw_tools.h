/**
 * @file	ei_draw_tools.h
 *
 * @brief 	Private declarations for drawing tools.
 *
 */

#ifndef EI_DRAW_TOOLS_H
#define EI_DRAW_TOOLS_H

#include "ei_types.h"
#include "ei_linked_struct.h"

bool is_rect_in_surface(const ei_rect_t* rect, ei_surface_t surface);

bool is_rect_in_rect(const ei_rect_t* rect1, const ei_rect_t* rect2);
/**
 * @brief	Returns a linked list of points that represent an arc
 *
 * @param	center		The center of the arc
 * @param	radius		The radius of the arc
 * @param	start_angle	The start angle of the arc
 * @param	end_angle	The end angle of the arc
 *
 * @return	A linked list of points that represent the arc
 */
struct ei_linked_point_t* arc(ei_point_t center, int radius, float start_angle, float end_angle);


/**
 * @brief	Returns a linked list of points that represent a rounded frame
 *
 * @param	rect		The rectangle of the frame
 * @param	radius		The radius of the corners
 * @param	top_rounded	Whether the top corners are rounded
 * @param	bottom_rounded	Whether the bottom corners are rounded
 *
 * @return	A linked list of points that represent the rounded frame
 */
struct ei_linked_point_t* rounded_frame(ei_rect_t rect, int radius, bool top_rounded, bool bottom_rounded);

/**
 * @brief	Returns a point translated by a certain amount
 *
 * @param	point	The point to translate
 * @param	x	The x translation
 * @param	y	The y translation
 *
 * @return	The translated point
 */
ei_point_t translate_point(ei_point_t point, int x, int y);

/**
 * @brief	Returns a point that is symmetrical to the given point with respect to the given center.
 *
 * @param	point	The point for which to find the symmetrical point.
 * @param	center	The point with respect to which to find the symmetrical point.
 *with respect to the given center
 * @return	The symmetrical point.
 */
ei_point_t symmetrical_point(ei_point_t point, ei_point_t center);

/**
 * @brief	Returns a linked list of points that are the symetric of the given linked list of points with respect of the center.
 *
 * @param	linked_points	The linked list of points for which to find the symmetrical linked list of points.
 * @param	center	The point with respect to which to find the symmetrical linked list of points.
 *
 * @return	The symmetrical linked list of points.
 */
struct ei_linked_point_t* symmetrical_linked_points(struct ei_linked_point_t* linked_points, ei_point_t center);

/**
 * @brief	Adjusts the intensity of a color by a given factor.
 *
 * @param	color	The color to adjust.
 * @param	factor	The factor by which to adjust the color's intensity. A factor greater than 1 will increase the intensity, while a factor less than 1 will decrease it.
 *
 * @return	The color with adjusted intensity.
 */
ei_color_t adjust_color_intensity(ei_color_t color, double factor);

/**
 * @brief Calculates the position of the text within a button.
 *
 * This function calculates the position of the text within a button. The position is calculated based on the button's screen location, the text, the font, and the text anchor. The text anchor determines the alignment of the text within the button.
 *
 * @param screen_location A pointer to the rectangle representing the button's screen location.
 * @param text A pointer to the string representing the text to be displayed on the button.
 * @param font The font to be used for the text.
 * @param text_anchor The anchor that determines how the text is anchored within the button.
 *
 * @return The calculated position of the text within the button.
 */
ei_point_t calculate_text_position(const ei_rect_t* screen_location, const char* text, ei_font_t font, ei_anchor_t text_anchor);

/**
 * @brief Draws an image within a rectangle.
 *
 * @param surface The surface to draw on.
 * @param rect The rectangle to draw the image within.
 * @param img The image to draw.
 * @param img_rect A pointer to a rectangle defining the portion of the image to draw. If NULL, the entire image is drawn.
 * @param border_thickness The thickness of the border around the image (the one of the frame).
 * @param clipper A pointer to a clipping rectangle. Pixels outside this rectangle won't be modified by the drawing. If NULL, no clipping will be done.
 */
void draw_image(ei_surface_t surface, ei_rect_t* rect, ei_surface_t img, ei_rect_ptr_t img_rect, int border_thickness, ei_rect_t* clipper);


#endif