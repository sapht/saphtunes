#include <stdlib.h>

#include "string.h"
#include "main.h"
#include "ui_common.h"
#include "ui_page1.h"

enum {
        TARGET_INT32,
        TARGET_STRING,
        TARGET_ROOTWIN
};

struct album_list_wdg {
	GtkTreeView       *view;
	GtkListStore      *store;
	GtkScrolledWindow *scroll;
	GtkCellRenderer   *cr;
	GtkTreeSelection  *sel;
};

struct song_list_wdg {
	GtkTreeView       *view;
	GtkListStore      *store;
	GtkScrolledWindow *scroll;
	GtkCellRenderer   *cr;
	GtkTreeSelection  *sel;
};

struct song_detail_wdg {
    /* Song Detail box */
    GtkWidget *frame;
    GtkWidget *box;
    GtkWidget *vbox;
	GtkWidget *gstatus;
	GtkWidget *gstatus_sw;
	GtkWidget *rstatus_dur;
	GtkWidget *rstatus_null;
	GtkWidget *rstatus_clip;
    GtkWidget *btn[2];
    int btn_num;
    int   btn_sig[2];
    void *btn_cb[2];
};

/*****************************************************
 * Creation                                          *
 *****************************************************/

/*  Album stuff */
static struct album_list_wdg *
create_album_list_wdg()
{
	struct album_list_wdg *wdg = malloc(sizeof(struct album_list_wdg));

    wdg->cr    = gtk_cell_renderer_text_new();
	wdg->view  = GTK_TREE_VIEW(gtk_tree_view_new()),
	wdg->store = gtk_list_store_new(
		3, 
		G_TYPE_INT, 
		G_TYPE_STRING, 
		G_TYPE_INT
	);
    gtk_tree_view_set_model(wdg->view, GTK_TREE_MODEL(wdg->store));

    wdg->scroll = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(0, 0));
    gtk_scrolled_window_set_policy(
		wdg->scroll,
		GTK_POLICY_NEVER, 
		GTK_POLICY_ALWAYS
	);
    gtk_container_add(
		GTK_CONTAINER(wdg->scroll), 
		GTK_WIDGET(wdg->view)
	);


	/* Create columns */
	char *cols = "Title\0Song#";
	for(int i=0; i<2; i++) {
		gtk_tree_view_insert_column_with_attributes(
			wdg->view,
			i,    /* Offset in header */
			cols, 
			wdg->cr,
			"text",
			i+1, /* Store col number to display */
			NULL);
		cols += strlen(cols) + 1;

		/* Tell GTK that this header corresponds to this field for sorting. */
		gtk_tree_view_column_set_sort_column_id(
			gtk_tree_view_get_column(wdg->view, i), i+1);
	}

    wdg->sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(wdg->view));

	return wdg;
}

/*  Song stuff */
static struct song_list_wdg *
create_song_list_wdg() 
{
	struct song_list_wdg *wdg = malloc(sizeof(struct song_list_wdg));
    wdg->cr = gtk_cell_renderer_text_new();
    wdg->view  = GTK_TREE_VIEW(gtk_tree_view_new());
    wdg->store = gtk_list_store_new(
        6, 
        G_TYPE_INT, 
        G_TYPE_STRING,
        G_TYPE_STRING,
        G_TYPE_INT,
        G_TYPE_INT,
        G_TYPE_INT
    );

    gtk_tree_view_set_model(wdg->view, GTK_TREE_MODEL(wdg->store));

    wdg->scroll = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(0, 0));
    gtk_scrolled_window_set_policy(
		wdg->scroll,
		GTK_POLICY_NEVER, 
		GTK_POLICY_ALWAYS
	);
    gtk_container_add(
		GTK_CONTAINER(wdg->scroll), 
		GTK_WIDGET(wdg->view)
	);


	char *cols = "name\0time\0nil\0clp\0git";

	for(int i=0; i<5; i++) {
		gtk_tree_view_insert_column_with_attributes(
			wdg->view,
			i, 
			cols, 
			wdg->cr,
			"text", 
			i+1,
			NULL);

		cols += strlen(cols) + 1;

		gtk_tree_view_column_set_sort_column_id(
			gtk_tree_view_get_column(wdg->view, i), i+1);
	}

    wdg->sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(wdg->view));

	return wdg;
}

static void
song_store_fill(GtkListStore *song_liststore, 
			   struct song_list *song_list)
{
    GtkTreeIter song_iter;
    gtk_list_store_clear(song_liststore);

	if(!song_list) {
		song_list = st.songs;
	}

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

