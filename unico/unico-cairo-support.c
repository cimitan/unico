/* The Unico Theme Engine for Gtk+.
 * Copyright (C) 2011 Andrea Cimitan <andrea.cimitan@canonical.com>
 *
 * This  library is free  software; you can  redistribute it and/or
 * modify it  under  the terms  of the  GNU Lesser  General  Public
 * License  as published  by the Free  Software  Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed  in the hope that it will be useful,
 * but  WITHOUT ANY WARRANTY; without even  the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License  along  with  this library;  if not,  write to  the Free
 * Software Foundation, Inc., 51  Franklin St, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <cairo.h>

#include "unico-cairo-support.h"
#include "unico-support.h"
#include "unico-types.h"

void
unico_cairo_draw_background (cairo_t *cr,
                             GtkThemingEngine *engine,
                             int x,
                             int y,
                             int width,
                             int height)
{
  GtkBorder *border;
  GtkStateFlags state;
  double line_width;
  int radius;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "border-radius", &radius,
                          "border-width", &border,
                          NULL);

  line_width = MIN (MIN (border->top, border->bottom),
                    MIN (border->left, border->right));

  unico_cairo_draw_background_rect (cr, engine,
                                        x+1+line_width*2, y+1+line_width*2,
                                        width-2-(line_width*4), height-2-(line_width*4),
                                        radius-1, unico_get_corners (engine));

  gtk_border_free (border);
}

void
unico_cairo_draw_background_from_path (cairo_t *cr,
                                       GtkThemingEngine *engine,
                                       double x,
                                       double y,
                                       double width,
                                       double height)
{
  GtkStateFlags state;
  GdkRGBA *background_color;
  cairo_pattern_t *background_pat;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "background-color", &background_color,
                          "background-image", &background_pat,
                          NULL);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  if (background_pat || background_color)
    {
      if (background_pat)
        {
          cairo_matrix_t matrix;
          cairo_matrix_init_scale (&matrix, 1.0/width, 1.0/height);
          cairo_pattern_set_matrix (background_pat, &matrix);
          cairo_set_source (cr, background_pat);
          cairo_pattern_destroy (background_pat);
        }
      else
        unico_cairo_set_source_color (cr, background_color);

      cairo_fill (cr);
    }

  cairo_restore (cr);

  gdk_rgba_free (background_color);
}

void
unico_cairo_draw_background_rect (cairo_t *cr,
                                  GtkThemingEngine *engine,
                                  double x,
                                  double y,
                                  double width,
                                  double height,
                                  int radius,
                                  UnicoCorners corners)
{
  unico_cairo_rounded_rect (cr, x, y, width, height, radius, corners);
  unico_cairo_draw_background_from_path (cr, engine, x, y, width, height);
}

void
unico_cairo_draw_frame (cairo_t *cr,
                        GtkThemingEngine *engine,
                        int x,
                        int y,
                        int width,
                        int height)
{
  GtkBorder *border;
  GtkStateFlags state;
  UnicoCorners corners;
  double line_width;
  int radius;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "border-radius", &radius,
                          "border-width", &border,
                          NULL);

  corners = unico_get_corners (engine);

  line_width = MIN (MIN (border->top, border->bottom),
                    MIN (border->left, border->right));

  unico_cairo_draw_stroke_outer_rect (cr, engine,
                                          x, y,
                                          width, height,
                                          radius+line_width, corners);

  unico_cairo_draw_stroke_inner_rect (cr, engine,
                                          x+(line_width*2), y+(line_width*2),
                                          width-(line_width*4), height-(line_width*4),
                                          radius-line_width, corners);

  unico_cairo_draw_border_rect (cr, engine,
                                    x+line_width, y+line_width,
                                    width-(line_width*2), height-(line_width*2),
                                    radius, corners);

  gtk_border_free (border);
}

void
unico_cairo_draw_border_from_path (cairo_t *cr,
                                   GtkThemingEngine *engine,
                                   double x,
                                   double y,
                                   double width,
                                   double height)
{
  GtkBorder *border;
  GtkStateFlags state;
  GdkRGBA *border_color;
  cairo_pattern_t *border_pat;
  double line_width;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "border-width", &border,
                          "border-color", &border_color,
                          "-unico-border-gradient", &border_pat,
                          NULL);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  line_width = MIN (MIN (border->top, border->bottom),
                    MIN (border->left, border->right));
  cairo_set_line_width (cr, line_width);

  if (border_pat || border_color)
    {
      if (border_pat)
        {
          cairo_matrix_t matrix;
          cairo_matrix_init_scale (&matrix, 1.0/(width-line_width), 1.0/(height-line_width));
          cairo_pattern_set_matrix (border_pat, &matrix);
          cairo_set_source (cr, border_pat);
          cairo_pattern_destroy (border_pat);
        }
      else
        unico_cairo_set_source_color (cr, border_color);

      cairo_stroke (cr);
    }

  cairo_restore (cr);

  gdk_rgba_free (border_color);
  gtk_border_free (border);
}

void
unico_cairo_draw_border_rect (cairo_t *cr,
                              GtkThemingEngine *engine,
                              double x,
                              double y,
                              double width,
                              double height,
                              int radius,
                              UnicoCorners corners)
{
  GtkBorder *border;
  GtkStateFlags state;
  double line_width;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "border-width", &border,
                          NULL);

  line_width = MIN (MIN (border->top, border->bottom),
                    MIN (border->left, border->right));

  cairo_save (cr);

  cairo_set_line_width (cr, line_width);

  unico_cairo_rounded_rect_inner (cr, x, y, width, height, radius, corners);
  unico_cairo_draw_border_from_path (cr, engine, x, y, width, height);

  cairo_restore (cr);

  gtk_border_free (border);
}

void
unico_cairo_draw_stroke_inner_from_path (cairo_t *cr,
                                         GtkThemingEngine *engine,
                                         double x,
                                         double y,
                                         double width,
                                         double height)
{
  GtkBorder *border;
  GtkStateFlags state;
  GdkRGBA *stroke_inner_color;
  cairo_pattern_t *stroke_inner_pat;
  double line_width;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "border-width", &border,
                          "-unico-stroke-inner-color", &stroke_inner_color,
                          "-unico-stroke-inner-gradient", &stroke_inner_pat,
                          NULL);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  line_width = MIN (MIN (border->top, border->bottom),
                    MIN (border->left, border->right));
  cairo_set_line_width (cr, line_width);

  if (stroke_inner_pat || stroke_inner_color)
    {
      if (stroke_inner_pat)
        {
          cairo_matrix_t matrix;
          cairo_matrix_init_scale (&matrix, 1.0/(width-line_width), 1.0/(height-line_width));
          cairo_pattern_set_matrix (stroke_inner_pat, &matrix);
          cairo_set_source (cr, stroke_inner_pat);
          cairo_pattern_destroy (stroke_inner_pat);
        }
      else
        unico_cairo_set_source_color (cr, stroke_inner_color);

      cairo_stroke (cr);
    }

  cairo_restore (cr);

  gdk_rgba_free (stroke_inner_color);
  gtk_border_free (border);
}

void
unico_cairo_draw_stroke_inner_rect (cairo_t *cr,
                                    GtkThemingEngine *engine,
                                    double x,
                                    double y,
                                    double width,
                                    double height,
                                    int radius,
                                    UnicoCorners corners)
{
  GtkBorder *border;
  GtkStateFlags state;
  double line_width;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "border-width", &border,
                          NULL);

  line_width = MIN (MIN (border->top, border->bottom),
                    MIN (border->left, border->right));

  cairo_save (cr);

  cairo_set_line_width (cr, line_width);

  unico_cairo_rounded_rect_inner (cr, x, y, width, height, radius, corners);
  unico_cairo_draw_stroke_inner_from_path (cr, engine, x, y, width, height);

  cairo_restore (cr);

  gtk_border_free (border);
}

void
unico_cairo_draw_stroke_outer_from_path (cairo_t *cr,
                                         GtkThemingEngine *engine,
                                         double x,
                                         double y,
                                         double width,
                                         double height)
{
  GtkBorder *border;
  GtkStateFlags state;
  GdkRGBA *stroke_outer_color;
  cairo_pattern_t *stroke_outer_pat;
  double line_width;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "border-width", &border,
                          "-unico-stroke-outer-color", &stroke_outer_color,
                          "-unico-stroke-outer-gradient", &stroke_outer_pat,
                          NULL);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  line_width = MIN (MIN (border->top, border->bottom),
                    MIN (border->left, border->right));
  cairo_set_line_width (cr, line_width);

  if (stroke_outer_pat || stroke_outer_color)
    {
      if (stroke_outer_pat)
        {
          cairo_matrix_t matrix;
          cairo_matrix_init_scale (&matrix, 1.0/(width-line_width), 1.0/(height-line_width));
          cairo_pattern_set_matrix (stroke_outer_pat, &matrix);
          cairo_set_source (cr, stroke_outer_pat);
          cairo_pattern_destroy (stroke_outer_pat);
          cairo_identity_matrix (cr);
        }
      else
        unico_cairo_set_source_color (cr, stroke_outer_color);

      cairo_stroke (cr);
    }

  cairo_restore (cr);

  gdk_rgba_free (stroke_outer_color);
  gtk_border_free (border);
}

void
unico_cairo_draw_stroke_outer_rect (cairo_t *cr,
                                    GtkThemingEngine *engine,
                                    double x,
                                    double y,
                                    double width,
                                    double height,
                                    int radius,
                                    UnicoCorners corners)
{
  GtkBorder *border;
  GtkStateFlags state;
  double line_width;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "border-width", &border,
                          NULL);

  line_width = MIN (MIN (border->top, border->bottom),
                    MIN (border->left, border->right));

  cairo_save (cr);

  cairo_set_line_width (cr, line_width);

  unico_cairo_rounded_rect_inner (cr, x, y, width, height, radius, corners);
  unico_cairo_draw_stroke_outer_from_path (cr, engine, x, y, width, height);

  cairo_restore (cr);

  gtk_border_free (border);
}

void
unico_cairo_exchange_axis (cairo_t *cr,
                           int *x,
                           int *y,
                           int *width,
                           int *height)
{
  int tmp;
  cairo_matrix_t matrix;

  cairo_translate (cr, *x, *y);
  cairo_matrix_init (&matrix, 0, 1, 1, 0, 0, 0);

  cairo_transform (cr, &matrix);

  /* swap width/height */
  tmp = *width;
  *x = 0;
  *y = 0;
  *width = *height;
  *height = tmp;
}

