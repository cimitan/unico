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

#define UNICO_RECTANGLE_SET(rect, _x, _y, _w, _h) (rect).x      = (_x); \
                                                  (rect).y      = (_y); \
                                                  (rect).width  = (_w); \
                                                  (rect).height = (_h);

static void
unico_draw_button_background (cairo_t *cr,
                              GtkThemingEngine *engine,
                              int x,
                              int y,
                              int width,
                              int height,
                              UnicoButtonParameters *button)
{
  unico_cairo_draw_background (cr, engine, x, y, width, height);
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
  unico_cairo_draw_frame (cr, engine, x, y, width, height);
}

static void
unico_get_frame_gap_clip (int x,
                          int y,
                          int width,
                          int height,
                          GdkRectangle *bevel,
                          GdkRectangle *border,
                          UnicoFrameParameters *frame)
{
  switch (frame->gap_side)
    {
      case GTK_POS_TOP:
        {
          UNICO_RECTANGLE_SET (*bevel, 2.0+frame->gap_x, 0.0,
                               frame->gap_width-3, frame->gap_width);
          UNICO_RECTANGLE_SET (*border, 1.0+frame->gap_x, 0.0,
                               frame->gap_width-2, frame->gap_width);
          break;
        }
      case GTK_POS_BOTTOM:
        {
          UNICO_RECTANGLE_SET (*bevel, 2.0+frame->gap_x, height-2.0,
                               frame->gap_width-3, frame->gap_width);
          UNICO_RECTANGLE_SET (*border, 1.0+frame->gap_x, height-1.0,
                               frame->gap_width-2, frame->gap_width);
          break;
        }
      case GTK_POS_LEFT:
        {
          UNICO_RECTANGLE_SET (*bevel, 0.0, 2.0+frame->gap_x,
                               frame->gap_width, frame->gap_width-3);
          UNICO_RECTANGLE_SET (*border, 0.0, 1.0+frame->gap_x,
                               frame->gap_width, frame->gap_width-2);
          break;
        }
      case GTK_POS_RIGHT:
        {
          UNICO_RECTANGLE_SET (*bevel, width-2.0, 2.0+frame->gap_x,
                               frame->gap_width, frame->gap_width-3);
          UNICO_RECTANGLE_SET (*border, width-1.0, 1.0+frame->gap_x,
                               frame->gap_width, frame->gap_width-2);
          break;
        }
    }
}

