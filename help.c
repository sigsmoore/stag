/* help.c */
/*
 * This file is public domain as declared by Sig T-M.
 */

#include "stag.h"

#define HELP_TEXT_MAX_LINE    80

WINDOW *help_win;
enum mode help_state;

void
show_help()
{
        if (state == HELP_MODE) return;
       help_state = state;
       state = HELP_MODE;
       help = make_win(3, 30, (LINES - 3) / 2, (COLS - 30) / 2);
       box(help, 0, 0);
       mvwprintw(help, 1, 2, "Press q to exit help mode.");
       wrefresh(help);
}
void
hide_help()
{
        if (state != HELP_MODE) return;
       delwin(help);
       touchwin(stdscr);
       touchwin(dir.win);
       touchwin(file.win);
       refresh();
       wrefresh(dir.win);
       wrefresh(file.win);
       help = NULL;
       state = help_state;
}
