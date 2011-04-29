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
#include <cairo-gobject.h>
#include <gtk/gtk.h>

#include "unico.h"
#include "unico-css-support.h"
#include "unico-engine.h"
#include "unico-types.h"

#define UNICO_NAMESPACE "unico"

#define UNICO_CAIRO_INIT \
        cairo_set_line_width (cr, 1.0); \
        cairo_set_line_cap (cr, CAIRO_LINE_CAP_SQUARE); \
        cairo_set_line_join (cr, CAIRO_LINE_JOIN_MITER);

G_DEFINE_DYNAMIC_TYPE (UnicoEngine, unico_engine, GTK_TYPE_THEMING_ENGINE)

static void
unico_engine_render_activity (GtkThemingEngine *engine,
                              cairo_t          *cr,
                              gdouble           x,
                              gdouble           y,
                              gdouble           width,
                              gdouble           height)
{
  UnicoStyleFunctions *style_functions;

  UNICO_CAIRO_INIT

  unico_lookup_functions (UNICO_ENGINE (engine), &style_functions);

  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_PROGRESSBAR))
    {
      style_functions->draw_progressbar_fill_background (engine, cr, x, y, width, height);
      style_functions->draw_progressbar_fill_frame (engine, cr, x, y, width, height);
    }
  else
    GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_activity (engine, cr, x, y, width, height);
}

static void
unico_engine_render_arrow (GtkThemingEngine *engine,
                           cairo_t          *cr,
                           gdouble           angle,
                           gdouble           x,
                           gdouble           y,
                           gdouble           size)
{
  GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_arrow (engine, cr, angle, x, y, size);
}

static void
unico_engine_render_background (GtkThemingEngine *engine,
                                cairo_t          *cr,
                                gdouble           x,
                                gdouble           y,
                                gdouble           width,
                                gdouble           height)
{
  UnicoStyleFunctions *style_functions;
  const GtkWidgetPath *path;

  UNICO_CAIRO_INIT

  unico_lookup_functions (UNICO_ENGINE (engine), &style_functions);
  path = gtk_theming_engine_get_path (engine);

  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUBAR))
    style_functions->draw_menubar_background (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_TOOLBAR) ||
           gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_DOCK))
    style_functions->draw_toolbar_background (engine, cr, x, y, width, height);
  else if (gtk_widget_path_is_type (path, GTK_TYPE_COMBO_BOX) ||
           gtk_widget_path_has_parent (path, GTK_TYPE_COMBO_BOX))
    style_functions->draw_combo_button_background (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_BUTTON) &&
           gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_SCROLLBAR))
    style_functions->draw_scrollbar_stepper_background (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_BUTTON) &&
           gtk_theming_engine_has_class (engine, GTK_STYLE_REGION_COLUMN_HEADER))
    style_functions->draw_column_header_background (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_BUTTON))
    style_functions->draw_button_background (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_ENTRY) &&
           !gtk_widget_path_has_parent (path, GTK_TYPE_TREE_VIEW))
    style_functions->draw_entry_background (engine, cr, x, y, width, height);
  else if (gtk_widget_path_is_type (path, GTK_TYPE_ICON_VIEW))
      style_functions->draw_icon_view (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_CELL))
      style_functions->draw_cell (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUITEM) &&
           gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUBAR))
    style_functions->draw_menubaritem_background (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUITEM))
    style_functions->draw_menuitem_background (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENU))
    style_functions->draw_menu_background (engine, cr, x, y, width, height);
  else if (gtk_widget_path_is_type (path, GTK_TYPE_PROGRESS_BAR) &&
           gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_TROUGH))
    style_functions->draw_progressbar_trough_background (engine, cr, x, y, width, height);
  else if (gtk_widget_path_is_type (path, GTK_TYPE_SCROLLBAR) &&
           gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_TROUGH))
    style_functions->draw_scrollbar_trough_background (engine, cr, x, y, width, height);
  else
    GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_background (engine, cr, x, y, width, height);
}

static void
unico_engine_render_check (GtkThemingEngine *engine,
                           cairo_t          *cr,
                           gdouble           x,
                           gdouble           y,
                           gdouble           width,
                           gdouble           height)
{
  UnicoStyleFunctions *style_functions;

  UNICO_CAIRO_INIT

  unico_lookup_functions (UNICO_ENGINE (engine), &style_functions);

  style_functions->draw_check (engine, cr, x, y, width, height);
}

