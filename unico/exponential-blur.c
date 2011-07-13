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

/* FIXME: not working yet, unfinished */

#include <math.h>

#include "exponential-blur.h"

static inline void
_blurinner (guchar* pixel,
            gint   *zR,
            gint   *zG,
            gint   *zB,
            gint   *zA,
            gint    alpha,
            gint    aprec,
            gint    zprec);

static inline void
_blurrow (guchar* pixels,
          gint    width,
          gint    height,
          gint    channels,
          gint    line,
          gint    alpha,
          gint    aprec,
          gint    zprec);

static inline void
_blurcol (guchar* pixels,
          gint    width,
          gint    height,
          gint    channels,
          gint    col,
          gint    alpha,
          gint    aprec,
          gint    zprec);

void
_expblur (guchar* pixels,
          gint    width,
          gint    height,
          gint    channels,
          gint    radius,
          gint    aprec,
          gint    zprec);

void
surface_exponential_blur (cairo_surface_t* surface,
                          guint            radius)
{
  guchar*        pixels;
  guint          width;
  guint          height;
  cairo_format_t format;

  /* sanity checks are done in raico-blur.c */

  /* before we mess with the surface execute any pending drawing */
  cairo_surface_flush (surface);

  pixels = cairo_image_surface_get_data (surface);
  width  = cairo_image_surface_get_width (surface);
  height = cairo_image_surface_get_height (surface);
  format = cairo_image_surface_get_format (surface);

  switch (format)
  {
    case CAIRO_FORMAT_ARGB32:
      _expblur (pixels, width, height, 4, radius, 16, 7);
      break;
    case CAIRO_FORMAT_RGB24:
      _expblur (pixels, width, height, 3, radius, 16, 7);
      break;
    case CAIRO_FORMAT_A8:
      _expblur (pixels, width, height, 1, radius, 16, 7);
      break;
    default:
      break;
  }

  /* inform cairo we altered the surfaces contents */
  cairo_surface_mark_dirty (surface);        
}

/*
 * pixels   image-data
 * width    image-width
 * height   image-height
 * channels image-channels
 *
 * in-place blur of image 'img' with kernel of approximate radius 'radius'
 *
 * blurs with two sided exponential impulse response
 *
 * aprec = precision of alpha parameter in fixed-point format 0.aprec
 *
 * zprec = precision of state parameters zR,zG,zB and zA in fp format 8.zprec
 */
void
_expblur (guchar* pixels,
          gint    width,
          gint    height,
          gint    channels,
          gint    radius,
          gint    aprec,
          gint    zprec)
{
  gint alpha;
  gint row = 0;
  gint col = 0;

  if (radius < 1)
    return;

  /* calculate the alpha such that 90% of 
   * the kernel is within the radius.
   * (Kernel extends to infinity) */
  alpha = (gint) ((1 << aprec) * (1.0f - expf (-2.3f / (radius + 1.f))));

  for (; row < height; row++)
    _blurrow (pixels,
              width,
              height,
              channels,
              row,
              alpha,
              aprec,
              zprec);

  for(; col < width; col++)
    _blurcol (pixels,
              width,
              height,
              channels,
              col,
              alpha,
              aprec,
              zprec);

  return;
}

static inline void
_blurinner (guchar* pixel,
            gint   *zR,
            gint   *zG,
            gint   *zB,
            gint   *zA,
            gint    alpha,
            gint    aprec,
            gint    zprec)
{
  gint R;
  gint G;
  gint B;
  guchar A;

  R = *pixel;
  G = *(pixel + 1);
  B = *(pixel + 2);
  A = *(pixel + 3);

  *zR += (alpha * ((R << zprec) - *zR)) >> aprec;
  *zG += (alpha * ((G << zprec) - *zG)) >> aprec;
  *zB += (alpha * ((B << zprec) - *zB)) >> aprec;
  *zA += (alpha * ((A << zprec) - *zA)) >> aprec;

  *pixel       = *zR >> zprec;
  *(pixel + 1) = *zG >> zprec;
  *(pixel + 2) = *zB >> zprec;
  *(pixel + 3) = *zA >> zprec;
} 

static inline void
_blurrow (guchar* pixels,
          gint    width,
          gint    height,
          gint    channels,
          gint    line,
          gint    alpha,
          gint    aprec,
          gint    zprec)
{
  gint    zR;
  gint    zG;
  gint    zB;
  gint    zA;
  gint    index;
  guchar* scanline;

  scanline = &(pixels[line * width * channels]);

  zR = *scanline << zprec;
  zG = *(scanline + 1) << zprec;
  zB = *(scanline + 2) << zprec;
  zA = *(scanline + 3) << zprec;

  for (index = 0; index < width; index ++)
    _blurinner (&scanline[index * channels],
                &zR,
                &zG,
                &zB,
                &zA,
                alpha,
                aprec,
                zprec);

  for (index = width - 2; index >= 0; index--)
    _blurinner (&scanline[index * channels],
                &zR,
                &zG,
                &zB,
                &zA,
                alpha,
                aprec,
                zprec);
}

static inline void
_blurcol (guchar* pixels,
          gint    width,
          gint    height,
          gint    channels,
          gint    x,
          gint    alpha,
          gint    aprec,
          gint    zprec)
{
  gint zR;
  gint zG;
  gint zB;
  gint zA;
  gint index;
  guchar* ptr;

  ptr = pixels;
  
  ptr += x * channels;

  zR = *((guchar*) ptr    ) << zprec;
  zG = *((guchar*) ptr + 1) << zprec;
  zB = *((guchar*) ptr + 2) << zprec;
  zA = *((guchar*) ptr + 3) << zprec;

  for (index = width; index < (height - 1) * width; index += width)
    _blurinner ((guchar*) &ptr[index * channels],
                &zR,
                &zG,
                &zB,
                &zA,
                alpha,
                aprec,
                zprec);

  for (index = (height - 2) * width; index >= 0; index -= width)
    _blurinner ((guchar*) &ptr[index * channels],
                &zR,
                &zG,
                &zB,
                &zA,
                alpha,
                aprec,
                zprec);
}
