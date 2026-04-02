#pragma once

#include <cvl/cvl_core.h>

#define NO_CHANGE -1
#define INVERT -2

#ifdef __cplusplus
extern "C" {
#endif

void cvl_draw_ellipse_filled(Image img, Point center, Point radius, int r, int g, int b, int i);

void cvl_draw_rectangle_filled(Image img, Point p1, Point p2, int r, int g, int b, int i);

void cvl_draw_line(Image img, Point p1, Point p2, int width, int dash, int gap, int r, int g, int b, int i);

void cvl_draw_line_solid(Image img, Point p1, Point p2, int r, int g, int b, int i);

void cvl_draw_rectangle(Image img, Point p1, Point p2, int width, int dash, int gap, int r, int g, int b, int i);

void cvl_draw_ellipse(Image img, Point center, Point radius, int width, int dash, int gap, int r, int g, int b, int i);

#ifdef __cplusplus
}
#endif
