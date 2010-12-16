/* The Unico Theme Engine for Gtk+.
 * Copyright (C) 2010 Andrea Cimitan <andrea.cimitan@canonical.com>
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
#include <gtk/gtk.h>

#include "unico.h"
#include "unico-engine.h"

#define UNICO_NAMESPACE "unico"

G_DEFINE_DYNAMIC_TYPE (UnicoEngine, unico_engine, GTK_TYPE_THEMING_ENGINE)

static void
unico_engine_render_arrow (GtkThemingEngine *engine,
                           cairo_t *cr,
                           gdouble angle,
                           gdouble x,
                           gdouble y,
                           gdouble size)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_arrow (engine, cr, angle, x, y, size);
}

static void
unico_engine_render_background (GtkThemingEngine *engine,
                                cairo_t *cr,
                                gdouble x,
                                gdouble y,
                                gdouble width,
                                gdouble height)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_background (engine, cr, x, y, width, height);
}

static void
unico_engine_render_check (GtkThemingEngine *engine,
                           cairo_t *cr,
                           gdouble x,
                           gdouble y,
                           gdouble width,
                           gdouble height)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_check (engine, cr, x, y, width, height);
}

static void
unico_engine_render_extension (GtkThemingEngine *engine,
                               cairo_t *cr,
                               gdouble x,
                               gdouble y,
                               gdouble width,
                               gdouble height,
                               GtkPositionType gap_side)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_extension (engine, cr, x, y, width, height, gap_side);
}

static void
unico_engine_render_focus (GtkThemingEngine *engine,
                           cairo_t *cr,
                           gdouble x,
                           gdouble y,
                           gdouble width,
                           gdouble height)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_focus (engine, cr, x, y, width, height);
}

static void
unico_engine_render_frame (GtkThemingEngine *engine,
                           cairo_t *cr,
                           gdouble x,
                           gdouble y,
                           gdouble width,
                           gdouble height)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_frame (engine, cr, x, y, width, height);
}

static void
unico_engine_render_frame_gap (GtkThemingEngine *engine,
                               cairo_t *cr,
                               gdouble x,
                               gdouble y,
                               gdouble width,
                               gdouble height,
                               GtkPositionType gap_side,
                               gdouble xy0_gap,
                               gdouble xy1_gap)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_frame_gap (engine, cr, x, y, width, height, gap_side, xy0_gap, xy1_gap);
}

static void
unico_engine_render_handle (GtkThemingEngine *engine,
                            cairo_t *cr,
                            gdouble x,
                            gdouble y,
                            gdouble width,
                            gdouble height)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_handle (engine, cr, x, y, width, height);
}

static GdkPixbuf*
unico_engine_render_icon_pixbuf (GtkThemingEngine *engine,
                                 const GtkIconSource *source,
                                 GtkIconSize size)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_icon_pixbuf (engine, source, size);
}

static void
unico_engine_render_layout (GtkThemingEngine *engine,
                            cairo_t *cr,
                            gdouble x,
                            gdouble y,
                            PangoLayout *layout)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_layout (engine, cr, x, y, layout);
}

static void
unico_engine_render_option (GtkThemingEngine *engine,
                            cairo_t *cr,
                            gdouble x,
                            gdouble y,
                            gdouble width,
                            gdouble height)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_option (engine, cr, x, y, width, height);
}

static void
unico_engine_render_slider (GtkThemingEngine *engine,
                            cairo_t *cr,
                            gdouble x,
                            gdouble y,
                            gdouble width,
                            gdouble height,
                            GtkOrientation orientation)
{
	GTK_THEMING_ENGINE_CLASS (unico_engine_parent_class)->render_slider (engine, cr, x, y, width, height, orientation);
}

void
unico_engine_register_types (GTypeModule *module)
{
	unico_engine_register_type (module);
}

static void
unico_engine_init (UnicoEngine *unico_engine)
{
}

static void
unico_engine_class_init (UnicoEngineClass * klass)
{
	GtkThemingEngineClass *engine_class = GTK_THEMING_ENGINE_CLASS (klass);

	engine_class->render_arrow       = unico_engine_render_arrow;
	engine_class->render_background  = unico_engine_render_background;
	engine_class->render_check       = unico_engine_render_check;
	engine_class->render_extension   = unico_engine_render_extension;
	engine_class->render_focus       = unico_engine_render_focus;
	engine_class->render_frame       = unico_engine_render_frame;
	engine_class->render_frame_gap   = unico_engine_render_frame_gap;
	engine_class->render_handle      = unico_engine_render_handle;
	engine_class->render_icon_pixbuf = unico_engine_render_icon_pixbuf;
	engine_class->render_layout      = unico_engine_render_layout;
	engine_class->render_option      = unico_engine_render_option;
	engine_class->render_slider      = unico_engine_render_slider;

	gtk_theming_engine_register_property (UNICO_NAMESPACE, NULL,
	                                      g_param_spec_boxed ("focus-color",
	                                                          "Focus color",
	                                                          "Focus color",
	                                                          GDK_TYPE_RGBA, 0));
}

static void
unico_engine_class_finalize (UnicoEngineClass *klass)
{
}
