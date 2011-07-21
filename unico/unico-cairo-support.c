/* The Unico Theming Engine for Gtk+.
 * Copyright (C) 2011 Canonical Ltd
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
 *
 * Authored by Andrea Cimitan <andrea.cimitan@canonical.com>
 *
 */

#include <cairo.h>

#include "gtkroundedboxprivate.h"
#include "unico-cairo-support.h"
#include "unico-support.h"
#include "unico-types.h"
#include "raico-blur.h"

/* set the border sides to 0 using hidden_side */
static void
hide_border_sides (GtkBorder *border,
                   guint      hidden_side)
{
  if (hidden_side & SIDE_TOP)
    border->top = 0;
  if (hidden_side & SIDE_RIGHT)
    border->right = 0;
  if (hidden_side & SIDE_BOTTOM)
    border->bottom = 0;
  if (hidden_side & SIDE_LEFT)
    border->left = 0;
}

/* shrink coordinate using GtkBorder */
static void
shrink_with_border (GtkBorder border,
                    gint     *x,
                    gint     *y,
                    gint     *width,
                    gint     *height)
{
  *x += border.left;
  *y += border.top;
  *width -= border.left + border.right;
  *height -= border.top + border.bottom;
}

/* draw the background */
static void
draw_background (GtkThemingEngine *engine,
                 cairo_t          *cr,
                 gdouble           x,
                 gdouble           y,
                 gdouble           width,
                 gdouble           height,
                 guint             hidden_side,
                 GtkJunctionSides  junction)
{
  GdkRGBA bg_color;
  GtkBorder border;
  GtkRoundedBox border_box;
  GtkStateFlags state;
  cairo_pattern_t *bg_pat;
  gboolean running;
  gdouble progress;

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get_border (engine, state, &border);
  hide_border_sides (&border, hidden_side);

  gtk_theming_engine_get_background_color (engine, state, &bg_color);
  gtk_theming_engine_get (engine, state,
                          "background-image", &bg_pat,
                          NULL);

  running = gtk_theming_engine_state_is_running (engine, GTK_STATE_PRELIGHT, &progress);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  if (gtk_theming_engine_has_class (engine, "background"))
    {
      cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0); /* transparent */
      cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
      cairo_paint (cr);
    }

  if (running)
    {
      cairo_pattern_t *other_pat;
      GtkStateFlags other_state;
      GdkRGBA other_bg;
      cairo_pattern_t *new_pat = NULL;

      if (state & GTK_STATE_FLAG_PRELIGHT)
        {
          other_state = state & ~(GTK_STATE_FLAG_PRELIGHT);

          /* useful math function to use for a pulse loop animation could be
           * progress = 1 - MAX (1 - fabs ((progress - 0.5)*2), 0);
           * but we need to handle the not-running case (once animation finished),
           * otherwise the last frame will be the PRELIGHT at full opacity. */
          progress = 1 - progress;
        }
      else
        other_state = state | GTK_STATE_FLAG_PRELIGHT;

      gtk_theming_engine_get_background_color (engine, other_state, &other_bg);
      gtk_theming_engine_get (engine, other_state,
                              "background-image", &other_pat,
                              NULL);

      if (bg_pat && other_pat)
        {
          cairo_pattern_type_t type, other_type;
          gint n0, n1;

          cairo_pattern_get_color_stop_count (bg_pat, &n0);
          cairo_pattern_get_color_stop_count (other_pat, &n1);
          type = cairo_pattern_get_type (bg_pat);
          other_type = cairo_pattern_get_type (other_pat);

          if (type == other_type && n0 == n1)
            {
              gdouble offset0, red0, green0, blue0, alpha0;
              gdouble offset1, red1, green1, blue1, alpha1;
              gdouble x00, x01, y00, y01, x10, x11, y10, y11;
              gdouble r00, r01, r10, r11;
              gint i;

              if (type == CAIRO_PATTERN_TYPE_LINEAR)
                {
                  cairo_pattern_get_linear_points (bg_pat, &x00, &y00, &x01, &y01);
                  cairo_pattern_get_linear_points (other_pat, &x10, &y10, &x11, &y11);

                  new_pat = cairo_pattern_create_linear (x00 + (x10 - x00) * progress,
                                                         y00 + (y10 - y00) * progress,
                                                         x01 + (x11 - x01) * progress,
                                                         y01 + (y11 - y01) * progress);
                }
              else
                {
                  cairo_pattern_get_radial_circles (bg_pat, &x00, &y00, &r00, &x01, &y01, &r01);
                  cairo_pattern_get_radial_circles (other_pat, &x10, &y10, &r10, &x11, &y11, &r11);

                  new_pat = cairo_pattern_create_radial (x00 + (x10 - x00) * progress,
                                                         y00 + (y10 - y00) * progress,
                                                         r00 + (r10 - r00) * progress,
                                                         x01 + (x11 - x01) * progress,
                                                         y01 + (y11 - y01) * progress,
                                                         r01 + (r11 - r01) * progress);
                }

              cairo_pattern_set_filter (new_pat, CAIRO_FILTER_FAST);
              i = 0;

              /* Blend both gradients into one */
              while (i < n0 && i < n1)
                {
                  cairo_pattern_get_color_stop_rgba (bg_pat, i,
                                                     &offset0,
                                                     &red0, &green0, &blue0,
                                                     &alpha0);
                  cairo_pattern_get_color_stop_rgba (other_pat, i,
                                                     &offset1,
                                                     &red1, &green1, &blue1,
                                                     &alpha1);
                  cairo_pattern_add_color_stop_rgba (new_pat,
                                                     offset0 + ((offset1 - offset0) * progress),
                                                     red0 + ((red1 - red0) * progress),
                                                     green0 + ((green1 - green0) * progress),
                                                     blue0 + ((blue1 - blue0) * progress),
                                                     alpha0 + ((alpha1 - alpha0) * progress));
                  i++;
                }
            }
          else
            {
              cairo_save (cr);

              cairo_rectangle (cr, 0, 0, width, height);
              cairo_clip (cr);

              cairo_push_group (cr);

              cairo_scale (cr, width, height);
              cairo_set_source (cr, other_pat);
              cairo_paint_with_alpha (cr, progress);
              cairo_set_source (cr, bg_pat);
              cairo_paint_with_alpha (cr, 1.0 - progress);

              new_pat = cairo_pop_group (cr);

              cairo_restore (cr);
            }
        }
      else if (bg_pat || other_pat)
        {
          const GdkRGBA *c;
          cairo_pattern_t *p;
          gdouble x0, y0, x1, y1, r0, r1;
          gint n, i;

          /* Blend a pattern with a color */
          if (bg_pat)
            {
              p = bg_pat;
              c = &other_bg;
              progress = 1 - progress;
            }
          else
            {
              p = other_pat;
              c = &bg_color;
            }

          if (cairo_pattern_get_type (p) == CAIRO_PATTERN_TYPE_LINEAR)
            {
              cairo_pattern_get_linear_points (p, &x0, &y0, &x1, &y1);
              new_pat = cairo_pattern_create_linear (x0, y0, x1, y1);
            }
          else
            {
              cairo_pattern_get_radial_circles (p, &x0, &y0, &r0, &x1, &y1, &r1);
              new_pat = cairo_pattern_create_radial (x0, y0, r0, x1, y1, r1);
            }

          cairo_pattern_get_color_stop_count (p, &n);

          for (i = 0; i < n; i++)
            {
              gdouble red1, green1, blue1, alpha1;
              gdouble offset;

              cairo_pattern_get_color_stop_rgba (p, i,
                                                 &offset,
                                                 &red1, &green1, &blue1,
                                                 &alpha1);
              cairo_pattern_add_color_stop_rgba (new_pat, offset,
                                                 c->red + ((red1 - c->red) * progress),
                                                 c->green + ((green1 - c->green) * progress),
                                                 c->blue + ((blue1 - c->blue) * progress),
                                                 c->alpha + ((alpha1 - c->alpha) * progress));
            }
        }
      else
        {
          /* Merge just colors */
          new_pat = cairo_pattern_create_rgba (CLAMP (bg_color.red + ((other_bg.red - bg_color.red) * progress), 0, 1),
                                               CLAMP (bg_color.green + ((other_bg.green - bg_color.green) * progress), 0, 1),
                                               CLAMP (bg_color.blue + ((other_bg.blue - bg_color.blue) * progress), 0, 1),
                                               CLAMP (bg_color.alpha + ((other_bg.alpha - bg_color.alpha) * progress), 0, 1));
        }

      if (new_pat)
        {
          /* Replace pattern to use */
          cairo_pattern_destroy (bg_pat);
          bg_pat = new_pat;
        }

      if (other_pat)
        cairo_pattern_destroy (other_pat);
    }

  _gtk_rounded_box_init_rect (&border_box, 0, 0, width, height);
  _gtk_rounded_box_apply_border_radius (&border_box, engine, state, junction);
  _gtk_rounded_box_shrink (&border_box, border.top, border.right, border.bottom, border.left);
  _gtk_rounded_box_path (&border_box, cr);

  if (bg_pat)
    {
      unico_cairo_style_pattern_set_matrix (bg_pat, width, height);
      cairo_set_source (cr, bg_pat);
    }
  else
    gdk_cairo_set_source_rgba (cr, &bg_color);

  cairo_fill (cr);

  if (bg_pat)
    cairo_pattern_destroy (bg_pat);

  cairo_restore (cr);
}

