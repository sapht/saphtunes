#include "main.h"
#include "album.h"
#include "song.h"
#include "gtk.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

void 
ui_album_select(GtkTreeView *album_list_view, 
                    GtkTreePath *item_path, 
                    GtkTreeViewColumn *column,
                    gpointer song_list
                    )
{

    GtkTreeIter iter;
    GtkTreeModel *album_model = gtk_tree_view_get_model(album_list_view);
    int found = gtk_tree_model_get_iter(album_model, &iter, item_path);
    printf("%d\n", found);


    int album_offset;
    gint num;
    gtk_tree_model_get(album_model, &iter, 
        0, &album_offset,
        -1);

    struct album *album = cm.albums->e[album_offset];

    g_print("%s [%d]\n", album->slug, num);

    GtkTreeIter song_iter;
    gtk_list_store_clear(song_list);
    for (int i=0; i<album->songs.len; i++) {
        gtk_list_store_append(GTK_LIST_STORE(song_list), &song_iter);
        gtk_list_store_set(GTK_LIST_STORE(song_list),
                           &song_iter,
                           0, album->songs.e[i]->slug,
                           -1);
    }
}

int ui_main(int *argc, char ***argv)
{
    GtkWidget *window;
    GtkWidget *table;

    GtkWidget *frame2;
    GtkWidget *frame3;

    gtk_init(argc, argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 800);
    gtk_window_set_title(GTK_WINDOW(window), "GtkFrame");

    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    table = gtk_table_new(2, 2, TRUE);
    gtk_table_set_row_spacings(GTK_TABLE(table), 10);
    gtk_table_set_col_spacings(GTK_TABLE(table), 10);
    gtk_container_add(GTK_CONTAINER(window), table);


    frame2 = gtk_frame_new("About selection");
    frame3 = gtk_frame_new("Song list");


    /***************************************
     *Creating album list widget           *
     ***************************************/
    GtkListStore *album_list = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
    GtkTreeIter album_iter;

    for(int i=0; i<cm.albums->len; i++) {
        struct album *album_entry = cm.albums->e[i];

        gtk_list_store_append(album_list, &album_iter);
        gtk_list_store_set(album_list, &album_iter,
                0, (gint) i,
                1, album_entry->slug,
                2, album_entry->songs.len,
                -1);
    }

    GtkWidget *album_list_view = gtk_tree_view_new();
    GtkCellRenderer *cell_renderer  = gtk_cell_renderer_text_new();

    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(album_list_view),
        0, "Title", cell_renderer,
        "text", 1,
        NULL);

    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(album_list_view),
        1, "song#", cell_renderer,
        "text", 2,
        NULL);

    gtk_tree_view_set_model(GTK_TREE_VIEW(album_list_view), GTK_TREE_MODEL(album_list));
    gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(album_list_view), 1);

    /***************************************** 
     * Song list view                        *
     *****************************************/
    GtkListStore *song_list = gtk_list_store_new(1, G_TYPE_STRING);
    /*GtkTreeIter song_iter;*/

    GtkWidget *song_list_view = gtk_tree_view_new();
    GtkCellRenderer *song_name_renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *song_name_column = gtk_tree_view_column_new_with_attributes("Title", song_name_renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(song_list_view), song_name_column);


    gtk_tree_view_set_model(GTK_TREE_VIEW(song_list_view), GTK_TREE_MODEL(song_list));



    /***************************************
     *Attaching all the stuffs             *
     ***************************************/

    /*GtkObject *scroll_a = gtk_adjustment_new(10, 10, 100, 1, 1, 1);*/
    GtkWidget *album_scroll = gtk_scrolled_window_new(0, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(album_scroll), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_container_add(GTK_CONTAINER(album_scroll), album_list_view);

    GtkWidget *song_scroll = gtk_scrolled_window_new(0, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(song_scroll), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_container_add(GTK_CONTAINER(song_scroll), song_list_view);

    gtk_table_attach_defaults(GTK_TABLE(table), album_scroll, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), song_scroll,  1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), frame2,       0, 2, 1, 2);

    /*gtk_table_attach_defaults(GTK_TABLE(table), frame4, 1, 2, 1, 2);*/

    g_signal_connect(G_OBJECT(album_list_view), "row-activated", G_CALLBACK(ui_album_select), song_list);

    g_signal_connect_swapped(G_OBJECT(window), "destroy",
            G_CALLBACK(gtk_main_quit), G_OBJECT(window));

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