static void
unico_engine_render_expander (GtkThemingEngine *engine,
                              cairo_t          *cr,
                              gdouble           x,
                              gdouble           y,
                              gdouble           width,
                              gdouble           height)
{
  /* FIXME put the code in unico-draw.c */
  GdkRGBA border, bg, fg;
  GtkStateFlags state;
  gint side;

  side = MIN (width, height);

  x += ((int) width / 2) - (side / 2);
  y += ((int) height / 2) - (side / 2);

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get_border_color (engine, state, &border);
  gtk_theming_engine_get_background_color (engine, state, &bg);
  gtk_theming_engine_get_color (engine, state, &fg);

  cairo_save (cr);

  cairo_set_line_width (cr, 1);

  unico_cairo_rounded_rect (cr, x + 0.5, y + 0.5, side, side, 2);
  gdk_cairo_set_source_rgba (cr, &bg);
  cairo_fill_preserve (cr);

  gdk_cairo_set_source_rgba (cr, &border);
  cairo_stroke (cr);

  cairo_set_line_width (cr, 1);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
  gdk_cairo_set_source_rgba (cr, &fg);

  cairo_move_to (cr, x + 3, y + side / 2 + 0.5);
  cairo_line_to (cr, x + side - 2, y + side / 2 + 0.5);

  if ((state & GTK_STATE_FLAG_ACTIVE) == 0)
  {
    cairo_move_to (cr, x + side / 2 + 0.5, y + 3);
    cairo_line_to (cr, x + side / 2 + 0.5, y + side - 2);
  }

  cairo_stroke (cr);

  cairo_restore (cr);
}

static void
unico_engine_render_extension (GtkThemingEngine *engine,
                               cairo_t          *cr,
                               gdouble           x,
                               gdouble           y,
                               gdouble           width,
                               gdouble           height,
                               GtkPositionType   gap_side)
{
  UnicoStyleFunctions *style_functions;

  UNICO_CAIRO_INIT

  unico_lookup_functions (UNICO_ENGINE (engine), &style_functions);

  if (gtk_theming_engine_has_region (engine, GTK_STYLE_REGION_TAB, NULL))
    {
      UnicoTabParameters tab;

      tab.gap_side = gap_side;

      style_functions->draw_tab (engine, cr, x, y, width, height, &tab);
    }
  else
    GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_extension (engine, cr, x, y, width, height, gap_side);
}

static void
unico_engine_render_focus (GtkThemingEngine *engine,
                           cairo_t          *cr,
                           gdouble           x,
                           gdouble           y,
                           gdouble           width,
                           gdouble           height)
{
  GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_focus (engine, cr, x, y, width, height);
}

static void
unico_engine_render_frame (GtkThemingEngine *engine,
                           cairo_t          *cr,
                           gdouble           x,
                           gdouble           y,
                           gdouble           width,
                           gdouble           height)
{
  UnicoStyleFunctions *style_functions;
  const GtkWidgetPath *path;

  UNICO_CAIRO_INIT

  unico_lookup_functions (UNICO_ENGINE (engine), &style_functions);
  path = gtk_theming_engine_get_path (engine);

  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUBAR))
    style_functions->draw_menubar_frame (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_TOOLBAR) ||
           gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_DOCK))
    style_functions->draw_toolbar_frame (engine, cr, x, y, width, height);
  else if (gtk_widget_path_is_type (path, GTK_TYPE_COMBO_BOX) ||
           gtk_widget_path_has_parent (path, GTK_TYPE_COMBO_BOX))
    style_functions->draw_combo_button_frame (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_BUTTON) &&
           gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_SCROLLBAR))
    style_functions->draw_scrollbar_stepper_frame (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_BUTTON) &&
           gtk_theming_engine_has_class (engine, GTK_STYLE_REGION_COLUMN_HEADER))
    style_functions->draw_column_header_frame (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_BUTTON))
    style_functions->draw_button_frame (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_ENTRY) &&
           !gtk_widget_path_has_parent (path, GTK_TYPE_TREE_VIEW))
    style_functions->draw_entry_frame (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUITEM) &&
           gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUBAR))
    style_functions->draw_menubaritem_frame (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUITEM))
    style_functions->draw_menuitem_frame (engine, cr, x, y, width, height);
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENU))
    style_functions->draw_menu_frame (engine, cr, x, y, width, height);
  else if (gtk_widget_path_is_type (path, GTK_TYPE_PROGRESS_BAR) &&
           gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_TROUGH))
    style_functions->draw_progressbar_trough_frame (engine, cr, x, y, width, height);
  else if (gtk_widget_path_is_type (path, GTK_TYPE_SCROLLBAR) &&
           gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_TROUGH))
    style_functions->draw_scrollbar_trough_frame (engine, cr, x, y, width, height);
  else
    GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_frame (engine, cr, x, y, width, height);
}