/* draw the inner glow */
static void
draw_glow (GtkThemingEngine *engine,
           cairo_t          *cr,
           gint              x,
           gint              y,
           gint              width,
           gint              height,
           guint             hidden_side,
           GtkJunctionSides  junction)
{
  GdkRGBA *glow_color;
  GtkBorder border;
  GtkRoundedBox border_box, padding_box, clip_box;
  GtkStateFlags state;
  cairo_t *cr_surface;
  cairo_surface_t *surface;
  raico_blur_t* blur = NULL;
  gint bradius = 0;

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get_border (engine, state, &border);
  hide_border_sides (&border, hidden_side);

  gtk_theming_engine_get (engine, state,
                          "-unico-glow-radius", &bradius,
                          "-unico-glow-color", &glow_color,
                          NULL);

  if (bradius <= 0)
    {
      gdk_rgba_free (glow_color);
      return;
    }

  /* clip the area */
  cairo_save (cr);

  cairo_translate (cr, x, y);

  _gtk_rounded_box_init_rect (&border_box, 0, 0, width, height);
  _gtk_rounded_box_apply_border_radius (&border_box, engine, state, junction);
  _gtk_rounded_box_shrink (&border_box, border.top, border.right, border.bottom, border.left);
  clip_box = border_box;
  _gtk_rounded_box_path (&clip_box, cr);

  cairo_clip (cr);

  /* create the surface to blur */
  surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                        width + bradius * 2,
                                        height + bradius * 2);
  cr_surface = cairo_create (surface); 
  cairo_set_fill_rule (cr_surface, CAIRO_FILL_RULE_EVEN_ODD);

  /* draw a rounded rectangle on the surface to blur */
  _gtk_rounded_box_move (&border_box, bradius, bradius);
  padding_box = border_box;
  _gtk_rounded_box_shrink (&padding_box, border.top * 2, border.right * 2, border.bottom * 2, border.left * 2);
  gdk_cairo_set_source_rgba (cr_surface, glow_color);
  _gtk_rounded_box_path (&border_box, cr_surface);
  _gtk_rounded_box_path (&padding_box, cr_surface);
  cairo_fill (cr_surface);

  /* create and apply raico blur */
  blur = raico_blur_create ();
  raico_blur_set_radius (blur, bradius);
  raico_blur_apply (blur, surface);

  /* paint the blurred surface to cr */
  cairo_set_source_surface (cr, surface, - bradius, - bradius); 
  cairo_paint (cr);

  cairo_restore (cr);

  cairo_surface_destroy (surface); 
  cairo_destroy (cr_surface);

  gdk_rgba_free (glow_color);
}

