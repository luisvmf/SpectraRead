/* $Id: gtkdatabox_ruler.h 4 2008-06-22 09:19:11Z rbock $ */
/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301 USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/.
 */

/*
 * Modified by Roland Bock 2007 - 2008.
 */

/**
 * SECTION:gtkdatabox_ruler
 * @short_description: An improved version of the #GtkRuler.
 * @include: gtkdatabox_ruler.h
 * @see_also: #GtkDatabox
 *
 * #GtkDataboxRuler is a widget for the GTK+ library similar to GtkRuler.
 *
 * It is improved in several ways:
 *
 * <itemizedlist>
 *    <listitem>
 *       <para>
 *          It supports linear and logarithmic scales.
 *       </para>
 *    </listitem>
 *    <listitem>
 *       <para>
 *           In the vertical orientation, the labels are rotated 90° (instead of being written
 *           as a vertical column of horizontal characters). This increases readability.
 *       </para>
 *    </listitem>
 *    <listitem>
 *       <para>
 *           It works for very small and very large value ranges.
 *       </para>
 *    </listitem>
 * </itemizedlist>
 **/
#ifndef __GTK_DATABOX_RULER_H__
#define __GTK_DATABOX_RULER_H__

#ifdef _MSC_VER
#define log2(x) (log(x)/log(2))
#endif

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gtkdatabox_scale.h>

G_BEGIN_DECLS
#define GTK_DATABOX_TYPE_RULER            (gtk_databox_ruler_get_type ())
#define GTK_DATABOX_RULER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_DATABOX_TYPE_RULER, GtkDataboxRuler))
#define GTK_DATABOX_RULER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_DATABOX_TYPE_RULER, GtkDataboxRulerClass))
#define GTK_DATABOX_IS_RULER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_DATABOX_TYPE_RULER))
#define GTK_DATABOX_IS_RULER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_DATABOX_TYPE_RULER))
#define GTK_DATABOX_RULER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_DATABOX_TYPE_RULER, GtkDataboxRulerClass))
/**
 * GtkDataboxRuler:
 *
 * An improved version (see above) of GtkRuler which supports linear and logarithmic scales.
 *
 **/
typedef struct _GtkDataboxRuler GtkDataboxRuler;
typedef struct _GtkDataboxRulerClass GtkDataboxRulerClass;

/**
 * GTK_DATABOX_RULER_MAX_MAX_LENGTH
 *
 * The maximum upper limit of label length in characters
 */
#define GTK_DATABOX_RULER_MAX_MAX_LENGTH 63

/**
 * GtkDataboxRulerPrivate
 *
 * A private data structure used by the #GtkDataboxRuler. It shields all internal things
 * from developers who are just using the object.
 *
 **/
 typedef struct _GtkDataboxRulerPrivate GtkDataboxRulerPrivate;

struct _GtkDataboxRuler
{
   GtkWidget widget;

   GtkDataboxRulerPrivate *priv;
};

struct _GtkDataboxRulerClass
{
   GtkWidgetClass parent_class;
};


GType
gtk_databox_ruler_get_type (void)
   G_GNUC_CONST;
     GtkWidget *gtk_databox_ruler_new (GtkOrientation orientation);

     void gtk_databox_ruler_set_range (GtkDataboxRuler * ruler,
				       gdouble lower,
				       gdouble upper, gdouble position);
     void gtk_databox_ruler_set_max_length (GtkDataboxRuler * ruler,
					    guint max_length);
     void gtk_databox_ruler_set_scale_type (GtkDataboxRuler * ruler,
					    guint scale_type);

     void gtk_databox_ruler_get_range (GtkDataboxRuler * ruler,
				       gdouble * lower,
				       gdouble * upper, gdouble * position);
     guint gtk_databox_ruler_get_max_length (GtkDataboxRuler * ruler);
     GtkDataboxScaleType gtk_databox_ruler_get_scale_type (GtkDataboxRuler *
							   ruler);

     void gtk_databox_ruler_set_orientation (GtkDataboxRuler * ruler, GtkOrientation orientation);
     GtkOrientation gtk_databox_ruler_get_orientation (GtkDataboxRuler *ruler);

     void gtk_databox_ruler_set_text_orientation (GtkDataboxRuler * ruler, GtkOrientation orientation);
     GtkOrientation gtk_databox_ruler_get_text_orientation (GtkDataboxRuler *ruler);

     void gtk_databox_ruler_set_text_alignment (GtkDataboxRuler * ruler, PangoAlignment alignment);
     PangoAlignment gtk_databox_ruler_get_text_alignment (GtkDataboxRuler * ruler);

    void gtk_databox_ruler_set_text_hoffset (GtkDataboxRuler * ruler,gint offset);
    gint gtk_databox_ruler_get_text_hoffset (GtkDataboxRuler * ruler);

     void gtk_databox_ruler_set_draw_ticks(GtkDataboxRuler * ruler, gboolean draw);
    gboolean gtk_databox_ruler_get_draw_ticks(GtkDataboxRuler * ruler);

     void gtk_databox_ruler_set_draw_position(GtkDataboxRuler * ruler, gboolean draw);
    gboolean gtk_databox_ruler_get_draw_position(GtkDataboxRuler * ruler);

     void gtk_databox_ruler_set_draw_subticks(GtkDataboxRuler * ruler, gboolean draw);
    gboolean gtk_databox_ruler_get_draw_subticks(GtkDataboxRuler * ruler);

     void gtk_databox_ruler_set_invert_edge(GtkDataboxRuler * ruler, gboolean invert);
    gboolean gtk_databox_ruler_get_invert_edge(GtkDataboxRuler * ruler);

    void gtk_databox_ruler_set_linear_label_format(GtkDataboxRuler * ruler, gchar *format);
    void gtk_databox_ruler_set_log_label_format(GtkDataboxRuler * ruler, gchar *format);

    gchar* gtk_databox_ruler_get_linear_label_format(GtkDataboxRuler * ruler);
    gchar* gtk_databox_ruler_get_log_label_format(GtkDataboxRuler * ruler);

    void gtk_databox_ruler_set_manual_ticks(GtkDataboxRuler * ruler, gfloat* manual_ticks);
    gfloat* gtk_databox_ruler_get_manual_ticks(GtkDataboxRuler * ruler);

    void gtk_databox_ruler_set_manual_tick_cnt(GtkDataboxRuler * ruler, guint manual_tick_cnt);
    guint gtk_databox_ruler_get_manual_tick_cnt(GtkDataboxRuler * ruler);

    void gtk_databox_ruler_set_manual_tick_labels(GtkDataboxRuler * ruler, gchar *labels[]);
    gchar ** gtk_databox_ruler_get_manual_tick_labels(GtkDataboxRuler * ruler);

    void gtk_databox_ruler_set_box_shadow(GtkDataboxRuler * ruler, GtkShadowType which_shadow);
    GtkShadowType gtk_databox_ruler_get_box_shadow(GtkDataboxRuler * ruler);

G_END_DECLS
#endif /* __GTK_DATABOX_RULER_H__ */