static void
unico_engine_render_frame_gap (GtkThemingEngine *engine,
                               cairo_t          *cr,
                               gdouble           x,
                               gdouble           y,
                               gdouble           width,
                               gdouble           height,
                               GtkPositionType   gap_side,
                               gdouble           xy0_gap,
                               gdouble           xy1_gap)
{
  UnicoStyleFunctions *style_functions;

  UNICO_CAIRO_INIT

  unico_lookup_functions (UNICO_ENGINE (engine), &style_functions);

  if (gtk_theming_engine_has_class (engine, "notebook"))
    {
      UnicoFrameParameters frame;

      frame.gap_side = gap_side;
      frame.gap_x = xy0_gap;
      frame.gap_width = xy1_gap-xy0_gap;

      style_functions->draw_notebook (engine, cr, x, y, width, height, &frame);
    }
  else if (gtk_theming_engine_has_class (engine, "frame"))
    {
      UnicoFrameParameters frame;

      frame.gap_side = gap_side;
      frame.gap_x = xy0_gap;
      frame.gap_width = xy1_gap-xy0_gap;

      style_functions->draw_frame (engine, cr, x, y, width, height, &frame);
    }
  else
    GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_frame_gap (engine, cr,
                                                                            x, y, width, height,
                                                                            gap_side, xy0_gap, xy1_gap);
}

static void
unico_engine_render_handle (GtkThemingEngine *engine,
                            cairo_t          *cr,
                            gdouble           x,
                            gdouble           y,
                            gdouble           width,
                            gdouble           height)
{
  GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_handle (engine, cr, x, y, width, height);
}

static GdkPixbuf*
pixbuf_set_transparency (GdkPixbuf *pixbuf,
                         gdouble    alpha_percent)
{
  GdkPixbuf *target;
  guchar *data, *current;
  guint x, y, rowstride, height, width;

  g_return_val_if_fail (pixbuf != NULL, NULL);
  g_return_val_if_fail (GDK_IS_PIXBUF (pixbuf), NULL);

  /* Returns a copy of pixbuf with it's non-completely-transparent pixels to
     have an alpha level "alpha_percent" of their original value. */

  target = gdk_pixbuf_add_alpha (pixbuf, FALSE, 0, 0, 0);

  if (alpha_percent == 1.0)
    return target;

  width = gdk_pixbuf_get_width (target);
  height = gdk_pixbuf_get_height (target);
  rowstride = gdk_pixbuf_get_rowstride (target);
  data = gdk_pixbuf_get_pixels (target);

  for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
        {
          /* The "4" is the number of chars per pixel, in this case, RGBA,
             the 3 means "skip to the alpha" */
          current = data + (y * rowstride) + (x * 4) + 3;
          *(current) = (guchar) (*(current) * alpha_percent);
        }
    }

  return target;
}

static GdkPixbuf*
pixbuf_scale_or_ref (GdkPixbuf *src,
                     gint       width,
                     gint       height)
{
  if (width == gdk_pixbuf_get_width (src) &&
      height == gdk_pixbuf_get_height (src))
    return g_object_ref (src);
  else
    return gdk_pixbuf_scale_simple (src,
                                    width, height,
                                    GDK_INTERP_BILINEAR);
}

static GdkPixbuf*
unico_engine_render_icon_pixbuf (GtkThemingEngine    *engine,
                                 const GtkIconSource *source,
                                 GtkIconSize          size)
{
  GdkPixbuf *scaled;
  GdkPixbuf *stated;
  GdkPixbuf *base_pixbuf;
  GdkScreen *screen;
  GtkSettings *settings;
  GtkStateFlags state;
  gint width = 1;
  gint height = 1;

  base_pixbuf = gtk_icon_source_get_pixbuf (source);
  screen = gtk_theming_engine_get_screen (engine);
  settings = gtk_settings_get_for_screen (screen);
  state = gtk_theming_engine_get_state (engine);

  g_return_val_if_fail (base_pixbuf != NULL, NULL);

  if (size != (GtkIconSize) -1 && !gtk_icon_size_lookup_for_settings (settings, size, &width, &height))
    {
      g_warning (G_STRLOC ": invalid icon size '%d'", size);
      return NULL;
    }

  /* If the size was wildcarded, and we're allowed to scale, then scale; otherwise,
   * leave it alone.
   */
  if (size != (GtkIconSize)-1 && gtk_icon_source_get_size_wildcarded (source))
    scaled = pixbuf_scale_or_ref (base_pixbuf, width, height);
  else
    scaled = g_object_ref (base_pixbuf);

  /* If the state was wildcarded, then generate a state. */
  if (gtk_icon_source_get_state_wildcarded (source))
    {
      if (state & GTK_STATE_FLAG_INSENSITIVE)
        {
          stated = pixbuf_set_transparency (scaled, 0.3);
          gdk_pixbuf_saturate_and_pixelate (stated, stated, 0.1, FALSE);

          g_object_unref (scaled);
        }
      else if (state & GTK_STATE_FLAG_PRELIGHT)
        {
          stated = gdk_pixbuf_copy (scaled);

          gdk_pixbuf_saturate_and_pixelate (scaled, stated, 1.2, FALSE);

          g_object_unref (scaled);
        }
      else
        stated = scaled;
    }
  else
    stated = scaled;

  return stated;
}