void
unico_cairo_draw_background (GtkThemingEngine *engine,
                             cairo_t          *cr,
                             gint              x,
                             gint              y,
                             gint              width,
                             gint              height,
                             guint             hidden_side,
                             GtkJunctionSides  junction)
{
  GtkBorder border;
  GtkStateFlags state;
  gint radius;

  unico_get_border_radius (engine, &radius);
  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get_border (engine, state, &border);
  hide_border_sides (&border, hidden_side);

  if (unico_has_outer_stroke (engine))
    shrink_with_border (border, &x, &y, &width, &height);

  draw_background (engine, cr,
                   x, y,
                   width, height,
                   hidden_side, junction);

  draw_glow (engine, cr,
             x, y,
             width, height,
             hidden_side, junction);
}

/* draw the border */
static void
draw_border (GtkThemingEngine *engine,
             cairo_t          *cr,
             gdouble           x,
             gdouble           y,
             gdouble           width,
             gdouble           height,
             guint             hidden_side,
             GtkJunctionSides  junction)
{
  GtkStateFlags state;
  GtkBorderStyle border_style;
  GtkRoundedBox border_box, padding_box;
  cairo_pattern_t *border_pat;
  gdouble progress;
  gboolean running;
  GtkBorder border;
  static const guint current_side[4] = { SIDE_TOP, SIDE_RIGHT, SIDE_BOTTOM, SIDE_LEFT };
  GdkRGBA *colors[4];
  guint i, j;

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get_border (engine, state, &border);
  hide_border_sides (&border, hidden_side);

  gtk_theming_engine_get (engine, state,
                          "border-style", &border_style,
                          "border-top-color", &colors[0],
                          "border-right-color", &colors[1],
                          "border-bottom-color", &colors[2],
                          "border-left-color", &colors[3],
                          "-unico-border-gradient", &border_pat,
                          NULL);

  running = gtk_theming_engine_state_is_running (engine, GTK_STATE_PRELIGHT, &progress);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  if (running)
    {
      cairo_pattern_t *other_pat;
      GtkStateFlags other_state;
      cairo_pattern_t *new_pat = NULL;

      if (state & GTK_STATE_FLAG_PRELIGHT)
        {
          other_state = state & ~(GTK_STATE_FLAG_PRELIGHT);
          progress = 1 - progress;
        }
      else
        other_state = state | GTK_STATE_FLAG_PRELIGHT;

      gtk_theming_engine_get (engine, other_state,
                              "-unico-border-gradient", &other_pat,
                              NULL);

      if (border_pat && other_pat)
        {
          cairo_pattern_type_t type, other_type;
          gint n0, n1;

          cairo_pattern_get_color_stop_count (border_pat, &n0);
          cairo_pattern_get_color_stop_count (other_pat, &n1);
          type = cairo_pattern_get_type (border_pat);
          other_type = cairo_pattern_get_type (other_pat);

          if (type == other_type && n0 == n1)
            {
              gdouble offset0, red0, green0, blue0, alpha0;
              gdouble offset1, red1, green1, blue1, alpha1;
              gdouble x00, x01, y00, y01, x10, x11, y10, y11;
              gdouble r00, r01, r10, r11;
              gint i;

              if (type == CAIRO_PATTERN_TYPE_LINEAR)
                {
                  cairo_pattern_get_linear_points (border_pat, &x00, &y00, &x01, &y01);
                  cairo_pattern_get_linear_points (other_pat, &x10, &y10, &x11, &y11);

                  new_pat = cairo_pattern_create_linear (x00 + (x10 - x00) * progress,
                                                         y00 + (y10 - y00) * progress,
                                                         x01 + (x11 - x01) * progress,
                                                         y01 + (y11 - y01) * progress);
                }
              else
                {
                  cairo_pattern_get_radial_circles (border_pat, &x00, &y00, &r00, &x01, &y01, &r01);
                  cairo_pattern_get_radial_circles (other_pat, &x10, &y10, &r10, &x11, &y11, &r11);

                  new_pat = cairo_pattern_create_radial (x00 + (x10 - x00) * progress,
                                                         y00 + (y10 - y00) * progress,
                                                         r00 + (r10 - r00) * progress,
                                                         x01 + (x11 - x01) * progress,
                                                         y01 + (y11 - y01) * progress,
                                                         r01 + (r11 - r01) * progress);
                }

              cairo_pattern_set_filter (new_pat, CAIRO_FILTER_FAST);
              i = 0;

              /* Blend both gradients into one */
              while (i < n0 && i < n1)
                {
                  cairo_pattern_get_color_stop_rgba (border_pat, i,
                                                     &offset0,
                                                     &red0, &green0, &blue0,
                                                     &alpha0);
                  cairo_pattern_get_color_stop_rgba (other_pat, i,
                                                     &offset1,
                                                     &red1, &green1, &blue1,
                                                     &alpha1);
                  cairo_pattern_add_color_stop_rgba (new_pat,
                                                     offset0 + ((offset1 - offset0) * progress),
                                                     red0 + ((red1 - red0) * progress),
                                                     green0 + ((green1 - green0) * progress),
                                                     blue0 + ((blue1 - blue0) * progress),
                                                     alpha0 + ((alpha1 - alpha0) * progress));
                  i++;
                }
            }
          else
            {
              cairo_save (cr);

              cairo_rectangle (cr, 0, 0, width, height);
              cairo_clip (cr);

              cairo_push_group (cr);

              cairo_scale (cr, width, height);
              cairo_set_source (cr, other_pat);
              cairo_paint_with_alpha (cr, progress);
              cairo_set_source (cr, border_pat);
              cairo_paint_with_alpha (cr, 1.0 - progress);

              new_pat = cairo_pop_group (cr);

              cairo_restore (cr);
            }
        }
      else if (border_pat || other_pat)
        {
          const GdkRGBA *c;
          cairo_pattern_t *p;
          gdouble x0, y0, x1, y1, r0, r1;
          gint n, i;

          /* Blend a pattern with a color */
          if (border_pat)
            {
              GdkRGBA other_color;

              gtk_theming_engine_get_border_color (engine, other_state, &other_color);

              p = border_pat;
              c = &other_color;
              progress = 1 - progress;
            }
          else
            {
              GdkRGBA border_color;

              gtk_theming_engine_get_border_color (engine, state, &border_color);

              p = other_pat;
              c = &border_color;
            }

          if (cairo_pattern_get_type (p) == CAIRO_PATTERN_TYPE_LINEAR)
            {
              cairo_pattern_get_linear_points (p, &x0, &y0, &x1, &y1);
              new_pat = cairo_pattern_create_linear (x0, y0, x1, y1);
            }
          else
            {
              cairo_pattern_get_radial_circles (p, &x0, &y0, &r0, &x1, &y1, &r1);
              new_pat = cairo_pattern_create_radial (x0, y0, r0, x1, y1, r1);
            }

          cairo_pattern_get_color_stop_count (p, &n);

          for (i = 0; i < n; i++)
            {
              gdouble red1, green1, blue1, alpha1;
              gdouble offset;

              cairo_pattern_get_color_stop_rgba (p, i,
                                                 &offset,
                                                 &red1, &green1, &blue1,
                                                 &alpha1);
              cairo_pattern_add_color_stop_rgba (new_pat, offset,
                                                 c->red + ((red1 - c->red) * progress),
                                                 c->green + ((green1 - c->green) * progress),
                                                 c->blue + ((blue1 - c->blue) * progress),
                                                 c->alpha + ((alpha1 - c->alpha) * progress));
            }
        }
      else
        {
          GdkRGBA *other_colors[4];

          gtk_theming_engine_get (engine, other_state,
                                  "border-top-color", &other_colors[0],
                                  "border-right-color", &other_colors[1],
                                  "border-bottom-color", &other_colors[2],
                                  "border-left-color", &other_colors[3],
                                  NULL);

          for (i = 0; i < 4; i++)
            {
              colors[i]->red = CLAMP (colors[i]->red + ((other_colors[i]->red - colors[i]->red) * progress), 0, 1);
              colors[i]->green = CLAMP (colors[i]->green + ((other_colors[i]->green - colors[i]->green) * progress), 0, 1);
              colors[i]->blue = CLAMP (colors[i]->blue + ((other_colors[i]->blue - colors[i]->blue) * progress), 0, 1);
              colors[i]->alpha = CLAMP (colors[i]->alpha + ((other_colors[i]->alpha - colors[i]->alpha) * progress), 0, 1);
              gdk_rgba_free (other_colors[i]);
            }
        }

      if (new_pat)
        {
          /* Replace pattern to use */
          cairo_pattern_destroy (border_pat);
          border_pat = new_pat;
        }

      if (other_pat)
        cairo_pattern_destroy (other_pat);
    }

  switch (border_style)
  {
    default:
      g_assert_not_reached ();
    case GTK_BORDER_STYLE_NONE:
    case GTK_BORDER_STYLE_SOLID:
      break;
    case GTK_BORDER_STYLE_INSET:
      /* FIXME static shading values */
      //color_shade (colors[1], 1.8, colors[1]);
      //color_shade (colors[2], 1.8, colors[2]);
      break;
    case GTK_BORDER_STYLE_OUTSET:
      /* FIXME static shading values */
      //color_shade (colors[0], 1.8, colors[0]);
      //color_shade (colors[3], 1.8, colors[3]);
      break;
  }

  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);

  _gtk_rounded_box_init_rect (&border_box, 0, 0, width, height);
  _gtk_rounded_box_apply_border_radius (&border_box, engine, state, junction);
  padding_box = border_box;
  _gtk_rounded_box_shrink (&padding_box, border.top, border.right, border.bottom, border.left);

  switch (border_style)
  {
    default:
      g_assert_not_reached ();
    case GTK_BORDER_STYLE_NONE:
      break;
    case GTK_BORDER_STYLE_SOLID:
    case GTK_BORDER_STYLE_INSET:
    case GTK_BORDER_STYLE_OUTSET:

      if (border_pat)
        {
          /* FIXME deal with line_width? */
          unico_cairo_style_pattern_set_matrix (border_pat, width, height);
          cairo_set_source (cr, border_pat);

          _gtk_rounded_box_path (&border_box, cr);
          _gtk_rounded_box_path (&padding_box, cr);
          cairo_fill (cr);
        }
      else if (gdk_rgba_equal (colors[0], colors[1]) &&
               gdk_rgba_equal (colors[0], colors[2]) &&
               gdk_rgba_equal (colors[0], colors[3]))
        {
          gdk_cairo_set_source_rgba (cr, colors[0]);

          _gtk_rounded_box_path (&border_box, cr);
          _gtk_rounded_box_path (&padding_box, cr);
          cairo_fill (cr);
        }
      else
        {
          for (i = 0; i < 4; i++) 
            {
              if (hidden_side & current_side[i])
                continue;

              for (j = 0; j < 4; j++)
                { 
                  if (hidden_side & current_side[j])
                    continue;

                  if (i == j || 
                      gdk_rgba_equal (colors[i], colors[j]))
                    {
                      /* We were already painted when i == j */
                      if (i > j)
                        break;

                      if (j == 0)
                        _gtk_rounded_box_path_top (&border_box, &padding_box, cr);
                      else if (j == 1)
                        _gtk_rounded_box_path_right (&border_box, &padding_box, cr);
                      else if (j == 2)
                        _gtk_rounded_box_path_bottom (&border_box, &padding_box, cr);
                      else if (j == 3)
                        _gtk_rounded_box_path_left (&border_box, &padding_box, cr);
                    }
                }
              /* We were already painted when i == j */
              if (i > j)
                continue;

              gdk_cairo_set_source_rgba (cr, colors[i]);

              cairo_fill (cr);
            }
        }
      break;
  }

  cairo_restore (cr);

  if (border_pat)
    cairo_pattern_destroy (border_pat);

  for (i = 0; i < 4; i++)
    gdk_rgba_free (colors[i]);
}

