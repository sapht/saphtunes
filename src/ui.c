#include "main.h"
#include "album.h"
#include "song.h"
#include "ui.h"
#include "ui_common.h"
#include "ui_page1.h"
#include "ui_page2.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *
ui_create_window()
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_title(GTK_WINDOW(window), "GtkFrame");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    return window;
}

GtkWidget *
ui_create_notebook()
{
    /*  Display the main window */
    GtkWidget *notebook = gtk_notebook_new();
    GtkWidget *label;
    
    label = gtk_widget_new(GTK_TYPE_LABEL, "label", "Stuffs", "xalign", 0.0, NULL);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), 
                             ui_page_1_create(), 
                             label);

    label = gtk_widget_new(GTK_TYPE_LABEL, "label", "More Stuffs", "xalign", 0.0, NULL);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), 
                             ui_page_2_create(), 
                             label);

    return notebook;
}

int ui_main(int *argc, char ***argv)
{
    gtk_init(argc, argv);
    GtkWidget *window = ui_create_window();
    GtkWidget *notebook = ui_create_notebook();

    gtk_container_add(GTK_CONTAINER(window), notebook);

    g_signal_connect_swapped(
            G_OBJECT(window), 
            "destroy",
            G_CALLBACK(gtk_main_quit), 
            G_OBJECT(window)
    );

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
