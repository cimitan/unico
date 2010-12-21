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

#ifndef UNICO_TYPES_H
#define UNICO_TYPES_H

G_BEGIN_DECLS

typedef unsigned char boolean;
typedef unsigned char uint8;
typedef struct _UnicoStyleFunctions UnicoStyleFunctions;

typedef struct
{
  boolean horizontal;
} UnicoButtonParameters;

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
	int             gap_x;
	int             gap_width;
} UnicoFrameParameters;

typedef struct
{
  boolean lower;
  boolean horizontal;
  boolean fill_level;
} UnicoSliderParameters;

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
#if 0
  void (*draw_arrow) (cairo_t *cr,
                      GtkThemingEngine *engine,
                      const ArrowParameters  *arrow,
                      int x, int y, int width, int height);
#endif
  void (*draw_button_background) (cairo_t *cr,
                                  GtkThemingEngine *engine,
                                  int x,
                                  int y,
                                  int width,
                                  int height,
                                  UnicoButtonParameters *button);

  void (*draw_button_frame) (cairo_t *cr,
                             GtkThemingEngine *engine,
                             int x,
                             int y,
                             int width,
                             int height,
                             UnicoButtonParameters *button);
#if 0
  void (*draw_checkbox) (cairo_t *cr,
                         GtkThemingEngine *engine,
                         const CheckboxParameters *checkbox,
                         int x, int y, int width, int height,
                         double trans);

  void (*draw_combo_separator) (cairo_t *cr,
                                GtkThemingEngine *engine,
                                int x, int y, int width, int height);

  void (*draw_combobox) (cairo_t *cr,
                         GtkThemingEngine *engine,
                         const ComboBoxParameters *combobox,
                         int x, int y, int width, int height,
                         boolean vertical);

  void (*draw_entry) (cairo_t *cr,
                      GtkThemingEngine *engine,
                      const FocusParameters  *focus,
                      int x, int y, int width, int height);

  void (*draw_entry_progress)   (cairo_t *cr,
                                 GtkThemingEngine *engine,
                                 const EntryProgressParameters *progress,
                                 int x, int y, int width, int height);

  void (*draw_expander) (cairo_t *cr,
                         GtkThemingEngine *engine,
                         const ExpanderParameters *expander,
                         int x, int y);

  void (*draw_focus) (cairo_t *cr,
                      GtkThemingEngine *engine,
                      const FocusParameters  *focus,
                      int x, int y, int width, int height);
#endif
  void (*draw_frame) (cairo_t *cr,
                      GtkThemingEngine *engine,
                      int x,
                      int y,
                      int width,
                      int height,
                      UnicoFrameParameters *frame);

  void (*draw_notebook) (cairo_t *cr,
                         GtkThemingEngine *engine,
                         int x,
                         int y,
                         int width,
                         int height,
                         UnicoFrameParameters *frame);

#if 0
  void (*draw_handle) (cairo_t *cr,
                       GtkThemingEngine *engine,
                       const HandleParameters *handle,
                       int x, int y, int width, int height);

  void (*draw_iconview) (cairo_t *cr,
                         GtkThemingEngine *engine,
                         int x, int y, int width, int height);

  void (*draw_list_view_header) (cairo_t *cr,
                                 GtkThemingEngine *engine,
                                 const ListViewHeaderParameters *header,
                                 int x, int y, int width, int height);

  void (*draw_menu_frame) (cairo_t *cr,
                           GtkThemingEngine *engine,
                           int x, int y, int width, int height,
                           int menustyle);

  void (*draw_menubar) (cairo_t *cr,
                        GtkThemingEngine *engine,
                        int x, int y, int width, int height,
                        int menubarstyle);

  void (*draw_menuitem) (cairo_t *cr,
                         GtkThemingEngine *engine,
                         int x, int y, int width, int height,
                         int menuitemstyle);

  void (*draw_progressbar_trough) (cairo_t *cr,
                                   GtkThemingEngine *engine,
                                   const ProgressBarParameters *progressbar,
                                   int x, int y, int width, int height);

  void (*draw_progressbar_fill) (cairo_t *cr,
                                 GtkThemingEngine *engine,
                                 const ProgressBarParameters *progressbar,
                                 int x, int y, int width, int height,
                                 gint offset);

  void (*draw_radiobutton) (cairo_t *cr,
                            GtkThemingEngine *engine,
                            const CheckboxParameters *checkbox,
                            int x, int y, int width, int height,
                            double trans);

  void (*draw_resize_grip) (cairo_t *cr,
                            GtkThemingEngine *engine,
                            const ResizeGripParameters *grip,
                            int x, int y, int width, int height);

  void (*draw_scale_trough) (cairo_t *cr,
                             GtkThemingEngine *engine,
                             const SliderParameters *slider,
                             int x, int y, int width, int height);

  void (*draw_scrollbar_stepper) (cairo_t *cr,
                                  GtkThemingEngine *engine,
                                  const ScrollBarParameters        *scrollbar,
                                  //const ScrollBarStepperParameters *stepper,
                                  int x, int y, int width, int height);

  void (*draw_scrollbar_slider) (cairo_t *cr,
                                 GtkThemingEngine *engine,
                                 const ScrollBarParameters *scrollbar,
                                 int x, int y, int width, int height);

  void (*draw_scrollbar_trough) (cairo_t *cr,
                                 GtkThemingEngine *engine,
                                 const ScrollBarParameters *scrollbar,
                                 int x, int y, int width, int height);

  void (*draw_selected_cell) (cairo_t *cr,
                              GtkThemingEngine *engine,
                              const CellParameters   *cell,
                              int x, int y, int width, int height);
  void (*draw_separator) (cairo_t *cr,
                          GtkThemingEngine *engine,
                          const SeparatorParameters *separator,
                          int x, int y, int width, int height);
#endif

  void (*draw_slider_button) (cairo_t *cr,
                              GtkThemingEngine *engine,
                              int x,
                              int y,
                              int width,
                              int height,
                              UnicoSliderParameters *slider);
#if 0
  void (*draw_slider_handle) (cairo_t *cr,
                              GtkThemingEngine *engine,
                              const HandleParameters *handle,
                              int x, int y, int width, int height,
                              boolean horizontal);

  void (*draw_spinbutton) (cairo_t *cr,
                           GtkThemingEngine *engine,
                           const SpinbuttonParameters *spinbutton,
                           int x, int y, int width, int height,
                           boolean horizontal);

  void (*draw_spinbutton_down) (cairo_t *cr,
                                GtkThemingEngine *engine,
                                int x, int y, int width, int height);

  void (*draw_statusbar) (cairo_t *cr,
                          GtkThemingEngine *engine,
                          int x, int y, int width, int height);
#endif
  void (*draw_tab) (cairo_t *cr,
                    GtkThemingEngine *engine,
                    int x,
                    int y,
                    int width,
                    int height,
                    UnicoTabParameters *tab);
#if 0
  void (*draw_toolbar) (cairo_t *cr,
                        GtkThemingEngine *engine,
                        const ToolbarParameters *toolbar,
                        int x, int y, int width, int height);

  void (*draw_tooltip) (cairo_t *cr,
                        GtkThemingEngine *engine,
                        int x, int y, int width, int height);
#endif
};

G_END_DECLS

#endif /* UNICO_TYPES_H */