/* draw the inner stroke */
static void
draw_inner_stroke (GtkThemingEngine *engine,
                   cairo_t          *cr,
                   gdouble           x,
                   gdouble           y,
                   gdouble           width,
                   gdouble           height,
                   guint             hidden_side,
                   GtkJunctionSides  junction)
{
  GdkRGBA *inner_stroke_color;
  GtkBorder border;
  GtkRoundedBox border_box, padding_box;
  GtkStateFlags state; 
  cairo_pattern_t *inner_stroke_pat;

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get_border (engine, state, &border);
  hide_border_sides (&border, hidden_side);

  gtk_theming_engine_get (engine, state,
                          "-unico-inner-stroke-color", &inner_stroke_color,
                          "-unico-inner-stroke-gradient", &inner_stroke_pat,
                          NULL);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);

  _gtk_rounded_box_init_rect (&border_box, 0, 0, width, height);
  _gtk_rounded_box_apply_border_radius (&border_box, engine, state, junction);
  padding_box = border_box;
  _gtk_rounded_box_shrink (&padding_box, border.top, border.right, border.bottom, border.left);

  if (inner_stroke_pat)
    {
      unico_cairo_style_pattern_set_matrix (inner_stroke_pat, width, height);
      cairo_set_source (cr, inner_stroke_pat);
    }
  else
    gdk_cairo_set_source_rgba (cr, inner_stroke_color);

  _gtk_rounded_box_path (&border_box, cr);
  _gtk_rounded_box_path (&padding_box, cr);
  cairo_fill (cr);

  cairo_restore (cr);

  if (inner_stroke_pat != NULL)
    cairo_pattern_destroy (inner_stroke_pat);

  gdk_rgba_free (inner_stroke_color);
}

