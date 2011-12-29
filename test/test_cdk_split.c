#include <cdk.h>
#include "util.h"
#include <stdio.h>
#include <string.h>

#ifdef HAVE_XCURSES
char *XCursesProgramName = "entry_ex";
#endif

struct cdk_dim {
    int x;
    int y;
    int w;
    int h;

    int s;
};

struct congman_singleton {
    CDKSCREEN *cdk_screen;
    WINDOW *curses_window;

    struct widgets {
        CDKOBJS* left;
        CDKOBJS* right;
    } w;

    char **strings_1;
    int strings_1_l;
    char **strings_2;
    int strings_2_l;
} _cm;

static void
start_ui () {
    _cm.curses_window = initscr();
    _cm.cdk_screen = initCDKScreen(_cm.curses_window);

    initCDKColor();
}

static void
stop_ui () {
    destroyCDKScreen(_cm.cdk_screen);
    endCDK();
}


CDKOBJS *
cdk_make_split (CDKSCREEN *cdkscreen, 
                    char **input_data, 
                    int input_data_num, 
                    struct cdk_dim *dim)
{
    CDKSCROLL *widget = newCDKScroll (
            cdkscreen,
            dim->x,
            dim->y,
            LEFT, /* XXX: Scrollbar position? */
            dim->w,
            dim->h,
            "Title",

            input_data, 
            input_data_num,

            0, /* Display numbers? */
            A_REVERSE,
            1,  /* Display box? */
            0 /* Display shadow? (yuck) */
            );
    return ObjPtr (widget);
}



CDKOBJS *
create_left_split(CDKSCREEN *cdk_screen) {
    struct cdk_dim dim = {
        .x = LEFT, .y = 1, .w = 30, .h = 25, .s = 1 
    };
    return cdk_make_split(cdk_screen, _cm.strings_1, _cm.strings_1_l, &dim);
}

CDKOBJS *
create_right_split(CDKSCREEN *cdk_screen) {
    struct cdk_dim dim = {
        .x = RIGHT, .y = 2, .w = 30, .h = 20
    };
    return cdk_make_split(cdk_screen, _cm.strings_2, _cm.strings_2_l, &dim);
}


int 
superfun(
        EObjectType cdktype, 
        void* object, 
        void* clientdata, 
        chtype key)
{
    CDKSCROLL *scroll = (CDKSCROLL*) object;

    int offset = getCDKScrollCurrent(scroll);
    /*char *msg = strdup(scroll->itemList[offset]);*/
    stop_ui();

    printf("%s\n", _cm.strings_1[offset]);
    exit(1);

}

void 
load_data()
{
    int num_fill = 80;
    char *fill_1[] = {"ein", "zwei", "drei", "vier"};
    char *fill_2[] = {"uno", "dos", "tres", "quattro", "cinco"};

    _cm.strings_1 = malloc(num_fill * sizeof(char*));
    _cm.strings_2 = malloc(num_fill * sizeof(char*));

    int data_num;
    for(data_num = 0; data_num < num_fill; data_num++) {
        _cm.strings_1[data_num] = fill_1[data_num % NUM_ELEMENTS(fill_1)];
        _cm.strings_2[data_num] = fill_2[data_num % NUM_ELEMENTS(fill_2)];
    }

    _cm.strings_1_l = num_fill;
    _cm.strings_2_l = num_fill;
}

int 
main (int argc GCC_UNUSED, char **argv GCC_UNUSED)
{
    start_ui();
    load_data();

    _cm.w.left  = create_left_split(_cm.cdk_screen);
    _cm.w.right = create_right_split(_cm.cdk_screen);

    bindCDKObject(
        vSCROLL,
        _cm.w.left,
        KEY_ENTER,
        superfun,
        0
    );

    refreshCDKScreen (_cm.cdk_screen);
    traverseCDKScreen(_cm.cdk_screen);

    stop_ui();

    return 0;
}
