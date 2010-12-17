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
unico_draw_background (cairo_t *cr,
                       GtkThemingEngine *engine,
                       int x,
                       int y,
                       int width,
                       int height)
{
  GtkStateFlags state;
  GdkRGBA *background_color;
  cairo_pattern_t *background_pat;
  int radius;

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get (engine, state,
                          "border-radius", &radius,
                          "background-color", &background_color,
                          "background-image", &background_pat,
                          NULL);

  /* background */
  if (background_pat || background_color)
    {
      cairo_save (cr);

      unico_rounded_rectangle (cr, x+1, y+1, width-2, height-2, radius, UNICO_CORNER_ALL);

      if (background_pat)
        {
          cairo_scale (cr, width-2, height-2);
          cairo_set_source (cr, background_pat);
          cairo_pattern_destroy (background_pat);
          cairo_identity_matrix (cr);
        }
      else
        unico_set_source_color (cr, background_color);

      cairo_fill (cr);

      cairo_restore (cr);
    }

  gdk_rgba_free (background_color);
}


void
unico_draw_frame (cairo_t *cr,
                  GtkThemingEngine *engine,
                  int x,
                  int y,
                  int width,
                  int height)
{
  GtkStateFlags state;
  GdkRGBA *border_color, *stroke_inner_color, *stroke_outer_color;
  cairo_pattern_t *border_pat, *stroke_inner_pat, *stroke_outer_pat;
  int radius;

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get (engine, state,
                          "border-radius", &radius,
                          "border-color", &border_color,
                          "-unico-border-gradient", &border_pat,
                          "-unico-stroke-inner-color", &stroke_inner_color,
                          "-unico-stroke-inner-gradient", &stroke_inner_pat,
                          "-unico-stroke-outer-color", &stroke_outer_color,
                          "-unico-stroke-outer-gradient", &stroke_outer_pat,
                          NULL);

  /* stroke outer */
  if (stroke_outer_pat || stroke_outer_color)
    {
      cairo_save (cr);

      unico_rounded_rectangle (cr, x+0.5, y+0.5, width-1, height-1, radius+1, UNICO_CORNER_ALL);

      if (stroke_outer_pat)
        {
          cairo_scale (cr, width-1, height-1);
          cairo_set_source (cr, stroke_outer_pat);
          cairo_pattern_destroy (stroke_outer_pat);
          cairo_identity_matrix (cr);
        }
      else
        unico_set_source_color (cr, stroke_outer_color);

      cairo_stroke (cr);

      cairo_restore (cr);
    }

  /* stroke inner */
  if (stroke_inner_pat || stroke_inner_color)
    {
      cairo_save (cr);

      unico_rounded_rectangle (cr, x+2.5, y+2.5, width-5, height-5, radius-1, UNICO_CORNER_ALL);

      if (stroke_inner_pat)
        {
          cairo_scale (cr, width-5, height-5);
          cairo_set_source (cr, stroke_inner_pat);
          cairo_pattern_destroy (stroke_inner_pat);
          cairo_identity_matrix (cr);
        }
      else
        unico_set_source_color (cr, stroke_inner_color);

      cairo_stroke (cr);

      cairo_restore (cr);
    }

  /* border */
  if (border_pat || border_color)
    {
      cairo_save (cr);

      unico_rounded_rectangle (cr, x+1.5, y+1.5, width-3, height-3, radius, UNICO_CORNER_ALL);

      if (border_pat)
      {
        cairo_scale (cr, width-3, height-3);
        cairo_set_source (cr, border_pat);
        cairo_pattern_destroy (border_pat);
        cairo_identity_matrix (cr);
      }
      else
        unico_set_source_color (cr, border_color);

      cairo_stroke (cr);

      cairo_restore (cr);
    }

  gdk_rgba_free (border_color);
  gdk_rgba_free (stroke_inner_color);
  gdk_rgba_free (stroke_outer_color);
}

void
unico_rounded_rectangle (cairo_t *cr,
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
unico_set_source_color (cairo_t *cr,
                        GdkRGBA *color)
{
  g_return_if_fail (cr && color);

  cairo_set_source_rgba (cr, color->red, color->green, color->blue, color->alpha);
}
