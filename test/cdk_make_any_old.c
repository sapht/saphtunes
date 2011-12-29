#include "cdk_make_any.h"
#include "util.h"
#define CDK_SELECTION_CHOICES ((char*[]) {"[ ]", "[*]"})
#define CDK_SELECTION_COUNT 2

#define CDK_TEST_DATA ((char*[]) {"a", "b", "c", "d"})

#define CDK_MATRIX_NUMROWS 8
#define CDK_MATRIX_NUMCOLS 5
#define MY_MAX 3
static CDKOBJS *all_objects[MY_MAX];

CDKOBJS *
cdk_make_alphalist (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    CDKALPHALIST *widget = newCDKAlphalist (cdkscreen,
                       x, y, 10, 15,
                       "AlphaList",
                       "->",
                       input_data, NUM_ELEMENTS (input_data),
                       '_', A_REVERSE,
                       TRUE, FALSE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_button (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    CDKBUTTON *widget = newCDKButton (cdkscreen, x, y,
                     "A Button!", NULL,
                     TRUE, FALSE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_buttonbox (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    CDKBUTTONBOX *widget = newCDKButtonbox (cdkscreen, x, y,
                       10, 16,
                       "ButtonBox", 6, 2,
                       input_data, NUM_ELEMENTS (input_data),
                       A_REVERSE,
                       TRUE, FALSE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_calendar (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    CDKCALENDAR *widget = newCDKCalendar (cdkscreen, x, y,
					 "Calendar", 25, 1, 2000,
					 COLOR_PAIR (16) | A_BOLD,
					 COLOR_PAIR (24) | A_BOLD,
					 COLOR_PAIR (32) | A_BOLD,
					 COLOR_PAIR (40) | A_REVERSE,
					 TRUE,
					 FALSE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_dialog (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    static char *message[] =
    {
      "This is a simple dialog box"
      ,"Is it simple enough?"
    };

    CDKDIALOG *widget = newCDKDialog (cdkscreen, x, y,
                     message, NUM_ELEMENTS (message),
                     input_data, NUM_ELEMENTS (input_data),
                     COLOR_PAIR (2) | A_REVERSE,
                     TRUE,
                     TRUE, FALSE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_dscale (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    CDKDSCALE *widget = newCDKDScale (cdkscreen, x, y,
                 "DScale", "Value", A_NORMAL,
                 15,
                 0.0, 0.0, 100.0,
                 1.0, (1.0 * 2.), 1,
                 TRUE,
                 FALSE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_entry (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    CDKENTRY *widget = newCDKEntry (cdkscreen, x, y,
                   NULL, "Entry: ", A_NORMAL,
                   '.', vMIXED, 40, 0, 256, TRUE, FALSE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_fscale (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    CDKFSCALE *widget = newCDKFScale (cdkscreen, x, y,
                     "FScale", "Value", A_NORMAL,
                     15,
                     0.0, 0.0, 100.0,
                     1.0, (1.0 * 2.), 1,
                     TRUE,
                     FALSE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_fslider (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    float low = -32;
    float high = 64;
    float inc = (float)0.1;
    CDKFSLIDER *widget = newCDKFSlider (cdkscreen,
                       x,
                       y,
                       "FSlider", "Label",
                       A_REVERSE | COLOR_PAIR (29) | ' ',
                       20,
                       low, low, high, inc, (inc * 2),
                       3,
                       TRUE,
                       FALSE);
    /* selection = activateCDKSlider (widget, 0); */
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_fselect (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    CDKFSELECT *widget = newCDKFselect (cdkscreen,
                       x,
                       y,
                       15,
                       25,
                       "FSelect", "->", A_NORMAL, '_', A_REVERSE,
                       "</5>", "</48>", "</N>", "</N>",
                       TRUE,
                       FALSE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_graph (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    static int values[] =
    {10, 15, 20, 25, 30, 35, 40, 45, 50, 55};
    static char *graphChars = "0123456789";
    CDKGRAPH *widget = newCDKGraph (cdkscreen, x, y,
                   10, 25,
                   "title", "X-axis", "Y-axis");
    setCDKGraph (widget, values, NUM_ELEMENTS (values), graphChars, TRUE, vPLOT);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_histogram (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    CDKHISTOGRAM *widget = newCDKHistogram (cdkscreen,
                       x,
                       y,
                       1,
                       20,
                       HORIZONTAL, "Histogram",
                       TRUE,
                       FALSE);
    setCDKHistogram (widget, vPERCENT, CENTER, A_BOLD, 0, 10, 6, ' ' |
            A_REVERSE, TRUE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_itemlist (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    CDKITEMLIST *widget = newCDKItemlist (cdkscreen, x, y,
                     NULL, "Month ",
                     input_data,
                     NUM_ELEMENTS (input_data),
                     1, TRUE, FALSE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_label (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    static char *message[] =
    {
      "This is a simple label."
      ,"Is it simple enough?"
    };
    CDKLABEL *widget = newCDKLabel (cdkscreen,
                   x,
                   y,
                   message, NUM_ELEMENTS (message),
                   TRUE,
                   TRUE);
    return ObjPtr (widget);
}

CDKOBJS *
cdk_make_marquee (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
    CDKMARQUEE *widget = newCDKMarquee (cdkscreen,
                       x,
                       y,
                       30,
                       TRUE,
                       TRUE);
    activateCDKMarquee (widget, "This is a message", 5, 3, TRUE);
    destroyCDKMarquee (widget);
    return 0;
}

CDKOBJS *
cdk_make_matrix (CDKSCREEN *cdkscreen, int x, int y)
{

   CDKMATRIX *widget;
   char *coltitle[CDK_MATRIX_NUMCOLS + 1];
   char *rowtitle[CDK_MATRIX_NUMROWS + 1];
   char temp[80];
   int cols = CDK_MATRIX_NUMCOLS;
   int colwidth[CDK_MATRIX_NUMCOLS + 1];
   int coltypes[CDK_MATRIX_NUMCOLS + 1];
   int maxwidth = 0;
   int n;
   int rows = CDK_MATRIX_NUMROWS;
   int vcols = 3;
   int vrows = 3;

   for (n = 0; n <= CDK_MATRIX_NUMROWS; ++n)
   {
      sprintf (temp, "row%d", n);
      rowtitle[n] = copyChar (temp);
   }
   for (n = 0; n <= CDK_MATRIX_NUMCOLS; ++n)
   {
      sprintf (temp, "col%d", n);
      coltitle[n] = copyChar (temp);
      colwidth[n] = (int)strlen (temp);
      coltypes[n] = vUCHAR;
      if (colwidth[n] > maxwidth)
	 maxwidth = colwidth[n];
   }

   widget = newCDKMatrix (cdkscreen,
			  x,
			  y,
			  rows, cols, vrows, vcols,
			  "Matrix", rowtitle, coltitle,
			  colwidth, coltypes,
			  -1, -1, '.',
			  COL, TRUE,
			  TRUE,
			  FALSE);
   return ObjPtr (widget);
}

CDKOBJS *
cdk_make_mentry (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
   CDKMENTRY *widget = newCDKMentry (cdkscreen,
				     x,
				     y,
				     "MEntry", "Label", A_BOLD, '.', vMIXED,
				     20,
				     5,
				     20,
				     0,
				     TRUE,
				     FALSE);
   return ObjPtr (widget);
}

CDKOBJS *
cdk_make_radio (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
   CDKRADIO *widget = newCDKRadio (cdkscreen,
				   x,
				   y,
				   RIGHT,
				   10,
				   20,
				   "Radio",
				   input_data, NUM_ELEMENTS (input_data),
				   '#' | A_REVERSE, 1,
				   A_REVERSE,
				   TRUE,
				   FALSE);
   return ObjPtr (widget);
}

CDKOBJS *
cdk_make_scale (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
   int low = 2;
   int high = 25;
   int inc = 1;
   CDKSCALE *widget = newCDKScale (cdkscreen,
				   x,
				   y,
				   "Scale", "Label", A_NORMAL,
				   5,
				   low, low, high,
				   inc, (inc * 2),
				   TRUE,
				   FALSE);
   return ObjPtr (widget);
}

CDKOBJS *
cdk_make_scroll (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
   CDKSCROLL *widget = newCDKScroll (cdkscreen,
				     x,
				     y,
				     RIGHT,
				     10,
				     20,
				     "Scroll",
				     input_data, NUM_ELEMENTS (input_data),
				     TRUE,
				     A_REVERSE,
				     TRUE,
				     FALSE);
   return ObjPtr (widget);
}

CDKOBJS *
cdk_make_slider (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
   int low = 2;
   int high = 25;
   int inc = 1;
   CDKSLIDER *widget = newCDKSlider (cdkscreen,
				     x,
				     y,
				     "Slider", "Label",
				     A_REVERSE | COLOR_PAIR (29) | ' ',
				     20,
				     low, low, high, inc, (inc * 2),
				     TRUE,
				     FALSE);
   /* selection = activateCDKSlider (widget, 0); */
   return ObjPtr (widget);
}

CDKOBJS *
cdk_make_selection (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
   CDKSELECTION *widget = newCDKSelection (cdkscreen, x, y,
					   NONE, 8, 20, "Selection",
					   input_data, NUM_ELEMENTS (input_data),
                       /* XXX */
					   CDK_SELECTION_CHOICES, CDK_SELECTION_COUNT,
					   A_REVERSE, TRUE, FALSE);
   return ObjPtr (widget);
}

CDKOBJS *
cdk_make_swindow (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
   CDKSWINDOW *widget = newCDKSwindow (cdkscreen,
				       x,
				       y,
				       6,
				       25,
				       "SWindow", 100,
				       TRUE,
				       FALSE);
   int n;
   for (n = 0; n < 30; ++n)
   {
      char temp[80];
      sprintf (temp, "Line %d", n);
      addCDKSwindow (widget, temp, BOTTOM);
   }
   activateCDKSwindow (widget, 0);
   return ObjPtr (widget);
}

CDKOBJS *
cdk_make_template (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
   char *Overlay = "</B/6>(___)<!6> </5>___-____";
   char *plate = "(###) ###-####";
   CDKTEMPLATE *widget = newCDKTemplate (cdkscreen,
					 x,
					 y,
					 "Template", "Label",
					 plate, Overlay,
					 TRUE,
					 FALSE);
   activateCDKTemplate (widget, 0);
   return ObjPtr (widget);
}

CDKOBJS *
cdk_make_uscale (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
   unsigned low = 0;
   unsigned high = 65535;
   unsigned inc = 1;
   CDKUSCALE *widget = newCDKUScale (cdkscreen,
				     x,
				     y,
				     "UScale", "Label", A_NORMAL,
				     5,
				     low, low, high,
				     inc, (inc * 32),
				     TRUE,
				     FALSE);
   return ObjPtr (widget);
}

CDKOBJS *
cdk_make_uslider (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
   unsigned low = 0;
   unsigned high = 65535;
   unsigned inc = 1;
   CDKUSLIDER *widget = newCDKUSlider (cdkscreen,
				       x,
				       y,
				       "USlider", "Label",
				       A_REVERSE | COLOR_PAIR (29) | ' ',
				       20,
				       low, low, high, inc, (inc * 32),
				       TRUE,
				       FALSE);
   /* selection = activateCDKSlider (widget, 0); */
   return ObjPtr (widget);
}

CDKOBJS *
cdk_make_viewer (CDKSCREEN *cdkscreen, char **input_data, int x, int y)
{
   static char *button[1] =
   {"Ok"};
   CDKVIEWER *widget = newCDKViewer (cdkscreen,
				     x,
				     y,
				     10,
				     20,
				     button, 1, A_REVERSE,
				     TRUE,
				     FALSE);
   setCDKViewer (widget, "Viewer",
		 input_data, NUM_ELEMENTS (input_data),
		 A_REVERSE, FALSE, TRUE, TRUE);
   activateCDKViewer (widget, 0);
   return ObjPtr (widget);
}


const struct
{
   char *name;
   EObjectType type;
}
cdk_menu_table[] =
{
   { "(CDKGRAPH)",	vGRAPH },	/* no traversal (not active) */
   { "(CDKHISTOGRAM)",	vHISTOGRAM },	/* no traversal (not active) */
   { "(CDKLABEL)",	vLABEL },	/* no traversal (not active) */
   { "(CDKMARQUEE)",	vMARQUEE },	/* hangs (leaves trash) */
   { "*CDKVIEWER",	vVIEWER },	/* traversal out-only on OK */
   { "CDKALPHALIST",	vALPHALIST },
   { "CDKBUTTON",	vBUTTON },
   { "CDKBUTTONBOX",	vBUTTONBOX },
   { "CDKCALENDAR",	vCALENDAR },
   { "CDKDIALOG",	vDIALOG },
   { "CDKDSCALE",	vDSCALE },
   { "CDKENTRY",	vENTRY },
   { "CDKFSCALE",	vFSCALE },
   { "CDKFSELECT",	vFSELECT },
   { "CDKFSLIDER",	vFSLIDER },
   { "CDKITEMLIST",	vITEMLIST },
   { "CDKMATRIX",	vMATRIX },
   { "CDKMENTRY",	vMENTRY },
   { "CDKRADIO",	vRADIO },
   { "CDKSCALE",	vSCALE },
   { "CDKSCROLL",	vSCROLL },
   { "CDKSELECTION",	vSELECTION },
   { "CDKSLIDER",	vSLIDER },
   { "CDKSWINDOW",	vSWINDOW },
   { "CDKTEMPLATE",	vTEMPLATE },
   { "CDKUSCALE",	vUSCALE },
   { "CDKUSLIDER",	vUSLIDER },
};

void cdk_rebind_esc (CDKOBJS *obj)
{
   bindCDKObject (ObjTypeOf (obj), obj, KEY_F (1), getcCDKBind, (void *)KEY_ESC);
}

int cdk_menu_selection_prehandler (EObjectType cdktype GCC_UNUSED, void *object,
		       void *clientData GCC_UNUSED,
		       chtype input GCC_UNUSED)
{
   int mp, sp;
   CDKSCREEN *screen;
   WINDOW *window;

   switch (input)
   {
   case KEY_ENTER:
      getCDKMenuCurrentItem ((CDKMENU *)object, &mp, &sp);

      screen = ScreenOf ((CDKMENU *)object);
      window = screen->window;
      mvwprintw (window, getmaxy (window) - 1, 0, "selection %d/%d", mp, sp);
      clrtoeol ();
      refresh ();
      if (sp >= 0 && sp < (int)NUM_ELEMENTS (cdk_menu_table))
	 cdk_make_type (screen, mp, cdk_menu_table[sp].type);
      break;
   }
   return 1;
}

static int oldmain (int argc GCC_UNUSED, char **argv GCC_UNUSED) {

    char *yes_no[] =
    {
       "Yes", "NO"
    };
    char *months[] =
    {
       "Jan", "Feb", "Mar", "Apr", "May", "Jun",
       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

   unsigned j, k;

   /* Declare local variables. */
   CDKSCREEN *cdkscreen = NULL;
   CDKMENU *menu = NULL;
   WINDOW *cursesWin = NULL;
   char *mesg[3];
   static char *menulist[MAX_MENU_ITEMS][MAX_SUB_ITEMS] =
   {
      {"Left"},
      {"Center"},
      {"Right"},
   };
   static int submenusize[] =
   {
      NUM_ELEMENTS (menu_table) + 1,
      NUM_ELEMENTS (menu_table) + 1,
      NUM_ELEMENTS (menu_table) + 1
   };
   static int menuloc[] =
   {LEFT, LEFT, RIGHT};

   for (j = 0; j < MY_MAX; ++j)
   {
      for (k = 0; k < NUM_ELEMENTS (menu_table); ++k)
      {
	 menulist[j][k + 1] = menu_table[k].name;
      }
   }

   /* Set up CDK. */
   cursesWin = initscr ();
   cdkscreen = initCDKScreen (cursesWin);

   /* Start CDK colors. */
   initCDKColor ();


   menu = newCDKMenu (cdkscreen, menulist, MY_MAX, submenusize, menuloc,
		      TOP, A_UNDERLINE, A_REVERSE);
   if (menu == 0)
   {
      destroyCDKScreen (cdkscreen);
      endCDK ();

      printf ("? Cannot create menus\n");
      ExitProgram (EXIT_FAILURE);
   }
   cdk_rebind_esc (ObjOf (menu));

   setCDKMenuPreProcess (menu, cdk_menu_selection_prehandler, 0);

   /* setup the initial display */
   cdk_make_type (cdkscreen, 0, vENTRY);
#if MY_MAX > 1
   cdk_make_type (cdkscreen, 1, vITEMLIST);
#if MY_MAX > 2
   cdk_make_type (cdkscreen, 2, vSELECTION);
#endif
#endif

   /* Draw the screen. */
   refreshCDKScreen (cdkscreen);

   /* Traverse the screen */
   traverseCDKScreen (cdkscreen);

   mesg[0] = "Done";
   mesg[1] = "";
   mesg[2] = "<C>Press any key to continue.";
   popupLabel (cdkscreen, mesg, 3);

   /* Clean up and exit. */
   for (j = 0; j < MY_MAX; ++j)
   {
      if (all_objects[j] != 0)
	 _destroyCDKObject (all_objects[j]);
   }
   destroyCDKMenu (menu);
   destroyCDKScreen (cdkscreen);
   endCDK ();

   ExitProgram (EXIT_SUCCESS);
}


void 
cdk_make_type (CDKSCREEN *cdkscreen, int menu, EObjectType type)
{
    CDKOBJS *(*func) (CDKSCREEN *, int, int) = 0;
    CDKOBJS *prior = 0;
    int x;
    int y;

    /* setup positions, staggered a little */
    switch (menu)
    {
    case 0:
      x = LEFT;
      y = 2;
      break;
    case 1:
      x = CENTER;
      y = 4;
      break;
    case 2:
      x = RIGHT;
      y = 6;
      break;
    default:
      beep ();
      return;
    }

    /* find the function to make a widget of the given type */
    switch (type)
    {
    case vALPHALIST:
      func = make_alphalist;
      break;
    case vBUTTON:
      func = make_button;
      break;
    case vBUTTONBOX:
      func = make_buttonbox;
      break;
    case vCALENDAR:
      func = make_calendar;
      break;
    case vDIALOG:
      func = make_dialog;
      break;
    case vDSCALE:
      func = make_dscale;
      break;
    case vENTRY:
      func = make_entry;
      break;
    case vFSCALE:
      func = make_fscale;
      break;
    case vFSELECT:
      func = make_fselect;
      break;
    case vFSLIDER:
      func = make_fslider;
      break;
    case vGRAPH:
      func = make_graph;
      break;
    case vHISTOGRAM:
      func = make_histogram;
      break;
    case vITEMLIST:
      func = make_itemlist;
      break;
    case vLABEL:
      func = make_label;
      break;
    case vMARQUEE:
      func = make_marquee;
      break;
    case vMATRIX:
      func = make_matrix;
      break;
    case vMENTRY:
      func = make_mentry;
      break;
    case vRADIO:
      func = make_radio;
      break;
    case vSCALE:
      func = make_scale;
      break;
    case vSCROLL:
      func = make_scroll;
      break;
    case vSELECTION:
      func = make_selection;
      break;
    case vSLIDER:
      func = make_slider;
      break;
    case vSWINDOW:
      func = make_swindow;
      break;
    case vTEMPLATE:
      func = make_template;
      break;
    case vUSCALE:
      func = make_uscale;
      break;
    case vUSLIDER:
      func = make_uslider;
      break;
    case vVIEWER:
      func = make_viewer;
      break;
    case vMENU:
    case vTRAVERSE:
    case vNULL:
      if (type > 0)
     beep ();
      return;
    }

    /* erase the old widget */
    if ((prior = all_objects[menu]) != 0)
    {
      EraseObj (prior);
      _destroyCDKObject (prior);
      all_objects[menu] = 0;
    }

    /* create the new widget */
    if (func != 0)
    {
      CDKOBJS *widget = func (cdkscreen, CDK_TEST_DATA x, y);
      if (widget != 0)
      {
     all_objects[menu] = widget;
     rebind_esc (widget);
      }
      else
     flash ();
    }
    else
    {
      beep ();
    }
}
