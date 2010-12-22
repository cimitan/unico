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

#ifndef UNICO_SUPPORT_H
#define UNICO_SUPPORT_H

#include <gtk/gtk.h>

#include "unico-engine.h"
#include "unico-types.h"

G_BEGIN_DECLS

G_GNUC_INTERNAL void unico_lookup_functions (UnicoEngine *engine,
                                             UnicoStyleFunctions **functions);

G_GNUC_INTERNAL UnicoCorners unico_get_corners (GtkThemingEngine *engine);

G_GNUC_INTERNAL void unico_get_line_width (GtkThemingEngine *engine, double *line_width);

G_GNUC_INTERNAL int unico_get_border_radius (GtkThemingEngine *engine, int *radius);

G_END_DECLS

#endif /* UNICO_SUPPORT_H */