static void
unico_draw_frame (cairo_t *cr,
                  GtkThemingEngine *engine,
                  int x,
                  int y,
                  int width,
                  int height,
                  UnicoFrameParameters *frame)
{
  GdkRectangle bevel_clip = {0, 0, 0, 0};
  GdkRectangle frame_clip = {0, 0, 0, 0};
  UnicoCorners corners;
  double line_width;
  int radius;

  corners = unico_get_corners (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  if (frame->gap_x != -1)
    unico_get_frame_gap_clip (x, y, width, height,
                              &bevel_clip, &frame_clip, frame);

  cairo_save (cr);

  /* Set clip for the bevel */
  if (frame->gap_x != -1)
    {
      /* Set clip for gap */
      cairo_set_fill_rule  (cr, CAIRO_FILL_RULE_EVEN_ODD);
      cairo_rectangle      (cr, x, y, width, height);
      cairo_rectangle      (cr, bevel_clip.x, bevel_clip.y, bevel_clip.width, bevel_clip.height);
      cairo_clip           (cr);
    }

  /* Draw the bevel */
  if (frame->shadow == GTK_SHADOW_ETCHED_IN ||
      frame->shadow == GTK_SHADOW_ETCHED_OUT)
    {
      if (frame->shadow == GTK_SHADOW_ETCHED_IN)
        unico_cairo_draw_stroke_inner_rect (cr, engine,
                                                x+line_width, y+line_width,
                                                width-(line_width*2), height-(line_width*2),
                                                radius-line_width, corners);
      else
        unico_cairo_draw_stroke_inner_rect (cr, engine,
                                                x, y,
                                                width-(line_width*2), height-(line_width*2),
                                                radius-line_width, corners);
    }
  else if (frame->shadow != GTK_SHADOW_NONE)
    unico_cairo_draw_stroke_inner_rect (cr, engine,
                                            x+line_width, y+line_width,
                                            width-(line_width*2), height-(line_width*2),
                                            radius-line_width, corners);

  /* restore the previous clip region */
  cairo_restore    (cr);
  cairo_save       (cr);

  if (frame->gap_x != -1)
    {
      /* Set clip for gap */
      cairo_set_fill_rule  (cr, CAIRO_FILL_RULE_EVEN_ODD);
      cairo_rectangle      (cr, x, y, width, height);
      cairo_rectangle      (cr, frame_clip.x, frame_clip.y, frame_clip.width, frame_clip.height);
      cairo_clip           (cr);
    }

  /* Draw frame */
  if (frame->shadow == GTK_SHADOW_ETCHED_IN ||
      frame->shadow == GTK_SHADOW_ETCHED_OUT)
    {
      if (frame->shadow == GTK_SHADOW_ETCHED_OUT)
        unico_cairo_draw_border_rect (cr, engine,
                                          x+line_width, y+line_width,
                                          width-(line_width*2), height-(line_width*2),
                                          radius-line_width, corners);
      else
        unico_cairo_draw_border_rect (cr, engine,
                                          x, y,
                                          width-(line_width*2), height-(line_width*2),
                                          radius-line_width, corners);
    }
  else
    {
      unico_cairo_draw_border_rect (cr, engine, x, y, width, height, radius, corners);
    }

  cairo_restore (cr);
}

static void
unico_draw_notebook (cairo_t *cr,
                     GtkThemingEngine *engine,
                     int x,
                     int y,
                     int width,
                     int height,
                     UnicoFrameParameters *frame)
{
  unico_cairo_draw_background (cr, engine, x, y, width, height);
  unico_draw_frame (cr, engine, x, y, width, height, frame);
}

static void
unico_draw_slider_button_path (cairo_t *cr,
                               int x,
                               int y,
                               int width,
                               int height,
                               int radius)
{
  cairo_move_to (cr, x+radius, y);
  cairo_arc (cr, x+width-radius, y+radius, radius, G_PI*1.5, G_PI*2);
  cairo_line_to (cr, x+width, y+height-width/2.0);
  cairo_line_to (cr, x+width/2.0, y+height);
  cairo_line_to (cr, x, y+height-width/2.0);
  cairo_arc (cr, x+radius, y+radius, radius, G_PI, G_PI*1.5);
}

static void
unico_draw_slider_button (cairo_t *cr,
                          GtkThemingEngine *engine,
                          int x,
                          int y,
                          int width,
                          int height,
                          UnicoSliderParameters *slider)
{
  UnicoCorners corners;
  double line_width;
  int radius;

  corners = unico_get_corners (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);
  radius = MIN (radius, MIN (width/2.0, height/2.0));

  cairo_set_line_width (cr, line_width);

  if (!slider->horizontal) /* XXX: could be broken, need to test */
    unico_cairo_exchange_axis (cr, &x, &y, &width, &height);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  unico_draw_slider_button_path (cr, 0, 0, width, height, radius);
  unico_cairo_draw_background_from_path (cr, engine, 0, 0, width, height);

  unico_draw_slider_button_path (cr, line_width/2.0, line_width/2.0,
                                     width-line_width, height-line_width,
                                     radius);
  unico_cairo_draw_stroke_outer_from_path (cr, engine,
                                               line_width/2.0, line_width/2.0,
                                               width-line_width, height-line_width);

  unico_draw_slider_button_path (cr, line_width/2.0+line_width*2, line_width/2.0+line_width*2,
                                     width-line_width-line_width*4, height-line_width-line_width*4,
                                     radius);
  unico_cairo_draw_stroke_inner_from_path (cr, engine,
                                               line_width/2.0+line_width*2, line_width/2.0+line_width*2,
                                               width-line_width-line_width*4, height-line_width-line_width*4);

  unico_draw_slider_button_path (cr, line_width/2.0+line_width, line_width/2.0+line_width,
                                     width-line_width-line_width*2, height-line_width-line_width*2,
                                     radius);
  unico_cairo_draw_border_from_path (cr, engine,
                                         line_width/2.0+line_width, line_width/2.0+line_width,
                                         width-line_width-line_width*2, height-line_width-line_width*2);

  cairo_restore (cr);
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
  GtkStateFlags state;
  UnicoCorners corners;
  double line_width;
  int radius;

  state = gtk_theming_engine_get_state (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  cairo_save (cr);

  if (state & GTK_STATE_FLAG_ACTIVE)
    cairo_rectangle (cr, x, y, width, height);
  else
    cairo_rectangle (cr, x, y, width+1, height+1); /* XXX: remember to color pixels below */
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

  unico_cairo_draw_background_rect (cr, engine,
                                        x+1+line_width, y+1+line_width,
                                        width-1-2-(line_width*2), height-1-2-(line_width*2),
                                        radius-1, corners);

  unico_cairo_draw_stroke_inner_rect (cr, engine,
                                          x+line_width, y+line_width,
                                          width-1-line_width*2, height-1-line_width*2,
                                          radius-line_width, corners);

  unico_cairo_draw_border_rect (cr, engine,
                                    x, y,
                                    width-1, height-1,
                                    radius, corners);

  cairo_restore (cr);
}

void
unico_register_style_default (UnicoStyleFunctions *functions)
{
  g_assert (functions);

  functions->draw_button_background = unico_draw_button_background;
  functions->draw_button_frame      = unico_draw_button_frame;
  functions->draw_frame             = unico_draw_frame;
  functions->draw_notebook          = unico_draw_notebook;
  functions->draw_slider_button     = unico_draw_slider_button;
  functions->draw_tab               = unico_draw_tab;
}
