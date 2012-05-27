#include "ui_common.h"
#include "main.h"
#include <math.h>
#include <stdlib.h>

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