/* draw the outer stroke */
static void
draw_outer_stroke (GtkThemingEngine *engine,
                   cairo_t          *cr,
                   gdouble           x,
                   gdouble           y,
                   gdouble           width,
                   gdouble           height,
                   guint             hidden_side,
                   GtkJunctionSides  junction)
{
  GdkRGBA *outer_stroke_color;
  GtkBorder border;
  GtkRoundedBox border_box, padding_box;
  GtkStateFlags state; 
  cairo_pattern_t *outer_stroke_pat;

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get_border (engine, state, &border);
  hide_border_sides (&border, hidden_side);

  gtk_theming_engine_get (engine, state,
                          "-unico-outer-stroke-color", &outer_stroke_color,
                          "-unico-outer-stroke-gradient", &outer_stroke_pat,
                          NULL);

  cairo_save (cr);

  cairo_translate (cr, x, y);

  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);

  _gtk_rounded_box_init_rect (&border_box, 0, 0, width, height);
  _gtk_rounded_box_apply_border_radius (&border_box, engine, state, junction);
  padding_box = border_box;
  _gtk_rounded_box_shrink (&padding_box, border.top, border.right, border.bottom, border.left);

  if (outer_stroke_pat)
    {
      unico_cairo_style_pattern_set_matrix (outer_stroke_pat, width, height);
      cairo_set_source (cr, outer_stroke_pat);
    }
  else
    gdk_cairo_set_source_rgba (cr, outer_stroke_color);

  _gtk_rounded_box_path (&border_box, cr);
  _gtk_rounded_box_path (&padding_box, cr);
  cairo_fill (cr);

  cairo_restore (cr);

  if (outer_stroke_pat != NULL)
    cairo_pattern_destroy (outer_stroke_pat);

  gdk_rgba_free (outer_stroke_color);
}

