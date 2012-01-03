#include "ui_common.h"
#include "song.h"
#include "album.h"
#include "main.h"
void
ui_fill_song_list(GtkWidget *song_list_wdg, 
                  struct song_list *song_list)
{
    GtkTreeIter song_iter;
    gtk_list_store_clear(GTK_LIST_STORE(song_list_wdg));

    int j, i;
    for(i=0; i<song_list->len; i++) {
        j=i;
        if (song_list != st.songs) {
            /* Find matching real id */
            for(j=0; j<st.songs->len; j++) {
                if(st.songs->e[j] == song_list->e[i]) { break; }
            }
        }

        gtk_list_store_append(GTK_LIST_STORE(song_list_wdg), &song_iter);
        gtk_list_store_set(GTK_LIST_STORE(song_list_wdg),
                           &song_iter,
                           0, j,
                           1, song_list->e[i]->slug,
                           -1);
    }
}


