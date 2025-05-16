/* $Id: lissajous.c 4 2008-06-22 09:19:11Z rbock $ */
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
#include <stdio.h>

#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_points.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_util.h>
#include <math.h>

#define POINTS 2000

#define  FRAME_RATE        50

/*----------------------------------------------------------------
 *  databox lissajous
 *----------------------------------------------------------------*/

static gfloat *lissajousX = NULL;
static gfloat *lissajousY = NULL;

static gint lissajous_timeout_id = 0;
static gfloat lissajous_frequency = 3. * G_PI / 2.;
static GtkWidget *lissajous_label = NULL;
static guint lissajous_counter = 0;

static gboolean
lissajous_frame_draw_func (GtkDatabox * box)
{
   gfloat freq;
   gfloat off;
   gchar label[10];
   gint i;

   if (!GTK_IS_DATABOX (box))
      return FALSE;

   lissajous_frequency += 0.025;
   off = lissajous_counter * 4 * G_PI / POINTS;

   freq = 14 + 10 * sin (lissajous_frequency);
   for (i = 0; i < POINTS; i++)
   {
      lissajousX[i] = 100. * sin (i * 4 * G_PI / POINTS + off);
      lissajousY[i] = 100. * cos (i * freq * G_PI / POINTS + off);
   }


   gtk_widget_queue_draw (GTK_WIDGET (box));

   sprintf (label, "%d", lissajous_counter++);
   gtk_entry_set_text (GTK_ENTRY (lissajous_label), label);

   return TRUE;
}


int createbox(GtkWidget **box, GtkWidget **table){
   /* Create a GtkDatabox widget along with scrollbars and rulers */
   gtk_databox_create_box_with_scrollbars_and_rulers (box, table,
						      FALSE, FALSE, FALSE, FALSE);

   g_object_set(G_OBJECT(box), "expand", TRUE, NULL);

}



static void
create_lissajous (void)
{
   GtkWidget *window = NULL;
   GtkWidget *box1;
   GtkWidget *box2;
   GtkWidget *close_button;
   GtkWidget *box;
   GtkWidget *label;
   GtkWidget *table;
   GtkWidget *separator;
   GtkDataboxGraph *graph;
   GdkRGBA color;
   gint i;

   lissajous_frequency = 0;
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_widget_set_size_request (window, 500, 500);

   g_signal_connect (G_OBJECT (window), "destroy",
		     G_CALLBACK (gtk_main_quit), NULL);

   gtk_window_set_title (GTK_WINDOW (window),
			 "GtkDatabox: Lissajous Example");
   gtk_container_set_border_width (GTK_CONTAINER (window), 0);

   box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
   gtk_container_add (GTK_CONTAINER (window), box1);

   label =
      gtk_label_new
      ("This example resembles an oszilloscope\nreceiving two signals, one is a sine (horizontal),\nthe other is a cosine with ever changing frequency (vertical).\nThe counter is synchron with the updates.");
   gtk_box_pack_start (GTK_BOX (box1), label, FALSE, FALSE, 0);
   separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
   gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, FALSE, 0);
   lissajous_label = gtk_entry_new ();
   gtk_entry_set_text (GTK_ENTRY (lissajous_label), "0");
   gtk_box_pack_start (GTK_BOX (box1), lissajous_label, FALSE, FALSE, 0);
   separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
   gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, FALSE, 0);

   lissajous_timeout_id = 0;
   lissajous_frequency = 3. * G_PI / 2.;
   lissajous_counter = 0;

createbox(&box,&table);

   gtk_box_pack_start (GTK_BOX (box1), table, TRUE, TRUE, 0);

   color.red = 0;
   color.green = 0;
   color.blue = 0.5;
   color.alpha = 1;

   pgtk_widget_override_background_color (box, GTK_STATE_FLAG_NORMAL, &color);

   lissajousX = g_new0 (gfloat, POINTS);
   lissajousY = g_new0 (gfloat, POINTS);

   for (i = 0; i < POINTS; i++)
   {
      lissajousX[i] = 100. * sin (i * 4 * G_PI / POINTS);
      lissajousY[i] = 100. * cos (i * 4 * G_PI / POINTS);
   }
   color.red = 1;
   color.green = 1;
   color.blue = 0;
   color.alpha = 1;

   graph = gtk_databox_lines_new (POINTS, lissajousX, lissajousY, &color, 1);
//-----------------------------------------------------------------------------------
//aqui. ver se isto libera a memoria.
//-----------------------------------------------------------------------------------
	gtk_widget_destroy(box);
	gtk_widget_destroy(table);
	createbox(&box,&table);
  color.red = 0;
   color.green = 0;
   color.blue = 0.5;
   color.alpha = 1;
   pgtk_widget_override_background_color (box, GTK_STATE_FLAG_NORMAL, &color);
//-----------------------------------------------------------------------------------
   gtk_box_pack_start (GTK_BOX (box1), table, TRUE, TRUE, 0);
   gtk_databox_graph_add (GTK_DATABOX (box), graph);

   gtk_databox_auto_rescale (GTK_DATABOX (box), 0.05);

   separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
   gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 0);

   box2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
   gtk_box_pack_end (GTK_BOX (box1), box2, FALSE, TRUE, 0);
   close_button = gtk_button_new_with_label ("close");

   g_signal_connect_swapped (G_OBJECT (close_button), "clicked",
			     G_CALLBACK (gtk_main_quit), G_OBJECT (box));

   gtk_box_pack_start (GTK_BOX (box2), close_button, TRUE, TRUE, 0);
   gtk_widget_set_can_default(close_button, TRUE);
   gtk_widget_grab_default (close_button);

   //lissajous_timeout_id = g_timeout_add (1000 / FRAME_RATE,
          //     (GSourceFunc) lissajous_frame_draw_func, box);

   gtk_widget_show_all (window);
}

gint
main (gint argc, char *argv[])
{
   gtk_init (&argc, &argv);

   create_lissajous ();
   gtk_main ();

   return 0;
}