void
unico_cairo_draw_frame (GtkThemingEngine *engine,
                        cairo_t          *cr,
                        gint              x,
                        gint              y,
                        gint              width,
                        gint              height,
                        guint             hidden_side,
                        GtkJunctionSides  junction)
{
  GtkBorder border;
  GtkStateFlags state;

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get_border (engine, state, &border);
  hide_border_sides (&border, hidden_side);

  if (unico_has_outer_stroke (engine))
    {
      draw_outer_stroke (engine, cr,
                         x, y,
                         width, height,
                         hidden_side, junction);

      shrink_with_border (border, &x, &y, &width, &height);
    }

  draw_inner_stroke (engine, cr,
                     x + border.left, y + border.top,
                     width - (border.left + border.right), height - (border.top + border.bottom),
                     hidden_side, junction);

  draw_border (engine, cr,
               x, y,
               width, height,
               hidden_side, junction);
}

void
unico_cairo_exchange_axis (cairo_t *cr,
                           gint    *x,
                           gint    *y,
                           gint    *width,
                           gint    *height)
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
unico_cairo_round_rect (cairo_t         *cr,
                        gdouble          x,
                        gdouble          y,
                        gdouble          width,
                        gdouble          height,
                        gint             radius,
                        guint            sides,
                        GtkJunctionSides junction)
{
  radius = CLAMP (radius, 0, MIN (width / 2, height / 2));

  if (sides & SIDE_RIGHT)
    {
      if (radius == 0 ||
          (junction & GTK_JUNCTION_CORNER_TOPRIGHT))
        cairo_move_to (cr, x + width, y);
      else
        {
          cairo_new_sub_path (cr);
          
          cairo_arc (cr, x + width - radius, y + radius, radius, - G_PI / 4, 0);
        }

      if (radius == 0 ||
          (junction & GTK_JUNCTION_CORNER_BOTTOMRIGHT))
        cairo_line_to (cr, x + width, y + height);
      else
        cairo_arc (cr, x + width - radius, y + height - radius, radius, 0, G_PI / 4);
    }

  if (sides & SIDE_BOTTOM)
    {
      if (radius != 0 &&
          ! (junction & GTK_JUNCTION_CORNER_BOTTOMRIGHT))
        {
          if ((sides & SIDE_RIGHT) == 0)
            cairo_new_sub_path (cr);

          cairo_arc (cr, x + width - radius, y + height - radius, radius, G_PI / 4, G_PI / 2);
        }
      else if ((sides & SIDE_RIGHT) == 0)
        cairo_move_to (cr, x + width, y + height);

      if (radius == 0 ||
          (junction & GTK_JUNCTION_CORNER_BOTTOMLEFT))
        cairo_line_to (cr, x, y + height);
      else
        cairo_arc (cr, x + radius, y + height - radius, radius, G_PI / 2, 3 * (G_PI / 4));
    }
  else
    cairo_move_to (cr, x, y + height);

  if (sides & SIDE_LEFT)
    {
      if (radius != 0 &&
          ! (junction & GTK_JUNCTION_CORNER_BOTTOMLEFT))
        {
          if ((sides & SIDE_BOTTOM) == 0)
            cairo_new_sub_path (cr);

          cairo_arc (cr, x + radius, y + height - radius, radius, 3 * (G_PI / 4), G_PI);
        }
      else if ((sides & SIDE_BOTTOM) == 0)
        cairo_move_to (cr, x, y + height);

      if (radius == 0 ||
          (junction & GTK_JUNCTION_CORNER_TOPLEFT))
        cairo_line_to (cr, x, y);
      else
        cairo_arc (cr, x + radius, y + radius, radius, G_PI, G_PI + G_PI / 4);
    }

  if (sides & SIDE_TOP)
    {
      if (radius != 0 &&
          ! (junction & GTK_JUNCTION_CORNER_TOPLEFT))
        {
          if ((sides & SIDE_LEFT) == 0)
            cairo_new_sub_path (cr);

          cairo_arc (cr, x + radius, y + radius, radius, 5 * (G_PI / 4), 3 * (G_PI / 2));
        }
      else if ((sides & SIDE_LEFT) == 0)
        cairo_move_to (cr, x, y);

      if (radius == 0 ||
          (junction & GTK_JUNCTION_CORNER_TOPRIGHT))
        cairo_line_to (cr, x + width, y);
      else
        cairo_arc (cr, x + width - radius, y + radius, radius, 3 * (G_PI / 2), - G_PI / 4);
    }
}

