#include <gtk/gtk.h>
#ifndef _UI_COMMON__H_
#define _UI_COMMON__H_
#include "song.h"
void ui_fill_song_list(GtkWidget *song_list_wdg, 
                       struct song_list *song_list);
#endif
