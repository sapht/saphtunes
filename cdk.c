#include <cdk.h>
#include "cdk.h"
#include "main.h"
#include "album.h"
#include "song.h"
#include <stdlib.h>

static int
cdk_fill_song_widget(struct song_list *song_list)
{
    char **song_repr = malloc(SONG_MAX_NUM * sizeof(void*));
    for(int i=0; i < song_list->len; i++) {
        song_repr[i] = song_list->e[i]->slug;
    }

    /* Erase the list, then fill it with song_repr, len=song_list->len */
    CDKSCROLL *scroll = (CDKSCROLL*) _cm.cdk_widgets.right;
    setCDKScrollItems(
        scroll,
        song_repr,
        song_list->len,
        0
    );

    return 1;
}


int
album_list_enter(
        EObjectType cdktype, 
        void* object, 
        void* clientdata, 
        chtype key)
/* Called when "enter" is pressed on an album */
{
    CDKSCROLL *album_w = (CDKSCROLL*) object;

    int offset = getCDKScrollCurrent(album_w);

    struct song_list *song_list = &_cm.albums->e[offset]->songs;

    /* Propagate widget of windows */
    int could_fill = cdk_fill_song_widget(song_list);
    refreshCDKScreen (_cm.cdk_screen);
    traverseCDKScreen(_cm.cdk_screen);
/*
    ui_stop();
    for(int i=0;i<song_list->len; i++) {
        printf("%s\n", song_list->e[i]->slug);
    }
    exit(1);*/
    return 0;
}

int
create_widgets()
{
    char **album_repr = malloc(ALBUM_MAX_NUM * sizeof(void*));
    for(int i=0; i < _cm.albums->len; i++) {
        album_repr[i] = _cm.albums->e[i]->slug;
    }

    CDKSCROLL *widget_left = newCDKScroll (
            _cm.cdk_screen,
            LEFT, 1, RIGHT, 30, 25, /* xyswh */
            "Album",
            album_repr, _cm.albums->len,
            0, A_REVERSE, 1, 0
            );

    CDKSCROLL *widget_right = newCDKScroll (
            _cm.cdk_screen,
            RIGHT, 1, RIGHT, 30, 25, /* xyswh */
            "Songs in album",
            0, 0,
            0, A_REVERSE, 1, 0
            );

    _cm.cdk_widgets.left  = (CDKOBJS*) widget_left;
    _cm.cdk_widgets.right = (CDKOBJS*) widget_right;

    bindCDKObject(
        vSCROLL,
        _cm.cdk_widgets.left,
        KEY_ENTER,
        album_list_enter,
        0
    );

    return 1;
}

void
ui_start () 
{
    _cm.curses_window = initscr();
    _cm.cdk_screen = initCDKScreen(_cm.curses_window);

    initCDKColor();
}

void
ui_stop () 
{
    destroyCDKScreen(_cm.cdk_screen);
    endCDK();
}


int
ui_main ()
{
    ui_start();
    create_widgets();
    refreshCDKScreen (_cm.cdk_screen);
    traverseCDKScreen(_cm.cdk_screen);
    ui_stop();
    return 0;
}