        gtk_list_store_append(song_liststore, &song_iter);
        gtk_list_store_set(song_liststore,
                           &song_iter,
                           0, j,
                           1, song_list->e[i]->slug,
                           2, duration,
                           3, song_list->e[i]->render_stat.nullspace,
                           4, song_list->e[i]->render_stat.clipping,
                           5, (0 != strcmp("", song_list->e[i]->git.status)),
                           -1);
    }
}

static void
album_store_fill(GtkListStore *album_store,
				struct song_list *all_songs,
				struct album_list *album_list)
{
	/*  Create initial items */
    GtkTreeIter album_iter;

	gtk_list_store_append(album_store, &album_iter);
	gtk_list_store_set   (album_store, &album_iter,
			0, -1,
			1, "--All songs--",
			2, all_songs->len,
			-1);

    for(int i=0; i<album_list->len; i++) {
        struct album *album_entry = album_list->e[i];

        gtk_list_store_append(album_store, &album_iter);
        gtk_list_store_set   (album_store, &album_iter,
                0, (gint) i,
                1, album_entry->slug,
                2, album_entry->songs.len,
                -1);
    }
}

static void
btn_open_song(GtkWidget *button, gpointer song_ptr)
{
    struct song *song = (struct song*) song_ptr;

    char *open_command = malloc(sizeof(char) * (8 + strlen(song->git.path)));
    sprintf(open_command, "open \"%s\"", song->git.path);
    system(open_command);
    printf("%s\n", open_command);
}

static void
btn_play_song(GtkWidget *button, gpointer song_ptr)
{
    struct song *song = (struct song*) song_ptr;

    char *open_command = malloc(256*sizeof(char) + sizeof(song->slug));
    sprintf(open_command, "open \"%s\"", song->git.path);
    system(open_command);
    printf("%s\n", open_command);
}


static struct song_detail_wdg *
create_song_detail_wdg()
{
    struct song_detail_wdg *sdbox = malloc(sizeof(struct song_detail_wdg));
	*sdbox = (struct song_detail_wdg) {
        .frame = gtk_frame_new("--"),
		.gstatus = gtk_text_view_new(),
		.rstatus_dur  = gtk_label_new("--"),
	 	.rstatus_null = gtk_label_new("--"),
		.rstatus_clip = gtk_label_new("--"),
        .box =   gtk_hbox_new(0, 1),
		.vbox = gtk_vbox_new(0, 1),
        .btn = { gtk_button_new_with_mnemonic("_Play"),
                 gtk_button_new_with_mnemonic("_Edit") },
        .btn_num = 2,
        .btn_sig = {0, 0},
        .btn_cb  = {btn_open_song, btn_play_song}
    };


	/* Set up the status boxes */
	GtkWidget *stati_box = gtk_hbox_new(0, 1);

	GtkWidget *gstatus_sw = gtk_scrolled_window_new(0, 0);
	gtk_scrolled_window_set_policy(
		GTK_SCROLLED_WINDOW(gstatus_sw),
		GTK_POLICY_AUTOMATIC,
		GTK_POLICY_AUTOMATIC);

	gtk_container_add(
		GTK_CONTAINER(gstatus_sw),
		sdbox->gstatus);

	GtkWidget *gstatus_frame = gtk_frame_new("git status");
	gtk_container_add(
		GTK_CONTAINER(gstatus_frame), 
		gstatus_sw
	);

	GtkWidget *rstatus_frame = gtk_frame_new("render stat");
	GtkWidget *rstatus_box = gtk_vbox_new(0, 1);
	gtk_box_pack_start(GTK_BOX(rstatus_box), sdbox->rstatus_dur,  1, 0, 0);
	gtk_box_pack_start(GTK_BOX(rstatus_box), sdbox->rstatus_null, 1, 0, 0);
	gtk_box_pack_start(GTK_BOX(rstatus_box), sdbox->rstatus_clip, 1, 0, 0);
	gtk_container_add(GTK_CONTAINER(rstatus_frame), rstatus_box);


	/* Pack the status box */
	gtk_box_pack_start(GTK_BOX(stati_box), gstatus_frame, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(stati_box), rstatus_frame, 1, 1, 0);

	/* Pack the master box */
	gtk_box_pack_start(GTK_BOX(sdbox->vbox), stati_box, 0, 0, 0);
	gtk_box_pack_start(GTK_BOX(sdbox->vbox), sdbox->box, 0, 0, 0);

    for(int i=0; i<sdbox->btn_num; i++) {
        gtk_box_pack_start(GTK_BOX(sdbox->box), sdbox->btn[i], 0, 0, 0);
    }

    gtk_container_add( GTK_CONTAINER(sdbox->frame), sdbox->vbox);

    return sdbox;
}

