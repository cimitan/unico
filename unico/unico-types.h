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

#ifndef UNICO_TYPES_H
#define UNICO_TYPES_H

#define DRAW_ARGS GtkThemingEngine *engine, \
                  cairo_t *cr, \
                  gint x, \
                  gint y, \
                  gint width, \
                  gint height

G_BEGIN_DECLS

typedef struct _UnicoStyleFunctions UnicoStyleFunctions;

typedef enum
{
  UNICO_CORNER_NONE        = 0,
  UNICO_CORNER_TOPLEFT     = 1,
  UNICO_CORNER_TOPRIGHT    = 2,
  UNICO_CORNER_BOTTOMLEFT  = 4,
  UNICO_CORNER_BOTTOMRIGHT = 8,
  UNICO_CORNER_ALL         = 15
} UnicoCorners;

typedef struct
{
	GtkShadowType   shadow;
	GtkPositionType gap_side;
	gint            gap_x;
	gint            gap_width;
} UnicoFrameParameters;

typedef struct
{
  gboolean lower;
  gboolean horizontal;
  gboolean fill_level;
} UnicoSliderParameters;

typedef enum
{
  UNICO_OUTER_STROKE_STYLE_CUSTOM = 0,
  UNICO_OUTER_STROKE_STYLE_INSET = 1,
  UNICO_OUTER_STROKE_STYLE_NONE = 2
} UnicoOuterStrokeStyle;

typedef enum
{
  UNICO_STYLE_DEFAULT = 0,
  UNICO_NUM_STYLES = 1
} UnicoStyles;

typedef struct
{
	GtkPositionType gap_side;
} UnicoTabParameters;

struct _UnicoStyleFunctions
{
  void (*draw_button_background) (DRAW_ARGS);

  void (*draw_button_frame) (DRAW_ARGS);

  void (*draw_cell) (DRAW_ARGS);

  void (*draw_check) (DRAW_ARGS);

  void (*draw_column_header_background) (DRAW_ARGS);

  void (*draw_column_header_frame) (DRAW_ARGS);

  void (*draw_combo_button_background) (DRAW_ARGS);

  void (*draw_combo_button_frame) (DRAW_ARGS);

  void (*draw_entry_background) (DRAW_ARGS);

  void (*draw_entry_frame) (DRAW_ARGS);

  void (*draw_frame) (DRAW_ARGS,
                      UnicoFrameParameters *frame);

  void (*draw_icon_view) (DRAW_ARGS);

  void (*draw_menu_frame) (DRAW_ARGS);

  void (*draw_menu_background) (DRAW_ARGS);

  void (*draw_menubar_background) (DRAW_ARGS);

  void (*draw_menubar_frame) (DRAW_ARGS);

  void (*draw_menubaritem_background) (DRAW_ARGS);

  void (*draw_menubaritem_frame) (DRAW_ARGS);

  void (*draw_menuitem_background) (DRAW_ARGS);

  void (*draw_menuitem_frame) (DRAW_ARGS);

  void (*draw_notebook) (DRAW_ARGS,
                         UnicoFrameParameters *frame);

  void (*draw_progressbar_fill_background) (DRAW_ARGS);

  void (*draw_progressbar_fill_frame) (DRAW_ARGS);

  void (*draw_progressbar_trough_background) (DRAW_ARGS);

  void (*draw_progressbar_trough_frame) (DRAW_ARGS);

  void (*draw_radio) (DRAW_ARGS);

  void (*draw_scrollbar_slider) (DRAW_ARGS);

  void (*draw_scrollbar_stepper_background) (DRAW_ARGS);

  void (*draw_scrollbar_stepper_frame) (DRAW_ARGS);

  void (*draw_scrollbar_trough_background) (DRAW_ARGS);

  void (*draw_scrollbar_trough_frame) (DRAW_ARGS);

  void (*draw_slider_button) (DRAW_ARGS,
                              UnicoSliderParameters *slider);

  void (*draw_tab) (DRAW_ARGS,
                    UnicoTabParameters *tab);

  void (*draw_toolbar_background) (DRAW_ARGS);

  void (*draw_toolbar_frame) (DRAW_ARGS);
};

G_END_DECLS

#endif /* UNICO_TYPES_H */
