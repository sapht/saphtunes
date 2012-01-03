#include <stdlib.h>

#include "main.h"
#include "ui_common.h"
#include "ui_page2.h"

#include "album.h"
#include "song.h"

static void
select_song(GtkTreeView *song_list_view, 
               GtkTreePath *item_path, 
               GtkTreeViewColumn *column,
               gpointer song_details
               )
{
    GtkTreeIter iter;
    GtkTreeModel *song_model = gtk_tree_view_get_model(song_list_view);
    int found = gtk_tree_model_get_iter(song_model, &iter, item_path);
    printf("%d\n", found);

    int song_offset;
    gtk_tree_model_get(song_model, &iter, 
        0, &song_offset,
        -1);

    struct song *song = st.songs->e[song_offset];

    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(song_details));
    gtk_text_buffer_set_text(buffer, song->git.status, -1);
}


GtkWidget *
ui_page_2_create()
{
    GtkListStore *song_list = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_STRING);
    GtkTreeIter song_iter;

    for(int i=0; i<st.songs->len; i++) {
        struct song *song_entry = st.songs->e[i];
        if(0 != song_entry->git.status){
            gtk_list_store_append(song_list, &song_iter);
            gtk_list_store_set(song_list, &song_iter,
                            0, (gint) i,
                            1, song_entry->slug,
                            -1);
        }

    }

    GtkWidget *song_list_view = gtk_tree_view_new();
    GtkCellRenderer *cell_renderer = gtk_cell_renderer_text_new();

    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(song_list_view),
        0, "Title", cell_renderer,
        "text", 1,
        NULL
    );

    gtk_tree_view_set_model(GTK_TREE_VIEW(song_list_view), GTK_TREE_MODEL(song_list));

    GtkWidget *song_status;
    song_status = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(song_status), 0);

    
    GtkWidget *table = gtk_table_new(1, 2, TRUE);
    gtk_table_set_row_spacings(GTK_TABLE(table), 10);
    gtk_table_set_col_spacings(GTK_TABLE(table), 10);

    GtkWidget *song_scroll = gtk_scrolled_window_new(0, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(song_scroll), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_container_add(GTK_CONTAINER(song_scroll), song_list_view);

    gtk_table_attach_defaults(GTK_TABLE(table), song_scroll,  0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), song_status,  1, 2, 0, 1);

    g_signal_connect(G_OBJECT(song_list_view),  "row-activated", G_CALLBACK(select_song), song_status);

    return table;
}
