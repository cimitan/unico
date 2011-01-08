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

#define UNICO_RECT_SET(rect, _x, _y, _w, _h) (rect).x      = (_x); \
                                             (rect).y      = (_y); \
                                             (rect).width  = (_w); \
                                             (rect).height = (_h);

static void
unico_draw_button_background (DRAW_ARGS)
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

static void
unico_draw_button_frame (DRAW_ARGS)
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

static void
unico_draw_check (DRAW_ARGS)
{
  GtkStateFlags state;
  UnicoCorners corners;
  gboolean in_cell, in_menu;
  gboolean draw_bullet, inconsistent;
  gdouble line_width;
  gint radius;

  corners = unico_get_corners (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  state = gtk_theming_engine_get_state (engine);

  in_cell = gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_CELL);
  in_menu = gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENU);

  inconsistent = (state & GTK_STATE_FLAG_INCONSISTENT) != 0;
  draw_bullet = (state & GTK_STATE_FLAG_ACTIVE) != 0;
  draw_bullet |= inconsistent;

  if (!in_menu)
    {
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

      unico_cairo_draw_background_rect (engine, cr,
                                        x+line_width*2, y+line_width*2,
                                        width-line_width*4, height-line_width*4,
                                        radius, unico_get_corners (engine));
    }

  if (draw_bullet)
    {
      GdkRGBA *bullet_color;

      gtk_theming_engine_get (engine, state,
                              "-unico-bullet-color", &bullet_color,
                              NULL);

      if (inconsistent)
        {
          cairo_save (cr);

          cairo_set_line_width (cr, 2.0);
          cairo_move_to (cr, 3, (double)height/2);
          cairo_line_to (cr, width-3, (double)height/2);

          cairo_restore (cr);
        }
      else
        {
          cairo_translate (cr, x, y);

          if (in_menu)
            {
              cairo_scale (cr, (double)width/18.0, (double)height/18.0);
              cairo_translate (cr, 2.0, 3.0);
            }
          else
            {
              GdkRGBA *bullet_outline_color;

              gtk_theming_engine_get (engine, state,
                                      "-unico-bullet-outline-color", &bullet_outline_color,
                                      NULL);

              cairo_scale (cr, (double)width/18.0, (double)height/18.0);

              cairo_move_to (cr, 5.0, 5.65);
              cairo_line_to (cr, 8.95, 9.57);
              cairo_line_to (cr, 16.0, 2.54);
              cairo_line_to (cr, 16.0, 8.36);
              cairo_line_to (cr, 10.6, 15.1);
              cairo_line_to (cr, 7.6, 15.1);
              cairo_line_to (cr, 2.95, 10.48);
              cairo_line_to (cr, 2.95, 7.65);
              cairo_close_path (cr);

              gdk_cairo_set_source_rgba (cr, bullet_outline_color);
              cairo_fill (cr);

              cairo_translate (cr, 4.0, 2.0);

              gdk_rgba_free (bullet_outline_color);
            }

          cairo_move_to (cr, 0.0, 6.0);
          cairo_line_to (cr, 0.0, 8.0);
          cairo_line_to (cr, 4.0, 12.0);
          cairo_line_to (cr, 6.0, 12.0);
          cairo_line_to (cr, 15.0, 1.0);
          cairo_line_to (cr, 15.0, 0.0);
          cairo_line_to (cr, 14.0, 0.0);
          cairo_line_to (cr, 5.0, 9.0);
          cairo_line_to (cr, 1.0, 5.0);
          cairo_close_path (cr);
        }

      gdk_cairo_set_source_rgba (cr, bullet_color);
      cairo_fill (cr);

      gdk_rgba_free (bullet_color);
    }
}

static void
unico_draw_column_header_background (DRAW_ARGS)
{
  UnicoCorners corners = UNICO_CORNER_NONE;

  unico_cairo_draw_background_rect (engine, cr,
                                    x, y,
                                    width, height,
                                    0, corners);
}

