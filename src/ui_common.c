#include "ui_common.h"
#include "song.h"
#include "album.h"
#include "main.h"
#include <math.h>

char *
song_format_duration(int duration) 
{
	char *retval = calloc(8, sizeof(char));
	sprintf(retval, "%2d:%02d",
		(duration/60),
		(duration%60)
	);

	return retval;
}

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

        char *duration = song_format_duration(
			song_list->e[i]->render_stat.duration
		);

        gtk_list_store_append(GTK_LIST_STORE(song_list_wdg), &song_iter);
        gtk_list_store_set(GTK_LIST_STORE(song_list_wdg),
                           &song_iter,
                           0, j,
                           1, song_list->e[i]->slug,
                           2, duration,
                           3, song_list->e[i]->render_stat.nullspace,
                           4, song_list->e[i]->render_stat.clipping,
                           5, (0 != song_list->e[i]->git.status),
                           -1);
    }
}

int
treesel_offset_item(
    GtkTreeSelection *selection,
    int model_offset) 
{
    int retval;

    GtkTreeIter iter;
    GtkTreeModel *model;

    /* Get the model + the iter... (GTK is weird) */
    if(!gtk_tree_selection_get_selected(selection,
                                        &model,
                                        &iter)) {
        return -1;
    }

    gtk_tree_model_get(
        model,
        &iter,
        model_offset,
        &retval,
        -1);

    return retval;
}
