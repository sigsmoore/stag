/* help.c */
/*
 * This file is public domain as declared by Sig T-M.
 */

#include "stag.h"

#define HELP_TEXT_MAX_LINE    80

WINDOW *help_win;
enum mode help_state;

const char *global_help_text[] = {
	"Keybindings available in all modes:",
	"",
	"TAB          Cycle through modes. If no entries are marked, then skip info-mode.",
	"",
	"n            Move the mark down one entry.",
	"",
	"C-n | PGDN   Move  the  mark  down by one page of entries.",
	"",
	"p | UP       Move the mark up one entry.",
	"",
	"C-p | PGUP   Move the mark up by one page of  entries.",
	"",
	"q            Exit stag.",
};
const char *end_help_text[] = {
	"                          Press q to exit help-mode.",
};
const char *dir_help_text[] = {
	"Keybindings available in directory-mode:",
	"",
        "RET        Enter the directory under the mark.  This then displays the  children",
        "           directories of the directory under the mark in the directory panel.",
        "",
        "SPC        Recursively  add  the directories and files to the active-buffer that",
        "           are children to the directory under the mark.",
        "",
        "h          Toggle whether hidden directories  are  displayed  in  the  directory",
        "           panel.",
        "",
        "o | RIGHT  Switch focus to the file panel, thus entering file-mode.",
};
const char *file_help_text[] = {
	"Keybindings available in file-mode:",
	"",
        "RET        Edit the file under the mark, thus switching to info-mode.",
        "",
        "SPC        Toggles the selection status of the file under the mark.",
        "",
        "/          Prompts  the user for an ERE and toggles the selection status of all",
	"           files that have tags that match the ERE.",
        "",
        "a          Toggle the selection status of all files in the active-buffer.",
        "",
        "c          Clear the active-buffer.",
        "",
        "d          Remove the currently selected files.",
        "",
        "e          Edit the currently selected files, thus switching to info-mode.",
        "",
        "o | LEFT   switch focus to the directory panel, thus entering directory-mode.",
        "",
        "r          Reload  the currently selected files from disk, undoing all previous",
        "           edit operations.",
        "",
        "s          Write the file under the mark to the disk.",
        "",
        "u          Convert all selected files in the active-buffer into normal files.",
        "",
        "w          Write all selected files in the active-buffer to the disk.",
};
const char *info_help_text[] = {
	"Keybindings available in info-mode:",
	"",
        "LEFT       Switch focus to the directory panel, thus entering directory-mode.",
        "",
        "RET        Edit the tag under the mark.  This consequently switches the focus to",
        "           the edit panel and enters edit-mode.",
        "",
        "o | RIGHT  Switch the focus  to the file panel, entering file-mode.",
};

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
