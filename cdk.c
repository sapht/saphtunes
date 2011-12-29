#include <cdk.h>
#include "cdk.h"
#include "main.h"
#include "album.h"
#include <stdlib.h>

int
album_list_enter(
        EObjectType cdktype, 
        void* object, 
        void* clientdata, 
        chtype key)
/* Called when "enter" is pressed on an album */
{
    CDKSCROLL *scroll = (CDKSCROLL*) object;

    int offset = getCDKScrollCurrent(scroll);
    ui_stop();

    printf("%s\n", _cm.albums->e[offset]->path);
    exit(1);
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
            LEFT, 1, LEFT, 30, 25, /* xyswh */
            "Album",
            album_repr, _cm.albums->len,
            0, A_REVERSE, 1, 0
            );

    CDKSCROLL *widget_right = newCDKScroll (
            _cm.cdk_screen,
            RIGHT, 1, LEFT, 30, 25, /* xyswh */
            "Songs in album",
            0, 0,
            0, A_REVERSE, 1, 0
            );

    _cm.cdk_widgets.left = ObjPtr(widget_left);
    _cm.cdk_widgets.right = ObjPtr(widget_right);

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
