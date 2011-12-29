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
    stop_ui();

    printf("%s\n", _cm.album->e[offset]->path);
    exit(1);
}


static void
ui_start () 
{
    _cm.curses_window = initscr();
    _cm.cdk_screen = initCDKScreen(_cm.curses_window);

    initCDKColor();
}

static void
ui_stop () 
{
    destroyCDKScreen(_cm.cdk_screen);
    endCDK();
}


static void
ui_main ()
{
    ui_start();
    create_widgets();
    refreshCDKScreen (_cm.cdk_screen);
    traverseCDKScreen(_cm.cdk_screen);
    ui_stop();
    return 0;
}

int
create_widgets()
{
    char **album_repr = malloc(ALBUM_MAX * sizeof(void*));
    for(int i=0; i < _cm.album->l; i++) {
        album_repr[i] = _cm.album->e[i]->slug;
    }

    CDKSCROLL *widget_left = newCDKScroll (
            cdkscreen,
            LEFT, 1, LEFT, 30, 25, /* xyswh */
            "Album",
            album_repr, _cm.album->l,
            0, A_REVERSE, 1, 0
            );

    CDKSCROLL *widget_right = newCDKScroll (
            cdkscreen,
            RIGHT, 1, LEFT, 30, 25, /* xyswh */
            "Songs in album",
            0, 0,
            0, A_REVERSE, 1, 0
            );

    _cm.cdk_widgets.left = ObjPtr(widget_left);
    _cm.cdk_widgets.right = ObjPtr(widget_left);

    bindCDKObject(
        vSCROLL,
        _cm.cdk_widgets.left,
        KEY_ENTER,
        album_list_enter,
        0
    );
}
