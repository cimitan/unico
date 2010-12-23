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
unico_cairo_draw_background (GtkThemingEngine *engine,
                             cairo_t *cr,
                             gint x,
                             gint y,
                             gint width,
                             gint height)
{
  gdouble line_width;
  gint radius;

  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  unico_cairo_draw_background_rect (engine, cr,
                                    x+line_width*2, y+line_width*2,
                                    width-line_width*4, height-line_width*4,
                                    radius, unico_get_corners (engine));
}

void
unico_cairo_draw_background_from_path (GtkThemingEngine *engine,
                                       cairo_t *cr,
                                       gdouble x,
                                       gdouble y,
                                       gdouble width,
                                       gdouble height)
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
        gdk_cairo_set_source_rgba (cr, background_color);

      cairo_fill (cr);
    }

  cairo_restore (cr);

  gdk_rgba_free (background_color);
}

void
unico_cairo_draw_background_rect (GtkThemingEngine *engine,
                                  cairo_t *cr,
                                  gdouble x,
                                  gdouble y,
                                  gdouble width,
                                  gdouble height,
                                  gint radius,
                                  UnicoCorners corners)
{
  unico_cairo_rounded_rect (cr, x, y, width, height, radius, corners);
  unico_cairo_draw_background_from_path (engine, cr, x, y, width, height);
}

void
unico_cairo_draw_frame (GtkThemingEngine *engine,
                        cairo_t *cr,
                        gint x,
                        gint y,
                        gint width,
                        gint height)
{
  UnicoCorners corners;
  gdouble line_width;
  gint radius;

  corners = unico_get_corners (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  unico_cairo_draw_stroke_outer_rect (engine, cr,
                                      x, y,
                                      width, height,
                                      radius+line_width, corners);

  unico_cairo_draw_stroke_inner_rect (engine, cr,
                                      x+line_width*2, y+line_width*2,
                                      width-line_width*4, height-line_width*4,
                                      radius-line_width, corners);

  unico_cairo_draw_border_rect (engine, cr,
                                x+line_width, y+line_width,
                                width-line_width*2, height-line_width*2,
                                radius, corners);
}

void
unico_cairo_draw_border_from_path (GtkThemingEngine *engine,
                                   cairo_t *cr,
                                   gdouble x,
                                   gdouble y,
                                   gdouble width,
                                   gdouble height)
{
  GtkStateFlags state;
  GdkRGBA *border_color;
  cairo_pattern_t *border_pat;
  gdouble line_width;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "border-color", &border_color,
                          "-unico-border-gradient", &border_pat,
                          NULL);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  line_width = cairo_get_line_width (cr);

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
        gdk_cairo_set_source_rgba (cr, border_color);

      cairo_stroke (cr);
    }

  cairo_restore (cr);

  gdk_rgba_free (border_color);
}

void
unico_cairo_draw_border_rect (GtkThemingEngine *engine,
                              cairo_t *cr,
                              gdouble x,
                              gdouble y,
                              gdouble width,
                              gdouble height,
                              gint radius,
                              UnicoCorners corners)
{
  gdouble line_width;

  unico_get_line_width (engine, &line_width);

  cairo_save (cr);

  cairo_set_line_width (cr, line_width);

  unico_cairo_rounded_rect_inner (cr, x, y, width, height, radius, corners);
  unico_cairo_draw_border_from_path (engine, cr, x, y, width, height);

  cairo_restore (cr);
}

void
unico_cairo_draw_stroke_inner_from_path (GtkThemingEngine *engine,
                                         cairo_t *cr,
                                         gdouble x,
                                         gdouble y,
                                         gdouble width,
                                         gdouble height)
{
  GtkStateFlags state;
  GdkRGBA *stroke_inner_color;
  cairo_pattern_t *stroke_inner_pat;
  gdouble line_width;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "-unico-stroke-inner-color", &stroke_inner_color,
                          "-unico-stroke-inner-gradient", &stroke_inner_pat,
                          NULL);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  line_width = cairo_get_line_width (cr);

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
        gdk_cairo_set_source_rgba (cr, stroke_inner_color);

      cairo_stroke (cr);
    }

  cairo_restore (cr);

  gdk_rgba_free (stroke_inner_color);
}

void
unico_cairo_draw_stroke_inner_rect (GtkThemingEngine *engine,
                                    cairo_t *cr,
                                    gdouble x,
                                    gdouble y,
                                    gdouble width,
                                    gdouble height,
                                    gint radius,
                                    UnicoCorners corners)
{
  gdouble line_width;

  unico_get_line_width (engine, &line_width);

  cairo_save (cr);

  cairo_set_line_width (cr, line_width);

  unico_cairo_rounded_rect_inner (cr, x, y, width, height, radius, corners);
  unico_cairo_draw_stroke_inner_from_path (engine, cr, x, y, width, height);

  cairo_restore (cr);
}

void
unico_cairo_draw_stroke_outer_from_path (GtkThemingEngine *engine,
                                         cairo_t *cr,
                                         gdouble x,
                                         gdouble y,
                                         gdouble width,
                                         gdouble height)
{
  GtkStateFlags state;
  GdkRGBA *stroke_outer_color;
  cairo_pattern_t *stroke_outer_pat;
  gdouble line_width;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "-unico-stroke-outer-color", &stroke_outer_color,
                          "-unico-stroke-outer-gradient", &stroke_outer_pat,
                          NULL);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  line_width = cairo_get_line_width (cr);

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
        gdk_cairo_set_source_rgba (cr, stroke_outer_color);

      cairo_stroke (cr);
    }

  cairo_restore (cr);

  gdk_rgba_free (stroke_outer_color);
}

