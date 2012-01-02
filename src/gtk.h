#ifndef _CM_GTK__H_
#define _CM_GTK__H_
#include <gtk/gtk.h>

void ui_album_select(
GtkTreeView *album_list_view, 
GtkTreePath *item_path, 
GtkTreeViewColumn *column, 
gpointer song_list
);
int ui_main(int *argc, char ***argv);
#endif