static void
unico_draw_column_header_frame (DRAW_ARGS)
{
  UnicoCorners corners = UNICO_CORNER_NONE;
  gdouble line_width;

  unico_get_line_width (engine, &line_width);

  unico_cairo_draw_stroke_inner_rect (engine, cr,
                                      x, y,
                                      width-line_width, height-line_width,
                                      0, corners);

  unico_cairo_draw_border_rect (engine, cr,
                                x-line_width, y-line_width,
                                width, height,
                                0, corners);
}

static void
unico_get_frame_gap_clip (gint x,
                          gint y,
                          gint width,
                          gint height,
                          GdkRectangle *bevel,
                          GdkRectangle *border,
                          UnicoFrameParameters *frame)
{
  switch (frame->gap_side)
    {
      case GTK_POS_TOP:
        {
          UNICO_RECT_SET (*bevel, 2.0+frame->gap_x, 0.0,
                          frame->gap_width-3, 2.0);
          UNICO_RECT_SET (*border, 1.0+frame->gap_x, 0.0,
                          frame->gap_width-3, 2.0);
          break;
        }
      case GTK_POS_BOTTOM:
        {
          UNICO_RECT_SET (*bevel, 2.0+frame->gap_x, height-2.0,
                          frame->gap_width-3, 2.0);
          UNICO_RECT_SET (*border, 1.0+frame->gap_x, height-1.0,
                          frame->gap_width-3, 2.0);
          break;
        }
      case GTK_POS_LEFT:
        {
          UNICO_RECT_SET (*bevel, 0.0, 2.0+frame->gap_x,
                          2.0, frame->gap_width-3);
          UNICO_RECT_SET (*border, 0.0, 1.0+frame->gap_x,
                          1.0, frame->gap_width-3);
          break;
        }
      case GTK_POS_RIGHT:
        {
          UNICO_RECT_SET (*bevel, width-2.0, 2.0+frame->gap_x,
                          2.0, frame->gap_width-3);
          UNICO_RECT_SET (*border, width-1.0, 1.0+frame->gap_x,
                          1.0, frame->gap_width-3);
          break;
        }
    }
}

