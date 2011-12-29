#include <cdk/cdk.h>
#include <stdio.h>

#define MAX_LIST_SIZE 32

int fill_content(char **content) {
    int i;
    /*unsigned memory_used = 0;*/
    char* line = malloc(32 * sizeof(char));
    for (i = 0; i<10; i++) {
        /*memory_used = CDKallocStrings(
            &content,
            "Test text",
            (unsigned) i,
            memory_used
            );*/
        sprintf(line, "Test #%d", i);
        content[i] = strdup(line);
    }

    return i;
}

void add_key_bindings(CDKALPHALIST *widget) 
{
    /*
    bindCDKObject (vALPHALIST, widget, '?', do_help, NULL);
    bindCDKObject (vALPHALIST, widget, KEY_F1, do_help, NULL);
    bindCDKObject (vALPHALIST, widget, KEY_F2, do_delete, widget);
    bindCDKObject (vALPHALIST, widget, KEY_F3, do_delete1, widget);
    bindCDKObject (vALPHALIST, widget, KEY_F4, do_reload, widget);
    bindCDKObject (vALPHALIST, widget, KEY_F5, do_undo, widget);
    */
}

CDKALPHALIST *
alpha_list_create(CDKSCREEN *screen,
                  CDK_PARAMS *param_p,
                  char *title,
                  char *label,
                  char **list_content,
                  int list_size)
{
    CDKALPHALIST *alpha_list = newCDKAlphalist (
            screen,
            CDKparamValue (param_p, 'X', CENTER),
            CDKparamValue (param_p, 'Y', CENTER),
            CDKparamValue (param_p, 'H', 0),
            CDKparamValue (param_p, 'W', 0),
            title, 
            label,
            CDKparamNumber (param_p, 'c') ? 0 : list_content,
            CDKparamNumber (param_p, 'c') ? 0 : list_size,
            '_', A_REVERSE,
            CDKparamValue (param_p, 'N', TRUE),
            CDKparamValue (param_p, 'S', FALSE)
            );

    if(alpha_list == 0) {
        destroyCDKScreen (screen);
        endCDK ();

        fprintf (stderr, "Cannot create widget\n");
        exit(1);
    }

    if(CDKparamNumber(param_p, 'c')) {
        setCDKAlphalistContents(alpha_list,
                list_content,
                list_size
        );
    }

    return alpha_list;
}

int main(int argc, char **argv) {
    WINDOW *curses_win       = 0;

    CDKALPHALIST *alpha_list = 0;
    CDKSCREEN *cdk_screen = 0;

    char **list_content   = malloc(sizeof(void*) * MAX_LIST_SIZE);
    int list_size         = 0;
    char *selected_member;

    list_size = fill_content(list_content);
    /*for(int i =0; i<list_size; i++ ) {
        printf("%s\n", list_content[i]);
    }*/

    /* I'm not sure about the necessity of this; it appears to be some kind of defaults */
    CDK_PARAMS params;
    CDKparseParams (argc, argv, &params, "c" CDK_CLI_PARAMS);

    /* Create the curses window, and the CDK wrapper */
    curses_win = initscr();
    cdk_screen = initCDKScreen(curses_win);

    initCDKColor();

    /* This is our widget. */
    alpha_list = alpha_list_create(
        cdk_screen,
        &params,
        "Hello",
        "World",
        list_content,
        list_size
    );

    add_key_bindings(alpha_list);

    selected_member = activateCDKAlphalist(alpha_list,
            0);
    

    return 0;
}
