#include <stdlib.h>

#include "main.h"
#include "ui_common.h"
#include "ui_page1.h"

#include "album.h"
#include "song.h"

struct song_buttons {
    GtkWidget *open;
    GtkWidget *frame;
    int open_sig;
};



static void
btn_open_song(GtkWidget *button, gpointer song_ptr)
{
    struct song *song = (struct song*) song_ptr;

    char *open_command = malloc(256*sizeof(char) + sizeof(song->slug));
    sprintf(open_command, "open \"%s\"", song->git.path);
    system(open_command);
    printf("%s\n", open_command);
}


static void
select_song(GtkTreeSelection *selection,
            gpointer song_buttons
            )
{
    GtkTreeIter iter;
    GtkTreeModel *song_model;

    if(!gtk_tree_selection_get_selected(selection,
                                         &song_model,
                                         &iter)) {
        return;
    }

    int song_offset;
    gtk_tree_model_get(song_model, &iter, 
        0, &song_offset,
        -1);

    struct song *song = st.songs->e[song_offset];
    

    struct song_buttons *buttstruct = 
            (struct song_buttons *) song_buttons;

    gtk_frame_set_label(GTK_FRAME(buttstruct->frame), song->slug);

    GtkWidget *open_button = buttstruct->open;
    if(buttstruct->open_sig != 0) {
        g_signal_handler_disconnect(open_button, buttstruct->open_sig);
    }

    buttstruct->open_sig = g_signal_connect (open_button, "clicked",
        G_CALLBACK (btn_open_song), song);
}

static void
select_album(/*GtkTreeView *album_list_view, */
                       /*GtkTreePath *item_path, */
                       /*GtkTreeViewColumn *column,*/
             GtkTreeSelection *selection,
             gpointer song_list
             )
{
    GtkTreeIter iter;
    GtkTreeModel *album_model;

    if(!gtk_tree_selection_get_selected(selection,
                                         &album_model,
                                         &iter)) {
        return;
    }

    int album_offset;
    gtk_tree_model_get(album_model, &iter, 
        0, &album_offset,
        -1);

    ui_fill_song_list(song_list, &st.albums->e[album_offset]->songs);
}

static void
album_sort_title(GtkTreeViewColumn *view_col,
                 gpointer data)
{
    GtkTreeView *album_view = GTK_TREE_VIEW(
        gtk_tree_view_column_get_tree_view(view_col)
    );

    GtkTreeSortable *album_sorter = GTK_TREE_SORTABLE(
        gtk_tree_view_get_model(album_view)
    );

    GtkSortType order;
    gint current_sort_field;

    if (gtk_tree_sortable_get_sort_column_id(
            album_sorter,
            &current_sort_field,
            &order) && current_sort_field == 1) {

        gtk_tree_sortable_set_sort_column_id(
            album_sorter,
            1,
            ((order == GTK_SORT_ASCENDING) 
                ? GTK_SORT_DESCENDING
                : GTK_SORT_ASCENDING)
        );
    } else {
        gtk_tree_sortable_set_sort_column_id(
            album_sorter,
            1,
            GTK_SORT_ASCENDING);
    }
}

static void
album_sort_numsongs(GtkTreeViewColumn *view_col,
                 gpointer data)
{
    GtkTreeView *album_view = GTK_TREE_VIEW(
        gtk_tree_view_column_get_tree_view(view_col)
    );

    GtkTreeSortable *album_sorter = GTK_TREE_SORTABLE(
        gtk_tree_view_get_model(album_view)
    );

    GtkSortType order;
    gint current_sort_field;

    if (gtk_tree_sortable_get_sort_column_id(
            album_sorter,
            &current_sort_field,
            &order) && current_sort_field == 2) {

        gtk_tree_sortable_set_sort_column_id(
            album_sorter,
            2,
            ((order == GTK_SORT_ASCENDING) 
                ? GTK_SORT_DESCENDING
                : GTK_SORT_ASCENDING)
        );
    } else {
        gtk_tree_sortable_set_sort_column_id(
            album_sorter,
            2,
            GTK_SORT_ASCENDING);
    }
}