void
unico_cairo_round_rect_inner (cairo_t         *cr,
                              gdouble          x,
                              gdouble          y,
                              gdouble          width,
                              gdouble          height,
                              gint             radius,
                              guint            sides,
                              GtkJunctionSides junction)
{
  gdouble line_width;

  line_width = cairo_get_line_width (cr);

  unico_cairo_round_rect (cr, x + (line_width / 2.0),
                          y + (line_width / 2.0),
                          width - line_width,
                          height - line_width,
                          radius, sides, junction);
}

void
unico_cairo_set_source_border (GtkThemingEngine *engine,
                               cairo_t          *cr,
                               gdouble           width,
                               gdouble           height)
{
  GdkRGBA border_color;
  GtkBorderStyle border_style;
  GtkStateFlags state;
  cairo_pattern_t *border_pat;

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get_border_color (engine, state, &border_color);
  gtk_theming_engine_get (engine, state,
                          "border-style", &border_style,
                          "-unico-border-gradient", &border_pat,
                          NULL);

  if (border_pat)
    {
      unico_cairo_style_pattern_set_matrix (border_pat, width, height);
      cairo_set_source (cr, border_pat);
    }
  else
    gdk_cairo_set_source_rgba (cr, &border_color);


  if (border_pat != NULL)
    cairo_pattern_destroy (border_pat);
}