static void
unico_draw_frame (DRAW_ARGS,
                  UnicoFrameParameters *frame)
{
  GdkRectangle bevel_clip = {0, 0, 0, 0};
  GdkRectangle frame_clip = {0, 0, 0, 0};
  UnicoCorners corners;
  gdouble line_width;
  gint radius;

  corners = unico_get_corners (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  if (frame->gap_x != -1)
    unico_get_frame_gap_clip (x, y, width, height,
                              &bevel_clip, &frame_clip, frame);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  cairo_save (cr);

  /* Set clip for the bevel */
  if (frame->gap_x != -1)
    {
      /* Set clip for gap */
      cairo_set_fill_rule  (cr, CAIRO_FILL_RULE_EVEN_ODD);
      cairo_rectangle      (cr, 0, 0, width, height);
      cairo_rectangle      (cr, bevel_clip.x, bevel_clip.y, bevel_clip.width, bevel_clip.height);
      cairo_clip           (cr);
    }

  /* Draw the bevel */
  if (frame->shadow == GTK_SHADOW_ETCHED_IN ||
      frame->shadow == GTK_SHADOW_ETCHED_OUT)
    {
      if (frame->shadow == GTK_SHADOW_ETCHED_IN)
        unico_cairo_draw_stroke_inner_rect (engine, cr,
                                            line_width, line_width,
                                            width-line_width*2, height-line_width*2,
                                            radius-line_width, corners);
      else
        unico_cairo_draw_stroke_inner_rect (engine, cr,
                                            0, 0,
                                            width-line_width*2, height-line_width*2,
                                            radius-line_width, corners);
    }
  else if (frame->shadow != GTK_SHADOW_NONE)
    unico_cairo_draw_stroke_inner_rect (engine, cr,
                                        line_width, line_width,
                                        width-line_width*2, height-line_width*2,
                                        radius-line_width, corners);

  /* restore the previous clip region */
  cairo_restore    (cr);
  cairo_save       (cr);

  if (frame->gap_x != -1)
    {
      /* Set clip for gap */
      cairo_set_fill_rule  (cr, CAIRO_FILL_RULE_EVEN_ODD);
      cairo_rectangle      (cr, 0, 0, width, height);
      cairo_rectangle      (cr, frame_clip.x, frame_clip.y, frame_clip.width, frame_clip.height);
      cairo_clip           (cr);
    }

  /* Draw frame */
  if (frame->shadow == GTK_SHADOW_ETCHED_IN ||
      frame->shadow == GTK_SHADOW_ETCHED_OUT)
    {
      if (frame->shadow == GTK_SHADOW_ETCHED_OUT)
        unico_cairo_draw_border_rect (engine, cr,
                                      line_width, line_width,
                                      width-line_width*2, height-line_width*2,
                                      radius-line_width, corners);
      else
        unico_cairo_draw_border_rect (engine, cr,
                                      0, 0,
                                      width-line_width*2, height-line_width*2,
                                      radius-line_width, corners);
    }
  else
    {
      unico_cairo_draw_border_rect (engine, cr, 0, 0, width, height, radius, corners);
    }

  cairo_restore (cr);

  cairo_restore (cr);
}

static void
unico_draw_menubar_background (DRAW_ARGS)
{
  UnicoCorners corners = UNICO_CORNER_NONE;

  unico_cairo_draw_background_rect (engine, cr,
                                    x, y,
                                    width, height,
                                    0, corners);
}

static void
unico_draw_menubar_frame (DRAW_ARGS)
{
  UnicoCorners corners = UNICO_CORNER_NONE;
  gdouble line_width;

  unico_get_line_width (engine, &line_width);

  unico_cairo_draw_stroke_inner_rect (engine, cr,
                                      x+line_width, y+line_width,
                                      width-line_width*2, height-line_width*2,
                                      0, corners);

  unico_cairo_draw_border_rect (engine, cr,
                                x, y,
                                width, height,
                                0, corners);
}

static void
unico_draw_menubaritem_background (DRAW_ARGS)
{
  UnicoCorners corners;
  gint radius;

  corners = unico_get_corners (engine);
  unico_get_border_radius (engine, &radius);

  unico_cairo_draw_background_rect (engine, cr,
                                    x, y,
                                    width, height+1,
                                    radius, corners);
}

static void
unico_draw_menubaritem_frame (DRAW_ARGS)
{
  UnicoCorners corners;
  gdouble line_width;
  gint radius;

  corners = unico_get_corners (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  unico_cairo_draw_stroke_inner_rect (engine, cr,
                                      x+line_width, y+line_width,
                                      width-line_width*2, height+1,
                                      radius, corners);

  unico_cairo_draw_border_rect (engine, cr,
                                x, y,
                                width, height+1,
                                radius, corners);
}

static void
unico_draw_menuitem_background (DRAW_ARGS)
{
  UnicoCorners corners;
  gint radius;

  corners = unico_get_corners (engine);
  unico_get_border_radius (engine, &radius);

  unico_cairo_draw_background_rect (engine, cr,
                                    x, y,
                                    width, height,
                                    radius, corners);
}

static void
unico_draw_menuitem_frame (DRAW_ARGS)
{
  UnicoCorners corners;
  gdouble line_width;
  gint radius;

  corners = unico_get_corners (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  unico_cairo_draw_stroke_inner_rect (engine, cr,
                                      x+line_width, y+line_width,
                                      width-line_width*2, height-line_width*2,
                                      radius, corners);

  unico_cairo_draw_border_rect (engine, cr,
                                x, y,
                                width, height,
                                radius, corners);
}

static void
unico_draw_notebook (DRAW_ARGS,
                     UnicoFrameParameters *frame)
{
  unico_cairo_draw_background (engine, cr, x, y, width, height);
  unico_draw_frame (engine, cr, x, y, width, height, frame);
}

static void
unico_draw_progressbar_trough_background (DRAW_ARGS)
{
  gdouble line_width;
  gint radius;

  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  unico_cairo_draw_background_rect (engine, cr,
                                    x+line_width, y+line_width,
                                    width-line_width*2, height-line_width*2,
                                    radius, unico_get_corners (engine));
}

static void
unico_draw_progressbar_trough_frame (DRAW_ARGS)
{
  gint radius;

  unico_get_border_radius (engine, &radius);

  unico_cairo_draw_border_rect (engine, cr,
                                x, y,
                                width, height,
                                radius, unico_get_corners (engine));
}

static void
unico_draw_radio (DRAW_ARGS)
{
  GtkStateFlags state;
  UnicoCorners corners;
  gboolean in_cell, in_menu;
  gboolean draw_bullet, inconsistent;
  gdouble line_width;
  gint radius;

  corners = unico_get_corners (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  state = gtk_theming_engine_get_state (engine);

  in_cell = gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_CELL);
  in_menu = gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENU);

  inconsistent = (state & GTK_STATE_FLAG_INCONSISTENT) != 0;
  draw_bullet = (state & GTK_STATE_FLAG_ACTIVE) != 0;
  draw_bullet |= inconsistent;

  if (!in_menu)
    {
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

      unico_cairo_draw_background_rect (engine, cr,
                                        x+line_width*2, y+line_width*2,
                                        width-line_width*4, height-line_width*4,
                                        radius, unico_get_corners (engine));
    }

  if (draw_bullet)
    {
      GdkRGBA *bullet_color;

      gtk_theming_engine_get (engine, state,
                              "-unico-bullet-color", &bullet_color,
                              NULL);

      if (inconsistent)
        {
          cairo_save (cr);

          cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
          cairo_set_line_width (cr, 2.0);

          cairo_move_to(cr, 5, (double)height/2);
          cairo_line_to(cr, width-5, (double)height/2);

          gdk_cairo_set_source_rgba (cr, bullet_color);
          cairo_stroke (cr);

          cairo_restore (cr);
        }
      else
        {
          if (in_menu)
            cairo_arc (cr, x+(double)width/2, y+(double)height/2, (double)(width+height)/4-4, 0, G_PI*2);
          else
            {
              GdkRGBA *bullet_outline_color;

              gtk_theming_engine_get (engine, state,
                                      "-unico-bullet-outline-color", &bullet_outline_color,
                                      NULL);

              cairo_arc (cr, x+(double)width/2, y+(double)height/2, (double)(width+height)/4-4, 0, G_PI*2);
              gdk_cairo_set_source_rgba (cr, bullet_outline_color);
              cairo_fill (cr);

              cairo_arc (cr, x+(double)width/2, y+(double)height/2, (double)(width+height)/4-5, 0, G_PI*2);

              gdk_rgba_free (bullet_outline_color);
            }

          gdk_cairo_set_source_rgba (cr, bullet_color);
          cairo_fill (cr);
        }

      gdk_rgba_free (bullet_color);
    }
}

static void
unico_draw_scrollbar_stepper_background (DRAW_ARGS)
{
  gdouble line_width;
  gint radius;

  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  unico_cairo_draw_background_rect (engine, cr,
                                    x+line_width, y+line_width,
                                    width-line_width*2, height-line_width*2,
                                    radius, unico_get_corners (engine));
}

static void
unico_draw_scrollbar_stepper_frame (DRAW_ARGS)
{
  UnicoCorners corners;
  gdouble line_width;
  gint radius;

  corners = unico_get_corners (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  unico_cairo_draw_stroke_inner_rect (engine, cr,
                                      x+line_width, y+line_width,
                                      width-line_width*2, height-line_width*2,
                                      radius, corners);

  unico_cairo_draw_border_rect (engine, cr,
                                x, y,
                                width, height,
                                radius, corners);
}

static void
unico_draw_scrollbar_slider (DRAW_ARGS)
{
  UnicoCorners corners;
  gdouble line_width;
  gint radius;

  corners = unico_get_corners (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  unico_cairo_draw_background_rect (engine, cr,
                                    x+line_width, y+line_width,
                                    width-line_width*2, height-line_width*2,
                                    radius, corners);

  unico_cairo_draw_stroke_inner_rect (engine, cr,
                                      x+line_width, y+line_width,
                                      width-line_width*2, height-line_width*2,
                                      radius, corners);

  unico_cairo_draw_border_rect (engine, cr,
                                x, y,
                                width, height,
                                radius, corners);
}

static void
unico_draw_scrollbar_trough_background (DRAW_ARGS)
{
  gdouble line_width;
  gint hoffset = 0;
  gint voffset = 0;
  gint radius;

  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  if (width>height)
    hoffset = 1;
  else
    voffset = 1;

  unico_cairo_draw_background_rect (engine, cr,
                                    x+line_width+hoffset, y+line_width+voffset,
                                    width-line_width*2-hoffset*2, height-line_width*2-voffset*2,
                                    radius, unico_get_corners (engine));
}

static void
unico_draw_scrollbar_trough_frame (DRAW_ARGS)
{
  gint hoffset = 0;
  gint voffset = 0;
  gint radius;

  unico_get_border_radius (engine, &radius);

  if (width>height)
    hoffset = 1;
  else
    voffset = 1;

  unico_cairo_draw_border_rect (engine, cr,
                                x+hoffset, y+voffset,
                                width-hoffset*2, height-voffset*2,
                                radius, unico_get_corners (engine));
}

static void
unico_draw_slider_button_path (cairo_t *cr,
                               gint x,
                               gint y,
                               gint width,
                               gint height,
                               gint radius)
{
  cairo_move_to (cr, x+radius, y);
  cairo_arc (cr, x+width-radius, y+radius, radius, G_PI*1.5, G_PI*2);
  cairo_line_to (cr, x+width, y+height-width/2.0);
  cairo_line_to (cr, x+width/2.0, y+height);
  cairo_line_to (cr, x, y+height-width/2.0);
  cairo_arc (cr, x+radius, y+radius, radius, G_PI, G_PI*1.5);
}

static void
unico_draw_slider_button (DRAW_ARGS,
                          UnicoSliderParameters *slider)
{
  UnicoCorners corners;
  gdouble line_width;
  gint radius;

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
  unico_cairo_draw_background_from_path (engine, cr, 0, 0, width, height);

  unico_draw_slider_button_path (cr, line_width/2.0, line_width/2.0,
                                     width-line_width, height-line_width,
                                     radius);
  unico_cairo_draw_stroke_outer_from_path (engine, cr,
                                           line_width/2.0, line_width/2.0,
                                           width-line_width, height-line_width);

  unico_draw_slider_button_path (cr, line_width/2.0+line_width*2, line_width/2.0+line_width*2,
                                     width-line_width-line_width*4, height-line_width-line_width*4,
                                     radius);
  unico_cairo_draw_stroke_inner_from_path (engine, cr,
                                           line_width/2.0+line_width*2, line_width/2.0+line_width*2,
                                           width-line_width-line_width*4, height-line_width-line_width*4);

  unico_draw_slider_button_path (cr, line_width/2.0+line_width, line_width/2.0+line_width,
                                     width-line_width-line_width*2, height-line_width-line_width*2,
                                     radius);
  unico_cairo_draw_border_from_path (engine, cr,
                                     line_width/2.0+line_width, line_width/2.0+line_width,
                                     width-line_width-line_width*2, height-line_width-line_width*2);

  cairo_restore (cr);
}

static void
unico_draw_tab (DRAW_ARGS,
                UnicoTabParameters *tab)
{
  GtkStateFlags state;
  UnicoCorners corners;
  gdouble line_width;
  gint radius;

  state = gtk_theming_engine_get_state (engine);
  unico_get_line_width (engine, &line_width);
  unico_get_border_radius (engine, &radius);

  cairo_save (cr);

  cairo_rectangle (cr, x, y, width, height);
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

  unico_cairo_draw_background_rect (engine, cr,
                                    x+line_width, y+line_width,
                                    width-1-line_width*2, height-1-line_width*2,
                                    radius, corners);

  unico_cairo_draw_stroke_inner_rect (engine, cr,
                                      x+line_width, y+line_width,
                                      width-1-line_width*2, height-1-line_width*2,
                                      radius-line_width, corners);

  unico_cairo_draw_border_rect (engine, cr,
                                x, y,
                                width-1, height-1,
                                radius, corners);

  cairo_restore (cr);
}

static void
unico_draw_toolbar_background (DRAW_ARGS)
{
  UnicoCorners corners = UNICO_CORNER_NONE;
  gdouble line_width;

  unico_get_line_width (engine, &line_width);

  unico_cairo_draw_background_rect (engine, cr,
                                    x, y,
                                    width, height,
                                    0, corners);
}

static void
unico_draw_toolbar_frame (DRAW_ARGS)
{
  UnicoCorners corners = UNICO_CORNER_NONE;
  gdouble line_width;

  unico_get_line_width (engine, &line_width);

  unico_cairo_draw_stroke_inner_rect (engine, cr,
                                      x+line_width, y+line_width,
                                      width-line_width*2, height-line_width*2,
                                      0, corners);

  unico_cairo_draw_border_rect (engine, cr,
                                x, y,
                                width, height,
                                0, corners);
}

void
unico_register_style_default (UnicoStyleFunctions *functions)
{
  g_assert (functions);

  functions->draw_button_background             = unico_draw_button_background;
  functions->draw_button_frame                  = unico_draw_button_frame;
  functions->draw_check                         = unico_draw_check;
  functions->draw_column_header_background      = unico_draw_column_header_background;
  functions->draw_column_header_frame           = unico_draw_column_header_frame;
  functions->draw_frame                         = unico_draw_frame;
  functions->draw_menubar_background            = unico_draw_menubar_background;
  functions->draw_menubar_frame                 = unico_draw_menubar_frame;
  functions->draw_menubaritem_background        = unico_draw_menubaritem_background;
  functions->draw_menubaritem_frame             = unico_draw_menubaritem_frame;
  functions->draw_menuitem_background           = unico_draw_menuitem_background;
  functions->draw_menuitem_frame                = unico_draw_menuitem_frame;
  functions->draw_notebook                      = unico_draw_notebook;
  functions->draw_progressbar_trough_background = unico_draw_progressbar_trough_background;
  functions->draw_progressbar_trough_frame      = unico_draw_progressbar_trough_frame;
  functions->draw_radio                         = unico_draw_radio;
  functions->draw_scrollbar_slider              = unico_draw_scrollbar_slider;
  functions->draw_scrollbar_stepper_background  = unico_draw_scrollbar_stepper_background;
  functions->draw_scrollbar_stepper_frame       = unico_draw_scrollbar_stepper_frame;
  functions->draw_scrollbar_trough_background   = unico_draw_scrollbar_trough_background;
  functions->draw_scrollbar_trough_frame        = unico_draw_scrollbar_trough_frame;
  functions->draw_slider_button                 = unico_draw_slider_button;
  functions->draw_tab                           = unico_draw_tab;
  functions->draw_toolbar_background            = unico_draw_toolbar_background;
  functions->draw_toolbar_frame                 = unico_draw_toolbar_frame;
}
