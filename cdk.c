#include <cdk.h>
#include <signal.h>
#include "cdk.h"
#include "main.h"
#include "album.h"
#include "song.h"
#include <stdlib.h>

static int
cdk_fill_song_widget(struct song_list *song_list)
{
    char **song_repr = malloc(song_list->len * sizeof(char*));
    for(int i=0; i < song_list->len; i++) {
        song_repr[i] = song_list->e[i]->slug;
    }

    char **bogus_fill = malloc(LINES * sizeof(char*));
    for(int i=0; i<LINES; i++) {
        bogus_fill[i] = "           ";
    }

    setCDKScrollItems(cm.cdk.widgets.right,
                 bogus_fill,
                 LINES,
                 0);

    refreshCDKScreen(cm.cdk.screen);
    /* Erase the list, then fill it with song_repr, len=song_list->len */
    setCDKScrollItems(cm.cdk.widgets.right,
                 song_repr,
                 song_list->len,
                 0);

    moveCDKScroll(cm.cdk.widgets.right, 
        5, 5, 
        0, 1
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
    int offset = getCDKScrollCurrent(cm.cdk.widgets.left);

    struct song_list *song_list = &cm.albums->e[offset]->songs;

    /* Propagate widget of windows */
    int success = cdk_fill_song_widget(song_list);
    if(!success) {
        ui_stop();
        printf("Could not fill song_list\n");
        exit(1);
    }

    /*activateCDKScroll(cm.cdk.widgets.right, 0);*/
    refreshCDKScreen (cm.cdk.screen);
    return 0;
}

/*static void ui_resize() {
        destroyCDKScroll(cm.cdk.widgets.left);
        destroyCDKScroll(cm.cdk.widgets.right);
        create_widgets();
        setCDKFocusLast(cm.cdk.screen);
}*/

/*static void ui_resize_signal(int signal) { ui_resize(); }*/

int
create_widgets()
{
    char **album_repr = malloc(ALBUM_MAX_NUM * sizeof(void*));
    for(int i=0; i < cm.albums->len; i++) {
        album_repr[i] = cm.albums->e[i]->slug;
    }

    cm.cdk.widgets.left  = newCDKScroll (
            cm.cdk.screen,
            LEFT, 1, RIGHT,  LINES-5, (COLS/2)-1, /* xyswh */
            "Album",
            album_repr, cm.albums->len,
            0, A_REVERSE, 1, 0
            );

    cm.cdk.widgets.right = newCDKScroll (
            cm.cdk.screen,
            RIGHT, 1, RIGHT, LINES-5, (COLS/2)-1, /* xyswh */
            "Songs in album",
            0, 0,
            0, A_REVERSE, 1, 0
            );
     

    bindCDKObject(
        vSCROLL,
        cm.cdk.widgets.left,
        KEY_ENTER,
        album_list_enter,
        0
    );

    return 1;
}

void
ui_start () 
{
    cm.cdk.curses_window = initscr();
    cm.cdk.screen = initCDKScreen(cm.cdk.curses_window);
    /*signal(SIGWINCH, ui_resize_signal);*/

    initCDKColor();
}

void
ui_stop () 
{
    destroyCDKScreen(cm.cdk.screen);
    endCDK();
}


int
ui_main ()
{
    ui_start();
    create_widgets();
    refreshCDKScreen (cm.cdk.screen);
    traverseCDKScreen(cm.cdk.screen);
    ui_stop();
    return 0;
}
