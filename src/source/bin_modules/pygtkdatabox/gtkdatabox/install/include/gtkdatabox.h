/* $Id: gtkdatabox.h 3 2008-06-22 08:45:45Z rbock $ */
/* GtkDatabox - An extension to the gtk+ library
 * Copyright (C) 1998 - 2008  Dr. Roland Bock
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __GTK_DATABOX_H__
#define __GTK_DATABOX_H__

#include <gtkdatabox_typedefs.h>
#include <gtkdatabox_graph.h>
#include <gtkdatabox_ruler.h>
#include <gtk/gtk.h>

/**
 * SECTION:gtkdatabox
 * @short_description: A GTK+ widget to display large amounts of numerical data quickly and easily.
 * @include: gtkdatabox.h
 * @see_also: #GtkDataboxGraph, #GtkDataboxPoints, #GtkDataboxLines, #GtkDataboxMarkers, #GtkDataboxGrid
 *
 * #GtkDatabox is a widget for the GTK+ library designed to display
 * large amounts of numerical data quickly and easily. It allows
 * for one or more data sets of thousands of data points (X and Y coordinates)
 * to be displayed and updated in split seconds.
 *
 * It offers the ability to zoom into and out of the data, and to navigate
 * through your data by scrolling.
 *
 * In addition to rulers and a simple
 * coordinate cross, it allows you to add one (or even more) configurable grids
 * like on an oscilloscope.
 *
 * Data may be presented as dots, lines connecting the data,
 * or vertical bars. The widget allows you to easily transform pixel
 * coordinates into data coordinates, thus allowing you to easily create
 * powerful applications for data analysis.
 *
 **/

G_BEGIN_DECLS
#define GTK_TYPE_DATABOX		  (gtk_databox_get_type ())
#define GTK_DATABOX(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                           GTK_TYPE_DATABOX, \
                                           GtkDatabox))
#define GTK_DATABOX_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                           GTK_TYPE_DATABOX, \
                                           GtkDataboxClass))
#define GTK_IS_DATABOX(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                           GTK_TYPE_DATABOX))
#define GTK_IS_DATABOX_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                           GTK_TYPE_DATABOX))
#define GTK_DATABOX_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                           GTK_TYPE_DATABOX, \
                                           GtkDataboxClass))
#define GTK_DATABOX_GET_PRIVATE(obj) \
	G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_DATABOX, GtkDataboxPrivate)

typedef struct _GtkDataboxClass GtkDataboxClass;

typedef struct
{
    gfloat x1;
    gfloat x2;
    gfloat y1;
    gfloat y2;
} GtkDataboxValueRectangle;

/**
 * _GtkDatabox
 * @box: The parent object
 *
 * Implementation of #GtkDatabox.
 *
 **/
struct _GtkDatabox
{
    /*< private >*/
    GtkWidget box;
};

struct _GtkDataboxClass
{
    GtkWidgetClass parent_class;

    void (*zoomed) (GtkDatabox * box);
    void (*selection_started) (GtkDatabox * box,
                               GtkDataboxValueRectangle * selectionValues);
    void (*selection_changed) (GtkDatabox * box,
                               GtkDataboxValueRectangle * selectionValues);
    void (*selection_finalized) (GtkDatabox * box,
                                 GtkDataboxValueRectangle *
                                 selectionValues);
    void (*selection_canceled) (GtkDatabox * box);
};

GType gtk_databox_get_type (void);

GtkWidget *gtk_databox_new (void);

GList * gtk_databox_get_graphs (GtkDatabox *);

gint gtk_databox_graph_add (GtkDatabox * box, GtkDataboxGraph * graph);
gint gtk_databox_graph_add_front (GtkDatabox * box, GtkDataboxGraph * graph);

gint gtk_databox_graph_remove (GtkDatabox * box, GtkDataboxGraph * graph);
gint gtk_databox_graph_remove_all (GtkDatabox * box);

gint gtk_databox_auto_rescale (GtkDatabox * box, gfloat border);
gint gtk_databox_calculate_extrema (GtkDatabox * box,
                                    gfloat * min_x, gfloat * max_x,
                                    gfloat * min_y, gfloat * max_y);