static void
unico_engine_render_layout (GtkThemingEngine *engine,
                            cairo_t          *cr,
                            gdouble           x,
                            gdouble           y,
                            PangoLayout      *layout)
{
  GdkRGBA *color;
  GtkStateFlags state;

  UNICO_CAIRO_INIT

  cairo_translate (cr, x, y);

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "color", &color,
                          NULL);

  gdk_cairo_set_source_rgba (cr, color);

  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_ACCELERATOR))
    {
      unico_cairo_set_source_color_with_alpha (cr, color, 0.6);
    }

  if (state & GTK_STATE_FLAG_INSENSITIVE)
    {
      GdkRGBA *text_shadow_color;

      gtk_theming_engine_get (engine, state,
                              "-unico-text-shadow-color", &text_shadow_color,
                              NULL);

      cairo_save (cr);

      cairo_translate (cr, 1, 1);

      if (text_shadow_color)
        gdk_cairo_set_source_rgba (cr, text_shadow_color);
      else
        cairo_set_source_rgba (cr, 1, 1, 1, 0.6);

      pango_cairo_show_layout (cr, layout);

      cairo_restore (cr);

      gdk_rgba_free (text_shadow_color);
    }

  pango_cairo_show_layout (cr, layout);

  gdk_rgba_free (color);
}

static void
unico_engine_render_line (GtkThemingEngine *engine,
                          cairo_t          *cr,
                          gdouble           x0,
                          gdouble           y0,
                          gdouble           x1,
                          gdouble           y1)
{
  const GtkWidgetPath *path;

  UNICO_CAIRO_INIT

  path = gtk_theming_engine_get_path (engine);

  /* FIXME we need better theming here. */
  if ((gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MARK) &&
       gtk_widget_path_is_type (path, GTK_TYPE_SCALE)) ||
      (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_SEPARATOR) &&
       gtk_widget_path_is_type (path, GTK_TYPE_TREE_VIEW)))
    {
      GdkRGBA bg;
      GtkStateFlags state;

      state = gtk_theming_engine_get_state (engine);
      gtk_theming_engine_get_background_color (engine, state, &bg);

      cairo_save (cr);

      cairo_move_to (cr, x0 + 0.5, y0 + 0.5);
      cairo_line_to (cr, x1 + 0.5, y1 + 0.5);

      gdk_cairo_set_source_rgba (cr, &bg);
      cairo_stroke (cr);

      cairo_restore (cr);
    }
  else
    GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_line (engine, cr, x0, y0, x1, y1);
}

static void
unico_engine_render_option (GtkThemingEngine *engine,
                            cairo_t          *cr,
                            gdouble           x,
                            gdouble           y,
                            gdouble           width,
                            gdouble           height)
{
  UnicoStyleFunctions *style_functions;

  UNICO_CAIRO_INIT

  unico_lookup_functions (UNICO_ENGINE (engine), &style_functions);

  style_functions->draw_radio (engine, cr, x, y, width, height);
}

static void
unico_engine_render_slider (GtkThemingEngine *engine,
                            cairo_t          *cr,
                            gdouble           x,
                            gdouble           y,
                            gdouble           width,
                            gdouble           height,
                            GtkOrientation    orientation)
{
  UnicoStyleFunctions *style_functions;

  UNICO_CAIRO_INIT

  unico_lookup_functions (UNICO_ENGINE (engine), &style_functions);

  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_SLIDER) &&
      gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_SCROLLBAR))
    {
      style_functions->draw_scrollbar_slider (engine, cr, x, y, width, height);
    }
  else
    {
      UnicoSliderParameters slider;

      slider.horizontal = (orientation == GTK_ORIENTATION_HORIZONTAL);
      slider.lower = FALSE;
      slider.fill_level = FALSE;

      style_functions->draw_slider_button (engine, cr, x, y, width, height, &slider);
    }
}

