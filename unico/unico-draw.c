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

#include <gtk/gtk.h>
#include <cairo.h>

#include "unico-cairo-support.h"
#include "unico-draw.h"
#include "unico-types.h"

static void
unico_draw_button_background (cairo_t *cr,
                              GtkThemingEngine *engine,
                              int x,
                              int y,
                              int width,
                              int height,
                              UnicoButtonParameters *button)
{
  unico_draw_background (cr, engine, x, y, width, height);
}

static void
unico_draw_button_frame (cairo_t *cr,
                         GtkThemingEngine *engine,
                         int x,
                         int y,
                         int width,
                         int height,
                         UnicoButtonParameters *button)
{
  unico_draw_frame (cr, engine, x, y, width, height);
}

static void
unico_draw_tab (cairo_t *cr,
                GtkThemingEngine* engine,
                int x,
                int y,
                int width,
                int height,
                UnicoTabParameters *tab)
{
  GtkBorder *border_width;
  GtkStateFlags state;
  GdkRGBA *background_color, *border_color, *stroke_inner_color;
  UnicoCorners corners;
  cairo_pattern_t *background_pat, *border_pat, *stroke_inner_pat;
  double line_width;
  int radius;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "border-color", &border_color,
                          "border-radius", &radius,
                          "border-width", &border_width,
                          "background-color", &background_color,
                          "background-image", &background_pat,
                          "-unico-border-gradient", &border_pat,
                          "-unico-stroke-inner-color", &stroke_inner_color,
                          "-unico-stroke-inner-gradient", &stroke_inner_pat,
                          NULL);

  if (state & GTK_STATE_FLAG_ACTIVE)
    cairo_rectangle (cr, x, y, width, height);
  else
    cairo_rectangle (cr, x, y, width+1, height+1);
  cairo_clip (cr);

  /* Make the tabs slightly bigger than they should be, to create a gap */
  if (tab->gap_side == GTK_POS_TOP || tab->gap_side == GTK_POS_BOTTOM)
    {
      height += 3.0;

      if (tab->gap_side == GTK_POS_TOP)
        cairo_translate (cr, 0.0, -3.0); /* gap at the other side */
    }
  else
    {
      width += 3.0;

      if (tab->gap_side == GTK_POS_LEFT)
        cairo_translate (cr, -3.0, 0.0); /* gap at the other side */
    }

  cairo_save (cr);

  line_width = MIN (MIN (border_width->top, border_width->bottom),
                    MIN (border_width->left, border_width->right));
  cairo_set_line_width (cr, line_width);

  switch (tab->gap_side)
    {
      case GTK_POS_TOP:
        {
          height += line_width*2;
          corners = UNICO_CORNER_BOTTOMLEFT | UNICO_CORNER_BOTTOMRIGHT;
          break;
        }
      case GTK_POS_BOTTOM:
        {
          height += line_width*2;
          corners = UNICO_CORNER_TOPLEFT | UNICO_CORNER_TOPRIGHT;
          break;
        }
      case GTK_POS_LEFT:
        {
          width += line_width*2;
          corners = UNICO_CORNER_TOPRIGHT | UNICO_CORNER_BOTTOMRIGHT;
          break;
        }
      case GTK_POS_RIGHT:
        {
          width += line_width*2;
          corners = UNICO_CORNER_TOPLEFT | UNICO_CORNER_BOTTOMLEFT;
          break;
        }
    }

  /* background */
  if (background_pat || background_color)
    {
      cairo_save (cr);

      unico_rounded_rectangle (cr, x+line_width,
                                   y+line_width,
                                   width-1-(line_width*2),
                                   height-1-(line_width*2),
                                   radius, UNICO_CORNER_ALL);

      if (background_pat)
        {
          cairo_scale (cr, width-1-(line_width*2), height-1-(line_width*2));
          cairo_set_source (cr, background_pat);
          cairo_pattern_destroy (background_pat);
          cairo_identity_matrix (cr);
        }
      else
        unico_set_source_color (cr, background_color);

      cairo_fill (cr);

      cairo_restore (cr);
    }

  if (stroke_inner_pat || stroke_inner_color)
    {
      cairo_save (cr);

      unico_rounded_rectangle_inner (cr, x+line_width,
                                         y+line_width,
                                         width-1-line_width*2,
                                         height-1-line_width*2,
                                         radius-line_width, corners);

      if (stroke_inner_pat)
        {
          cairo_scale (cr, width-1-line_width*2, height-1-line_width*2);
          cairo_set_source (cr, stroke_inner_pat);
          cairo_pattern_destroy (stroke_inner_pat);
          cairo_identity_matrix (cr);
        }
      else
        unico_set_source_color (cr, stroke_inner_color);

      cairo_stroke (cr);

      cairo_restore (cr);
    }

  if (border_pat || border_color)
    {
      cairo_save (cr);

      unico_rounded_rectangle_inner (cr, x,
                                         y,
                                         width-1,
                                         height-1,
                                         radius, corners);

      if (border_pat)
        {
          cairo_scale (cr, width-1, height-1);
          cairo_set_source (cr, border_pat);
          cairo_pattern_destroy (border_pat);
          cairo_identity_matrix (cr);
        }
      else
        unico_set_source_color (cr, border_color);

      cairo_stroke (cr);

      cairo_restore (cr);
    }

  cairo_restore (cr);

  gdk_rgba_free (background_color);
  gdk_rgba_free (border_color);
  gdk_rgba_free (stroke_inner_color);
}

void
unico_register_style_default (UnicoStyleFunctions *functions)
{
  g_assert (functions);

  functions->draw_button_background = unico_draw_button_background;
  functions->draw_button_frame      = unico_draw_button_frame;
  functions->draw_tab               = unico_draw_tab;
}