void
unico_cairo_rounded_rect (cairo_t *cr,
                          double x,
                          double y,
                          double w,
                          double h,
                          int radius,
                          UnicoCorners corners)
{
  if (radius <= 0)
    {
      cairo_rectangle (cr, x, y, w, h);
      return;
    }

  radius = MIN (radius, MIN (w/2.0, h/2.0));

  if (corners & UNICO_CORNER_TOPLEFT)
    cairo_move_to (cr, x+radius, y);
  else
    cairo_move_to (cr, x, y);

  if (corners & UNICO_CORNER_TOPRIGHT)
    cairo_arc (cr, x+w-radius, y+radius, radius, G_PI*1.5, G_PI*2);
  else
    cairo_line_to (cr, x+w, y);

  if (corners & UNICO_CORNER_BOTTOMRIGHT)
    cairo_arc (cr, x+w-radius, y+h-radius, radius, 0, G_PI*0.5);
  else
    cairo_line_to (cr, x+w, y+h);

  if (corners & UNICO_CORNER_BOTTOMLEFT)
    cairo_arc (cr, x+radius, y+h-radius, radius, G_PI*0.5, G_PI);
  else
    cairo_line_to (cr, x, y+h);

  if (corners & UNICO_CORNER_TOPLEFT)
    cairo_arc (cr, x+radius, y+radius, radius, G_PI, G_PI*1.5);
  else
    cairo_line_to (cr, x, y);
}

void
unico_cairo_rounded_rect_inner (cairo_t *cr,
                                double x,
                                double y,
                                double width,
                                double height,
                                int radius,
                                UnicoCorners corners)
{
  double line_width;

  line_width = cairo_get_line_width (cr);

  unico_cairo_rounded_rect (cr, x+(line_width/2.0),
                                y+(line_width/2.0),
                                width-line_width,
                                height-line_width,
                                radius, corners);
}

void
unico_cairo_set_source_color (cairo_t *cr,
                              GdkRGBA *color)
{
  g_return_if_fail (cr && color);

  cairo_set_source_rgba (cr, color->red, color->green, color->blue, color->alpha);
}
