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
                              int x, int y, int width, int height,
                              ButtonParameters *button)
{
  unico_draw_background (cr, engine, x, y, width, height);
}

static void
unico_draw_button_frame (cairo_t *cr,
                         GtkThemingEngine *engine,
                         int x, int y, int width, int height,
                         ButtonParameters *button)
{
  unico_draw_frame (cr, engine, x, y, width, height);
}

void
unico_register_style_default (UnicoStyleFunctions *functions)
{
  g_assert (functions);

  functions->draw_button_background = unico_draw_button_background;
  functions->draw_button_frame      = unico_draw_button_frame;
}

