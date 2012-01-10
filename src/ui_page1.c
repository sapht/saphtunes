#include <stdlib.h>

#include "main.h"
#include "ui_common.h"
#include "ui_page1.h"

#include "album.h"
#include "song.h"

static struct song_detail_box {
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
btn_play_song(GtkWidget *button, gpointer song_ptr)
{
    struct song *song = (struct song*) song_ptr;

    char *open_command = malloc(256*sizeof(char) + sizeof(song->slug));
    sprintf(open_command, "open \"%s\"", song->git.path);
    system(open_command);
    printf("%s\n", open_command);
}


static struct song_detail_box *
create_song_detail_box()
{
    struct song_detail_box sdbox = {
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
		sdbox.gstatus);

	GtkWidget *gstatus_frame = gtk_frame_new("git status");
	gtk_container_add(
		GTK_CONTAINER(gstatus_frame), 
		gstatus_sw
	);

	GtkWidget *rstatus_frame = gtk_frame_new("render stat");
	GtkWidget *rstatus_box = gtk_vbox_new(0, 1);
	gtk_box_pack_start(GTK_BOX(rstatus_box), sdbox.rstatus_dur,  1, 0, 0);
	gtk_box_pack_start(GTK_BOX(rstatus_box), sdbox.rstatus_null, 1, 0, 0);
	gtk_box_pack_start(GTK_BOX(rstatus_box), sdbox.rstatus_clip, 1, 0, 0);
	gtk_container_add(GTK_CONTAINER(rstatus_frame), rstatus_box);


	/* Pack the status box */
	gtk_box_pack_start(GTK_BOX(stati_box), gstatus_frame, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(stati_box), rstatus_frame, 1, 1, 0);

	/* Pack the master box */
	gtk_box_pack_start(GTK_BOX(sdbox.vbox), stati_box, 0, 0, 0);
	gtk_box_pack_start(GTK_BOX(sdbox.vbox), sdbox.box, 0, 0, 0);

    for(int i=0; i<sdbox.btn_num; i++) {
        gtk_box_pack_start(GTK_BOX(sdbox.box), sdbox.btn[i], 0, 0, 0);
    }

    gtk_container_add( GTK_CONTAINER(sdbox.frame), sdbox.vbox);

    struct song_detail_box *sdbox_p = malloc(sizeof(sdbox));
    *sdbox_p = sdbox;
    return sdbox_p;
}

static void
select_song(GtkTreeSelection *selection,
            gpointer sdbox_p
            )
{
    struct song_detail_box *sdbox =
        (struct song_detail_box *) sdbox_p;

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
		ui_fill_song_list(
			song_list, 
			&st.albums->e[album_offset]->songs
		);
	} else {
		/* The placeholder -1 album: means entire list */
		ui_fill_song_list(
			song_list, 
			st.songs
		);
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

	gtk_list_store_append(album_list, &album_iter);
	gtk_list_store_set(album_list, &album_iter,
			0, -1,
			1, "--All songs--",
			2, st.songs->len,
			-1);

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

    gtk_tree_view_column_set_sort_column_id(
        gtk_tree_view_get_column(
            GTK_TREE_VIEW(album_list_view),
            0), 1
    );
    gtk_tree_view_column_set_sort_column_id(
        gtk_tree_view_get_column(
            GTK_TREE_VIEW(album_list_view),
            1), 2
    );


    /***************************************** 
     * Song list view                        *
     *****************************************/
    GtkListStore *song_list = gtk_list_store_new(
        6, 
        G_TYPE_INT, 
        G_TYPE_STRING,
        G_TYPE_STRING,
        G_TYPE_INT,
        G_TYPE_INT,
        G_TYPE_INT
    );
    /*GtkTreeIter song_iter;*/

    GtkWidget *song_list_view = gtk_tree_view_new();
    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(song_list_view),
        0, "name", cell_renderer,
        "text", 1,
        NULL);
    gtk_tree_view_column_set_sort_column_id(
        gtk_tree_view_get_column( GTK_TREE_VIEW(song_list_view), 0), 
        1
        );

    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(song_list_view),
        1, "time", cell_renderer,
        "text", 2,
        NULL);
    gtk_tree_view_column_set_sort_column_id(
        gtk_tree_view_get_column( GTK_TREE_VIEW(song_list_view), 1), 
        2
        );

    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(song_list_view),
        2, "nil", cell_renderer,
        "text", 3,
        NULL);
    gtk_tree_view_column_set_sort_column_id(
        gtk_tree_view_get_column( GTK_TREE_VIEW(song_list_view), 2), 
        3
        );

    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(song_list_view),
        3, "clp", cell_renderer,
        "text", 4,
        NULL);
    gtk_tree_view_column_set_sort_column_id(
        gtk_tree_view_get_column( GTK_TREE_VIEW(song_list_view), 3), 
        4
        );

    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(song_list_view),
        4, "git", cell_renderer,
        "text", 5,
        NULL);
    gtk_tree_view_column_set_sort_column_id(
        gtk_tree_view_get_column( GTK_TREE_VIEW(song_list_view), 3), 
        5
        );



    gtk_tree_view_set_model(GTK_TREE_VIEW(song_list_view), GTK_TREE_MODEL(song_list));


    /* *********************************** *
     * Song details                        *
     * ************************************/
    struct song_detail_box *sdbox = create_song_detail_box();


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
    gtk_table_attach_defaults(GTK_TABLE(table), sdbox->frame, 1, 2, 1, 2);

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
                     sdbox);

    return table;
}