GtkWidget *
ui_page_1_create()
{
    /***************************************
     *Creating album list widget           *
     ***************************************/
    GtkListStore *album_list = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
    GtkTreeIter album_iter;

    for(int i=0; i<st.albums->len; i++) {
        struct album *album_entry = st.albums->e[i];

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

    /*gtk_tree_sortable_set_sort_column_id(*/
        /*GTK_TREE_SORTABLE(album_list),*/
        /*2,*/
        /*GTK_SORT_DESCENDING);*/

    g_signal_connect(
        gtk_tree_view_get_column(
            GTK_TREE_VIEW(album_list_view),
            0),
         "clicked",
         G_CALLBACK(album_sort_title),
         NULL
    );

    g_signal_connect(
        gtk_tree_view_get_column(
            GTK_TREE_VIEW(album_list_view),
            1),
         "clicked",
         G_CALLBACK(album_sort_numsongs),
         NULL
    );


    /***************************************** 
     * Song list view                        *
     *****************************************/
    GtkListStore *song_list = gtk_list_store_new(
        4, 
        G_TYPE_INT, 
        G_TYPE_STRING,
        G_TYPE_INT,
        G_TYPE_INT
    );
    /*GtkTreeIter song_iter;*/

    GtkWidget *song_list_view = gtk_tree_view_new();
    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(song_list_view),
        0, "Title", cell_renderer,
        "text", 1,
        NULL);

    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(song_list_view),
        1, "null", cell_renderer,
        "text", 2,
        NULL);

    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(song_list_view),
        2, "clip", cell_renderer,
        "text", 3,
        NULL);

    gtk_tree_view_set_model(GTK_TREE_VIEW(song_list_view), GTK_TREE_MODEL(song_list));


    /* *********************************** *
     * Song details                        *
     * ************************************/
    GtkWidget *song_details, *song_details_box;
    song_details = gtk_frame_new("no selection");
    song_details_box = gtk_hbox_new(0, 1);

    struct song_buttons *song_buttons = 
            malloc(sizeof(struct song_buttons));
    song_buttons->frame = song_details;
    song_buttons->open = gtk_button_new_with_mnemonic("_Open");
    song_buttons->open_sig = 0;
    gtk_box_pack_start(GTK_BOX(song_details_box), song_buttons->open, 0, 0, 0);

    gtk_container_add(GTK_CONTAINER(song_details), song_details_box);


    /***************************************
     *Make containers for all the stuffs   *
     ***************************************/

    GtkWidget *table = gtk_table_new(2, 2, TRUE);
    gtk_table_set_row_spacings(GTK_TABLE(table), 10);
    gtk_table_set_col_spacings(GTK_TABLE(table), 10);

    GtkWidget *album_scroll = gtk_scrolled_window_new(0, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(album_scroll), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_container_add(GTK_CONTAINER(album_scroll), album_list_view);

    GtkWidget *song_scroll = gtk_scrolled_window_new(0, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(song_scroll), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_container_add(GTK_CONTAINER(song_scroll), song_list_view);

    gtk_table_attach_defaults(GTK_TABLE(table), album_scroll, 0, 1, 0, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), song_scroll,  1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), song_details, 1, 2, 1, 2);

    /*g_signal_connect(G_OBJECT(album_list_view), "row-activated", G_CALLBACK(select_album), song_list);*/
    GtkTreeSelection *album_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(album_list_view));
    GtkTreeSelection *song_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(song_list_view));
    g_signal_connect(G_OBJECT(album_selection), 
                     "changed", 
                     G_CALLBACK(select_album), 
                     song_list);

    g_signal_connect(G_OBJECT(song_selection),  
                     "changed", 
                     G_CALLBACK(select_song), 
                     song_buttons);

    return table;
}