static void
select_song(GtkTreeSelection *selection,
            gpointer sdbox_p
            )
{
    struct song_detail_wdg *sdbox =
        (struct song_detail_wdg *) sdbox_p;

	int song_offset = treesel_offset_item(
            selection,
            0
	);

	/*  TODO: This is some weird bug -- we seem to get a selection from select_album */
	if(song_offset < 0) return;
    struct song *song = 
        st.songs->e[song_offset];

    /* Frame label... */
	gtk_frame_set_label(
		GTK_FRAME(sdbox->frame),
		song->slug
		);

	char render_info[3][32];
	memset(render_info, 0, 3*32);
	sprintf(render_info[0],
		"Duration: %s",
		song_format_duration(song->render_stat.duration) 
	);
	sprintf(render_info[1],
		"%s",
		(song->render_stat.nullspace ? "Has Nullspace" : "") );
	sprintf(render_info[2],
		"%s",
		(song->render_stat.clipping  ? "Has Clipping" : "") );
	
	gtk_label_set_text(GTK_LABEL(sdbox->rstatus_dur),  render_info[0]);
	gtk_label_set_text(GTK_LABEL(sdbox->rstatus_null), render_info[1]);
	gtk_label_set_text(GTK_LABEL(sdbox->rstatus_clip), render_info[2]);

	gtk_text_buffer_set_text(
		gtk_text_view_get_buffer(
			GTK_TEXT_VIEW(sdbox->gstatus)
		),
		song->git.status,
		-1
	);

    /*  Modify handlers */
    for(int i=0; i<2; i++) {
        /*  Disconnect any present handler */
        if(sdbox->btn_sig[i]) {
            g_signal_handler_disconnect(
                sdbox->btn[i],
                sdbox->btn_sig[i]
                );
        }

        /*  Create the new handler */
        sdbox->btn_sig[i] = g_signal_connect(
            sdbox->btn[i],
            "clicked",
            G_CALLBACK(sdbox->btn_cb[i]),
            song
            );
    }
}



static void
select_album(GtkTreeSelection *selection,
             gpointer song_list)
{
	int album_offset = treesel_offset_item(
            selection, 0
        );
	if(album_offset >= 0) {
		song_store_fill(
			song_list, 
			&st.albums->e[album_offset]->songs
		);
	} else {
		/* The placeholder -1 album: means entire list */
		song_store_fill(
			song_list, 
			st.songs
		);
	}
}

GtkWidget *
ui_page_1_create()
{
	struct album_list_wdg  *wdg_album   = create_album_list_wdg();
	struct song_list_wdg   *wdg_songs   = create_song_list_wdg();
    struct song_detail_wdg *wdg_songdet = create_song_detail_wdg();

    GtkTargetEntry target_list[] = {
            { "INTEGER",    0, TARGET_INT32 },
            { "STRING",     0, TARGET_STRING },
            { "text/plain", 0, TARGET_STRING },
            { "application/x-rootwindow-drop", 0, TARGET_ROOTWIN }
    };

    guint n_targets = G_N_ELEMENTS (target_list);

	album_store_fill(wdg_album->store,
					 st.songs,
					 st.albums);

    g_signal_connect(wdg_album->sel,
                     "changed",
                     G_CALLBACK(select_album),
                     wdg_songs->store
	);

    g_signal_connect(wdg_songs->sel,
                     "changed",
                     G_CALLBACK(select_song),
                     wdg_songdet
	);

    gtk_drag_source_set(GTK_WIDGET(wdg_songs->view),
                        GDK_BUTTON1_MASK,
                        target_list,
                        n_targets,
                        GDK_ACTION_COPY);


    GtkWidget *table = gtk_table_new(2, 2, TRUE);
    gtk_table_set_row_spacings(GTK_TABLE(table), 10);
    gtk_table_set_col_spacings(GTK_TABLE(table), 10);

    gtk_table_attach_defaults(GTK_TABLE(table), 
		GTK_WIDGET(wdg_album->scroll), 
		0, 1, 0, 2);

    gtk_table_attach_defaults(GTK_TABLE(table), 
		GTK_WIDGET(wdg_songs->scroll), 
		1, 2, 0, 1);

    gtk_table_attach_defaults(GTK_TABLE(table), 
		GTK_WIDGET(wdg_songdet->frame), 
		1, 2, 1, 2);

    return table;
}
