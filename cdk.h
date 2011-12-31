#include <cdk.h>
#ifndef _CM_CDK__H_
#define _CM_CDK__H_
/* _CDK__H_ is probably defined in <cdk.h> */

struct cdk {
    CDKSCREEN *screen;
    WINDOW *curses_window;

    struct {
        CDKSCROLL* left;
        CDKSCROLL* right;
    } widgets;
};

int album_list_enter( EObjectType cdktype, void* object, void* clientdata, chtype key) ;
void ui_start () ;
void ui_stop () ;
int ui_main () ;
int create_widgets();

#endif
