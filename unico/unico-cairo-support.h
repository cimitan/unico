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

#ifndef UNICO_CAIRO_SUPPORT_H
#define UNICO_CAIRO_SUPPORT_H

#include <gtk/gtk.h>

#include "unico-types.h"

G_BEGIN_DECLS

G_GNUC_INTERNAL void unico_cairo_draw_background (cairo_t *cr,
                                                  GtkThemingEngine *engine,
                                                  int x,
                                                  int y,
                                                  int w,
                                                  int h);

G_GNUC_INTERNAL void unico_cairo_draw_background_from_path (cairo_t *cr,
                                                            GtkThemingEngine *engine,
                                                            double x,
                                                            double y,
                                                            double width,
                                                            double height);

G_GNUC_INTERNAL void unico_cairo_draw_background_rect (cairo_t *cr,
                                                       GtkThemingEngine *engine,
                                                       double x,
                                                       double y,
                                                       double width,
                                                       double height,
                                                       int radius,
                                                       UnicoCorners corners);

G_GNUC_INTERNAL void unico_cairo_draw_border_from_path (cairo_t *cr,
                                                        GtkThemingEngine *engine,
                                                        double x,
                                                        double y,
                                                        double width,
                                                        double height);

G_GNUC_INTERNAL void unico_cairo_draw_border_rect (cairo_t *cr,
                                                   GtkThemingEngine *engine,
                                                   double x,
                                                   double y,
                                                   double width,
                                                   double height,
                                                   int radius,
                                                   UnicoCorners corners);

G_GNUC_INTERNAL void unico_cairo_draw_frame (cairo_t *cr,
                                             GtkThemingEngine *engine,
                                             int x,
                                             int y,
                                             int w,
                                             int h);

G_GNUC_INTERNAL void unico_cairo_draw_stroke_inner_from_path (cairo_t *cr,
                                                              GtkThemingEngine *engine,
                                                              double x,
                                                              double y,
                                                              double width,
                                                              double height);

G_GNUC_INTERNAL void unico_cairo_draw_stroke_inner_rect (cairo_t *cr,
                                                         GtkThemingEngine *engine,
                                                         double x,
                                                         double y,
                                                         double width,
                                                         double height,
                                                         int radius,
                                                         UnicoCorners corners);

G_GNUC_INTERNAL void unico_cairo_draw_stroke_outer_from_path (cairo_t *cr,
                                                              GtkThemingEngine *engine,
                                                              double x,
                                                              double y,
                                                              double width,
                                                              double height);

G_GNUC_INTERNAL void unico_cairo_draw_stroke_outer_rect (cairo_t *cr,
                                                         GtkThemingEngine *engine,
                                                         double x,
                                                         double y,
                                                         double width,
                                                         double height,
                                                         int radius,
                                                         UnicoCorners corners);

G_GNUC_INTERNAL void unico_cairo_exchange_axis (cairo_t *cr,
                                                int *x,
                                                int *y,
                                                int *width,
                                                int *height);

G_GNUC_INTERNAL void unico_cairo_rounded_rect (cairo_t *cr,
                                               double x,
                                               double y,
                                               double w,
                                               double h,
                                               int radius,
                                               UnicoCorners corners);

G_GNUC_INTERNAL void unico_cairo_rounded_rect_inner (cairo_t *cr,
                                                     double x,
                                                     double y,
                                                     double width,
                                                     double height,
                                                     int radius,
                                                     UnicoCorners corners);

G_GNUC_INTERNAL void unico_cairo_set_source_color (cairo_t *cr,
                                                   GdkRGBA *color);

G_END_DECLS

#endif /* UNICO_CAIRO_SUPPORT_H */