void
unico_engine_register_types (GTypeModule *module)
{
  unico_engine_register_type (module);
}

static void
unico_engine_init (UnicoEngine *engine)
{
  unico_register_style_default (&engine->style_functions[UNICO_STYLE_DEFAULT]);
}

static void
unico_engine_class_init (UnicoEngineClass *klass)
{
  GtkThemingEngineClass *engine_class = GTK_THEMING_ENGINE_CLASS (klass);

  engine_class->render_activity    = unico_engine_render_activity;
  engine_class->render_arrow       = unico_engine_render_arrow;
  engine_class->render_background  = unico_engine_render_background;
  engine_class->render_check       = unico_engine_render_check;
  engine_class->render_expander    = unico_engine_render_expander;
  engine_class->render_extension   = unico_engine_render_extension;
  engine_class->render_focus       = unico_engine_render_focus;
  engine_class->render_frame       = unico_engine_render_frame;
  engine_class->render_frame_gap   = unico_engine_render_frame_gap;
  engine_class->render_handle      = unico_engine_render_handle;
  engine_class->render_icon_pixbuf = unico_engine_render_icon_pixbuf;
  engine_class->render_layout      = unico_engine_render_layout;
  engine_class->render_line        = unico_engine_render_line;
  engine_class->render_option      = unico_engine_render_option;
  engine_class->render_slider      = unico_engine_render_slider;

  gtk_theming_engine_register_property (UNICO_NAMESPACE, NULL,
                                        g_param_spec_boxed ("border-gradient",
                                                            "Border gradient",
                                                            "Border gradient",
                                                            CAIRO_GOBJECT_TYPE_PATTERN, 0));

  gtk_theming_engine_register_property (UNICO_NAMESPACE, NULL,
                                        g_param_spec_boxed ("bullet-color",
                                                            "Bullet color",
                                                            "Bullet color",
                                                            GDK_TYPE_RGBA, 0));

  gtk_theming_engine_register_property (UNICO_NAMESPACE, NULL,
                                        g_param_spec_boxed ("bullet-outline-color",
                                                            "Bullet outline color",
                                                            "Bullet outline color",
                                                            GDK_TYPE_RGBA, 0));

  gtk_theming_engine_register_property (UNICO_NAMESPACE, NULL,
                                        g_param_spec_boxed ("focus-color",
                                                            "Focus color",
                                                            "Focus color",
                                                            GDK_TYPE_RGBA, 0));

  gtk_theming_engine_register_property (UNICO_NAMESPACE, NULL,
                                        g_param_spec_boxed ("inner-stroke-color",
                                                            "Inner stroke color",
                                                            "Inner stroke color",
                                                            GDK_TYPE_RGBA, 0));

  gtk_theming_engine_register_property (UNICO_NAMESPACE, NULL,
                                        g_param_spec_boxed ("inner-stroke-gradient",
                                                            "Inner stroke gradient",
                                                            "Inner stroke gradient",
                                                            CAIRO_GOBJECT_TYPE_PATTERN, 0));

  gtk_theming_engine_register_property (UNICO_NAMESPACE, NULL,
                                        g_param_spec_boxed ("outer-stroke-color",
                                                            "Outer stroke color",
                                                            "Outer stroke color",
                                                            GDK_TYPE_RGBA, 0));

  gtk_theming_engine_register_property (UNICO_NAMESPACE, NULL,
                                        g_param_spec_boxed ("outer-stroke-gradient",
                                                            "Outer stroke gradient",
                                                            "Outer stroke gradient",
                                                            CAIRO_GOBJECT_TYPE_PATTERN, 0));

  gtk_theming_engine_register_property (UNICO_NAMESPACE, NULL,
                                        g_param_spec_enum ("outer-stroke-style",
                                                           "Outer stroke style",
                                                           "Outer stroke style",
                                                           UNICO_TYPE_OUTER_STROKE_STYLE,
                                                           UNICO_OUTER_STROKE_STYLE_NONE,
                                                           0));

  gtk_theming_engine_register_property (UNICO_NAMESPACE, NULL,
                                        g_param_spec_boxed ("text-shadow-color",
                                                            "Text shadow color",
                                                            "Text shadow color",
                                                            GDK_TYPE_RGBA, 0));
}

static void
unico_engine_class_finalize (UnicoEngineClass *klass)
{
}
