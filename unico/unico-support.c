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

#include "unico-engine.h"
#include "unico-types.h"

void
unico_lookup_functions (UnicoEngine *engine,
                        UnicoStyleFunctions **functions)
{
  if (functions)
    *functions = &engine->style_functions[UNICO_STYLE_DEFAULT];
}

UnicoCorners
unico_get_corners (GtkThemingEngine *engine)
{
  GtkJunctionSides sides;
  UnicoCorners corners = UNICO_CORNER_ALL;

  sides = gtk_theming_engine_get_junction_sides (engine);

  if (sides == 0)
    return UNICO_CORNER_ALL;

  if (sides & GTK_JUNCTION_LEFT)
    corners &= ~(UNICO_CORNER_TOPLEFT | UNICO_CORNER_BOTTOMLEFT);
  if (sides & GTK_JUNCTION_RIGHT)
    corners &= ~(UNICO_CORNER_TOPRIGHT | UNICO_CORNER_BOTTOMRIGHT);
  if (sides & GTK_JUNCTION_BOTTOM)
    corners &= ~(UNICO_CORNER_BOTTOMLEFT | UNICO_CORNER_BOTTOMRIGHT);
  if (sides & GTK_JUNCTION_TOP)
    corners &= ~(UNICO_CORNER_TOPLEFT | UNICO_CORNER_TOPRIGHT);

  return corners;
}

double
unico_get_line_width (GtkThemingEngine *engine)
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

  gtk_border_free (border);

  return line_width;
}

int
unico_get_border_radius (GtkThemingEngine *engine)
{
  GtkStateFlags state;
  int radius;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "border-radius", &radius,
                          NULL);

  return radius;
}
