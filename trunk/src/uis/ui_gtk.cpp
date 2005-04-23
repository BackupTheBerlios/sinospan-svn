/* Copyright (C) 2005 Ben Anderson
   Licensed under the Open Software License version 2.1 */
#include "ui_gtk.h"

#include <map>

#include <gtk/gtk.h>

#include "cell.h"
#include "itc.h"
#include "track.h"
#include "synth.h"

typedef std::map<Track*, GtkWidget*> pTrackAssoc_t;
pTrackAssoc_t pTrackAssoc;

struct UI_GTK_CB
{
	static void AddTrack(GtkWidget*,gpointer);
	static void RemoveTrack(GtkWidget*,gpointer);
	static void AddCell(GtkWidget*,gpointer);
	static void RemoveCell(GtkWidget*,gpointer);
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
		pTrackAssoc.insert(pTrackAssoc_t::value_type(trk, cellBox) );
		
		GtkWidget *addCellBut = gtk_button_new_with_label("Add Cell");
		g_signal_connect(G_OBJECT(addCellBut), "clicked",
			G_CALLBACK(UI_GTK_CB::AddCell), (gpointer) trk);
		gtk_box_pack_start(GTK_BOX(topBox), addCellBut, FALSE, TRUE,
									0);
		gtk_widget_show(addCellBut);
		
		GtkWidget *remTrackBut = gtk_button_new_with_label(
							"Remove Track");
		g_signal_connect(G_OBJECT(remTrackBut), "clicked",
			G_CALLBACK(UI_GTK_CB::RemoveTrack), (gpointer) trk);
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
	GtkWidget *topBox = gtk_widget_get_parent(w);
	gtk_widget_destroy(topBox);
	pTrackAssoc.erase(trk);
	return;
}

void UI_GTK_CB::AddCell(GtkWidget *w, gpointer ptrk)
{
	Cell *cel = new Cell;
	Track *trk = (Track*) ptrk;
	// To save ourselves a lot of work (and lessen the load on ITC), use
	// Cell::trk member to pass in the destined track.
	cel->trk = trk;
	ITC_DEFER(callback_0, ITC::OWNER_RENDER, pcel, (void*) cel,
	{
		Cell *cel = (Cell*) pcel;
		cel->trk->AddCell(cel);
	});
	GtkWidget *cellBox = pTrackAssoc.find(trk)->second;
	
	GtkWidget *topBox = gtk_vbox_new(FALSE, 0);
		
		GtkWidget *mdlPort = gtk_scrolled_window_new(0x0, 0x0);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(mdlPort),
				GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
			
			GtkWidget *mdlBox = gtk_vbox_new(FALSE, 0);
			gtk_scrolled_window_add_with_viewport(
					GTK_SCROLLED_WINDOW(mdlPort), mdlBox);
			gtk_widget_show(mdlBox);
			
		gtk_box_pack_start(GTK_BOX(topBox), mdlPort, TRUE, TRUE, 0);
		gtk_widget_show(mdlPort);
		
		// TODO: Add Module button
		
		GtkWidget *remCellBut = gtk_button_new_with_label(
								"Remove Cell");
		g_signal_connect(G_OBJECT(remCellBut), "clicked",
			G_CALLBACK(UI_GTK_CB::RemoveCell), (gpointer) cel);
		gtk_box_pack_start(GTK_BOX(topBox), remCellBut, FALSE, TRUE,
									0);
		gtk_widget_show(remCellBut);
		
	gtk_box_pack_start(GTK_BOX(cellBox), topBox, FALSE, TRUE, 0);
	gtk_widget_show(topBox);
}

void UI_GTK_CB::RemoveCell(GtkWidget *w, gpointer pcel)
{
	Cell *cel = (Cell*) pcel;
	ITC_DEFER(callback_0, ITC::OWNER_RENDER, pcel, (void*) cel,
	{
		Cell *cel = (Cell*) pcel;
		cel->trk->RemoveCell(cel);
	});
	// We're at the "remove track" button; Parent is the topBox.
	GtkWidget *topBox = gtk_widget_get_parent(w);
	gtk_widget_destroy(topBox);
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