void
unico_cairo_set_source_inner_stroke (GtkThemingEngine *engine,
                                     cairo_t          *cr,
                                     gdouble           width,
                                     gdouble           height)
{
  GdkRGBA *inner_stroke_color;
  GtkStateFlags state;
  cairo_pattern_t *inner_stroke_pat;

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get (engine, state,
                          "-unico-inner-stroke-color", &inner_stroke_color,
                          "-unico-inner-stroke-gradient", &inner_stroke_pat,
                          NULL);

  if (inner_stroke_pat)
    {
      unico_cairo_style_pattern_set_matrix (inner_stroke_pat, width, height);
      cairo_set_source (cr, inner_stroke_pat);
    }
  else
    gdk_cairo_set_source_rgba (cr, inner_stroke_color);

  if (inner_stroke_pat != NULL)
    cairo_pattern_destroy (inner_stroke_pat);

  gdk_rgba_free (inner_stroke_color);
}

void
unico_cairo_set_source_color_with_alpha (cairo_t *cr,
                                         GdkRGBA *color,
                                         gdouble  alpha)
{
  g_return_if_fail (cr && color);

  cairo_set_source_rgba (cr, color->red, color->green, color->blue, alpha);
}

void
unico_cairo_style_pattern_set_matrix (cairo_pattern_t *pat,
                                      gdouble          width,
                                      gdouble          height)
{
  cairo_matrix_t matrix;
  gint w, h;

  if (cairo_pattern_get_type (pat) == CAIRO_PATTERN_TYPE_SURFACE)
    {
      cairo_surface_t *surface;

      cairo_pattern_get_surface (pat, &surface);
      w = cairo_image_surface_get_width (surface);
      h = cairo_image_surface_get_height (surface);
    }
  else
    w = h = 1;

  cairo_matrix_init_scale (&matrix, (gdouble) w / width, (gdouble) h / height);
  cairo_pattern_set_matrix (pat, &matrix);
}
