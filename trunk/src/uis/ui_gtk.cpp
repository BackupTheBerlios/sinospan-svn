/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "ui_gtk.h"

#include <gtk/gtk.h>

#include "itc.h"
#include "track.h"
#include "synth.h"

struct UI_GTK_CB
{
	static void AddTrack(GtkWidget*,gpointer);
	static void RemoveTrack(GtkWidget*,gpointer);
	static gboolean Delete(GtkWidget*,GdkEvent*,gpointer);
	static void DestroyMain(GtkWidget*,gpointer);
};

UI_GTK::UI_GTK()
{
	return;
}

UI_GTK::~UI_GTK()
{
	return;
}

bool UI_GTK::Go(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	// Begin main window
	GtkWidget *mainWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(mainWin), "Sinospan");
	g_signal_connect(G_OBJECT(mainWin), "delete_event",
					G_CALLBACK(UI_GTK_CB::Delete), 0x0);
	g_signal_connect(G_OBJECT(mainWin), "destroy",
					G_CALLBACK(UI_GTK_CB::DestroyMain),
									0x0);
		
		GtkWidget *topBox = gtk_vbox_new(FALSE, 0);
			
			GtkWidget *trackBook = gtk_notebook_new();
			gtk_box_pack_start(GTK_BOX(topBox), trackBook, TRUE,
								TRUE, 0);
			gtk_widget_show(trackBook);
			
			GtkWidget *addTrackBut = gtk_button_new_with_label(
								"Add Track");
			g_signal_connect(G_OBJECT(addTrackBut), "clicked",
					G_CALLBACK(UI_GTK_CB::AddTrack),
							(gpointer) trackBook);
			gtk_box_pack_start(GTK_BOX(topBox), addTrackBut, FALSE,
								TRUE, 0);
			gtk_widget_show(addTrackBut);
			
		gtk_container_add(GTK_CONTAINER(mainWin), topBox);
		gtk_widget_show(topBox);
		
	gtk_widget_show(mainWin);
	return true;
}

bool UI_GTK::Stop()
{
	gtk_main_quit();
	return true;
}

void UI_GTK::Loop()
{
	gtk_main();
}

void UI_GTK_CB::AddTrack(GtkWidget *w, gpointer trackBook)
{
	Track *trk = new Track;
	ITC_DEFER(callback_0, ITC::OWNER_RENDER, trk, (void*) trk,
	{
		Synth::AddTrack( (Track*) trk);
	});
	GtkWidget *topBox = gtk_vbox_new(FALSE, 0);
		
		GtkWidget *cellPort = gtk_scrolled_window_new(0x0, 0x0);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(cellPort),
				GTK_POLICY_ALWAYS, GTK_POLICY_AUTOMATIC);
			
			GtkWidget *cellBox = gtk_hbox_new(FALSE, 0);
			gtk_scrolled_window_add_with_viewport(
				GTK_SCROLLED_WINDOW(cellPort), cellBox);
			gtk_widget_show(cellBox);
			
		gtk_box_pack_start(GTK_BOX(topBox), cellPort, TRUE, TRUE, 0);
		gtk_widget_show(cellPort);
		
		// TODO: Add Cell button
		
		GtkWidget *remTrackBut = gtk_button_new_with_label(
							"Remove Track");
		g_signal_connect(G_OBJECT(remTrackBut), "clicked",
					G_CALLBACK(UI_GTK_CB::RemoveTrack),
							(gpointer) trk);
		gtk_box_pack_start(GTK_BOX(topBox), remTrackBut, FALSE, TRUE,
									0);
		gtk_widget_show(remTrackBut);
		
	GtkWidget *label = gtk_label_new("Track");
	gtk_notebook_append_page(GTK_NOTEBOOK(trackBook), topBox, label);
	gtk_widget_show(topBox);
	gtk_widget_show(label);
	return;
}

void UI_GTK_CB::RemoveTrack(GtkWidget *w, gpointer ptrk)
{
	Track *trk = (Track*) ptrk;
	ITC_DEFER(callback_0, ITC::OWNER_RENDER, trk, (void*) trk,
	{
		Synth::RemoveTrack( (Track*) trk);
	});
	// w is the remove track button. Parent for the toplevel container.
	GtkWidget *twd = gtk_widget_get_parent(w);
	gtk_widget_destroy(twd);
	return;
}

gboolean UI_GTK_CB::Delete(GtkWidget *w, GdkEvent *e, gpointer nul)
{
	gtk_main_quit();
	return FALSE;
}

void UI_GTK_CB::DestroyMain(GtkWidget *w, gpointer nul)
{
	gtk_main_quit();
}