void gtk_databox_set_total_limits (GtkDatabox * box,
                                   gfloat left, gfloat right,
                                   gfloat top, gfloat bottom);
void gtk_databox_set_visible_limits (GtkDatabox * box,
                                     gfloat left, gfloat right,
                                     gfloat top, gfloat bottom);
void gtk_databox_get_total_limits (GtkDatabox * box,
                                   gfloat * left, gfloat * right,
                                   gfloat * top, gfloat * bottom);
void gtk_databox_get_visible_limits (GtkDatabox * box,
                                     gfloat * left, gfloat * right,
                                     gfloat * top, gfloat * bottom);

void gtk_databox_set_adjustment_x (GtkDatabox * box, GtkAdjustment * adj);
void gtk_databox_set_adjustment_y (GtkDatabox * box, GtkAdjustment * adj);
GtkAdjustment *gtk_databox_get_adjustment_x (GtkDatabox * box);
GtkAdjustment *gtk_databox_get_adjustment_y (GtkDatabox * box);

void gtk_databox_set_ruler_x (GtkDatabox * box, GtkDataboxRuler * ruler);
void gtk_databox_set_ruler_y (GtkDatabox * box, GtkDataboxRuler * ruler);
GtkDataboxRuler *gtk_databox_get_ruler_x (GtkDatabox * box);
GtkDataboxRuler *gtk_databox_get_ruler_y (GtkDatabox * box);

void gtk_databox_set_scale_type_x (GtkDatabox * box,
                                   GtkDataboxScaleType scale_type);
void gtk_databox_set_scale_type_y (GtkDatabox * box,
                                   GtkDataboxScaleType scale_type);
GtkDataboxScaleType gtk_databox_get_scale_type_x (GtkDatabox * box);
GtkDataboxScaleType gtk_databox_get_scale_type_y (GtkDatabox * box);

void gtk_databox_set_enable_selection (GtkDatabox * box, gboolean enable);
void gtk_databox_set_enable_zoom (GtkDatabox * box, gboolean enable);
void gtk_databox_set_box_shadow(GtkDatabox * box, GtkShadowType which_shadow);

gboolean gtk_databox_get_enable_selection (GtkDatabox * box);
gboolean gtk_databox_get_enable_zoom (GtkDatabox * box);
GtkShadowType gtk_databox__get_box_shadow(GtkDatabox * box);

void gtk_databox_zoom_to_selection (GtkDatabox * box);
void gtk_databox_zoom_out (GtkDatabox * box);
void gtk_databox_zoom_home (GtkDatabox * box);

gint16 gtk_databox_value_to_pixel_x (GtkDatabox * box, gfloat value);
gint16 gtk_databox_value_to_pixel_y (GtkDatabox * box, gfloat value);
gfloat gtk_databox_pixel_to_value_x (GtkDatabox * box, gint16 pixel);
gfloat gtk_databox_pixel_to_value_y (GtkDatabox * box, gint16 pixel);
void gtk_databox_values_to_xpixels (GtkDatabox *box, gint16 *pixels,
	void *values, GType vtype, guint size, guint start, guint stride, guint len);
void gtk_databox_values_to_ypixels (GtkDatabox *box, gint16 *pixels,
	void *values, GType vtype, guint size, guint start, guint stride, guint len);

void gtk_databox_create_box_with_scrollbars_and_rulers (GtkWidget **
        p_box,
        GtkWidget **
        p_table,
        gboolean scrollbar_x,
        gboolean scrollbar_y,
        gboolean ruler_x,
        gboolean ruler_y);

void
gtk_databox_create_box_with_scrollbars_and_rulers_positioned (GtkWidget ** p_box,
        GtkWidget ** p_table,
        gboolean scrollbar_x,
        gboolean scrollbar_y,
        gboolean ruler_x,
        gboolean ruler_y,
        gboolean ruler_x_top,
        gboolean ruler_y_left);

/* Used by graph objects */
cairo_surface_t* gtk_databox_get_backing_surface(GtkDatabox * box);

G_END_DECLS
#endif				/* __GTK_DATABOX_H__ */
