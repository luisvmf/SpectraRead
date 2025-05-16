/* $Id: gtkdatabox_grid.h 4 2008-06-22 09:19:11Z rbock $ */
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

/**
 * SECTION:gtkdatabox_grid
 * @short_description: A #GtkDataboxGraph used for displaying a grid (like in an oscilloscope).
 * @include: gtkdatabox_grid.h
 * @see_also: #GtkDatabox, #GtkDataboxGraph, #GtkDataboxPoints, #GtkDataboxLines,  #GtkDataboxBars, #GtkDataboxMarkers
 *
 * #GtkDataboxGrid is a #GtkDataboxGraph class for displaying a grid (like in an oscilloscope). You can determine
 * the number of horizontal and vertical lines. When you zoom in, the grid is also zoomed along with the data.
 *
 */

#ifndef __GTK_DATABOX_GRID_H__
#define __GTK_DATABOX_GRID_H__

#include <gtkdatabox_graph.h>

G_BEGIN_DECLS
#define GTK_DATABOX_TYPE_GRID		  (gtk_databox_grid_get_type ())
#define GTK_DATABOX_GRID(obj)		  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                           GTK_DATABOX_TYPE_GRID, \
                                           GtkDataboxGrid))
#define GTK_DATABOX_GRID_CLASS(klass)	  (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                           GTK_DATABOX_TYPE_GRID, \
                                           GtkDataboxGridClass))
#define GTK_DATABOX_IS_GRID(obj)	  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                           GTK_DATABOX_TYPE_GRID))
#define GTK_DATABOX_IS_GRID_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                           GTK_DATABOX_TYPE_GRID))
#define GTK_DATABOX_GRID_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                           GTK_DATABOX_TYPE_GRID, \
                                           GtkDataboxGridClass))
#define GTK_DATABOX_GRID_GET_PRIVATE(obj) \
	G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_DATABOX_TYPE_GRID, \
	GtkDataboxGridPrivate)

/**
 * GtkDataboxGrid
 *
 * #GtkDataboxGrid is a #GtkDataboxGraph class for displaying a grid (like in an oscilloscope). You can determine
 * the number of horizontal and vertical lines. When you zoom in, the grid is also zoomed along with the data.
 *
 */

   typedef struct _GtkDataboxGrid GtkDataboxGrid;

   typedef struct _GtkDataboxGridClass GtkDataboxGridClass;

   typedef enum
     {
       GTK_DATABOX_GRID_DASHED_LINES = 0,   /* Grid drawn with dashed lines */
       GTK_DATABOX_GRID_SOLID_LINES,        /* Grid drawn with solid lines */
       GTK_DATABOX_GRID_DOTTED_LINES        /* Grid drawn with dotted lines */
     }
     GtkDataboxGridLineStyle;

   struct _GtkDataboxGrid
   {
      GtkDataboxGraph parent;
   };

   struct _GtkDataboxGridClass
   {
      GtkDataboxGraphClass parent_class;
   };

   GType gtk_databox_grid_get_type (void);

   GtkDataboxGraph *gtk_databox_grid_new (gint hlines, gint vlines,
					  GdkRGBA * color, guint size);
   GtkDataboxGraph *gtk_databox_grid_array_new (gint hlines, gint vlines, gfloat *hline_vals, gfloat *vline_vals,
					  GdkRGBA * color, guint size);

   void gtk_databox_grid_set_hlines (GtkDataboxGrid * grid, gint hlines);
   gint gtk_databox_grid_get_hlines (GtkDataboxGrid * grid);

   void gtk_databox_grid_set_vlines (GtkDataboxGrid * grid, gint vlines);
   gint gtk_databox_grid_get_vlines (GtkDataboxGrid * grid);

   void gtk_databox_grid_set_hline_vals (GtkDataboxGrid * grid, gfloat* hline_vals);
   gfloat* gtk_databox_grid_get_hline_vals (GtkDataboxGrid * grid);

   void gtk_databox_grid_set_vline_vals (GtkDataboxGrid * grid, gfloat* vline_vals);
   gfloat* gtk_databox_grid_get_vline_vals (GtkDataboxGrid * grid);

   void gtk_databox_grid_set_line_style (GtkDataboxGrid *grid, gint line_style);
   gint gtk_databox_grid_get_line_style (GtkDataboxGrid *grid);

G_END_DECLS
#endif				/* __GTK_DATABOX_GRID_H__ */