void
unico_cairo_draw_stroke_outer_rect (GtkThemingEngine *engine,
                                    cairo_t *cr,
                                    gdouble x,
                                    gdouble y,
                                    gdouble width,
                                    gdouble height,
                                    gint radius,
                                    UnicoCorners corners)
{
  gdouble line_width;

  unico_get_line_width (engine, &line_width);

  cairo_save (cr);

  cairo_set_line_width (cr, line_width);

  unico_cairo_rounded_rect_inner (cr, x, y, width, height, radius, corners);
  unico_cairo_draw_stroke_outer_from_path (engine, cr, x, y, width, height);

  cairo_restore (cr);
}

void
unico_cairo_exchange_axis (cairo_t *cr,
                           gint *x,
                           gint *y,
                           gint *width,
                           gint *height)
{
  gint tmp;
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
                          gdouble x,
                          gdouble y,
                          gdouble width,
                          gdouble height,
                          gint radius,
                          UnicoCorners corners)
{
  if (radius <= 0)
    {
      cairo_rectangle (cr, x, y, width, height);
      return;
    }

  radius = MIN (radius, MIN (width/2.0, height/2.0));

  if (corners & UNICO_CORNER_TOPLEFT)
    cairo_move_to (cr, x+radius, y);
  else
    cairo_move_to (cr, x, y);

  if (corners & UNICO_CORNER_TOPRIGHT)
    cairo_arc (cr, x+width-radius, y+radius, radius, G_PI*1.5, G_PI*2);
  else
    cairo_line_to (cr, x+width, y);

  if (corners & UNICO_CORNER_BOTTOMRIGHT)
    cairo_arc (cr, x+width-radius, y+height-radius, radius, 0.0, G_PI*0.5);
  else
    cairo_line_to (cr, x+width, y+height);

  if (corners & UNICO_CORNER_BOTTOMLEFT)
    cairo_arc (cr, x+radius, y+height-radius, radius, G_PI*0.5, G_PI);
  else
    cairo_line_to (cr, x, y+height);

  if (corners & UNICO_CORNER_TOPLEFT)
    cairo_arc (cr, x+radius, y+radius, radius, G_PI, G_PI*1.5);
  else
    cairo_line_to (cr, x, y);
}

void
unico_cairo_rounded_rect_inner (cairo_t *cr,
                                gdouble x,
                                gdouble y,
                                gdouble width,
                                gdouble height,
                                gint radius,
                                UnicoCorners corners)
{
  gdouble line_width;

  line_width = cairo_get_line_width (cr);

  unico_cairo_rounded_rect (cr, x+(line_width/2.0),
                                y+(line_width/2.0),
                                width-line_width,
                                height-line_width,
                                radius, corners);
}

void
unico_cairo_rounded_rect_inverted (cairo_t *cr,
                                   gdouble x,
                                   gdouble y,
                                   gdouble width,
                                   gdouble height,
                                   gint radius,
                                   UnicoCorners corners)
{
  if (radius <= 0)
    {
      cairo_rectangle (cr, x, y, width, height);
      return;
    }

  radius = MIN (radius, MIN (width/2.0, height/2.0));

  cairo_translate (cr, x, y);

  if (corners & UNICO_CORNER_TOPLEFT)
    cairo_move_to (cr, 0, -radius);
  else
    cairo_move_to (cr, 0, 0);

  if (corners & UNICO_CORNER_BOTTOMLEFT)
    cairo_arc (cr, radius, height+radius, radius, G_PI, G_PI*1.5);
  else
    cairo_line_to (cr, 0, height);

  if (corners & UNICO_CORNER_BOTTOMRIGHT)
    cairo_arc (cr, width-radius, height+radius, radius, G_PI*1.5, G_PI*2.0);
  else
    cairo_line_to (cr, width, height);

  if (corners & UNICO_CORNER_TOPRIGHT)
    cairo_arc (cr, width-radius, -radius, radius, 0.0, G_PI*0.5);
  else
    cairo_line_to (cr, width, 0);

  if (corners & UNICO_CORNER_TOPLEFT)
    cairo_arc (cr, radius, -radius, radius, G_PI*0.5, G_PI);
  else
    cairo_line_to (cr, 0, 0);

  cairo_translate (cr, -x, -y);
}

void
unico_cairo_rounded_rect_inverted_inner (cairo_t *cr,
                                         gdouble x,
                                         gdouble y,
                                         gdouble width,
                                         gdouble height,
                                         gint radius,
                                         UnicoCorners corners)
{
  gdouble line_width;

  line_width = cairo_get_line_width (cr);

  unico_cairo_rounded_rect_inverted (cr, x+(line_width/2.0),
                                         y+(line_width/2.0),
                                         width-line_width,
                                         height-line_width,
                                         radius, corners);
}

void
unico_cairo_set_source_color_with_alpha (cairo_t *cr,
                                         GdkRGBA *color,
                                         gdouble alpha)
{
  g_return_if_fail (cr && color);

  cairo_set_source_rgba (cr, color->red, color->green, color->blue, alpha);
}
