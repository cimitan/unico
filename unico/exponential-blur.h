/* The Unico Theming Engine for Gtk+.
 * Copyright (C) 2009 Canonical Ltd
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
 * Authored by Mirco "MacSlow" Mueller <mirco.mueller@canonical.com>
 *
 * Notes:
 *   based on exponential-blur algorithm by Jani Huhtanen
 *
 */

#ifndef _EXPONENTIAL_BLUR_H
#define _EXPONENTIAL_BLUR_H

#include <cairo.h>
#include <glib.h>

void
surface_exponential_blur (cairo_surface_t* surface,
                          guint            radius);

#endif // _EXPONENTIAL_BLUR_H

