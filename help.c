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
        size_t  global_help_lines = sizeof(global_help_text)/sizeof(*global_help_text),
		end_help_lines = sizeof(end_help_text)/sizeof(*end_help_text),
		mode_help_lines = 0;
	const char **mode_help_text;
	int n_lines, n_rows, i, curr_line;
	char line[HELP_TEXT_MAX_LINE];

	switch(state) {
	case DIR_MODE:
		mode_help_lines = sizeof(dir_help_text)/sizeof(*dir_help_text);
		mode_help_text = dir_help_text;
		break;
	case FILE_MODE:
		mode_help_lines = sizeof(file_help_text)/sizeof(*file_help_text);
		mode_help_text = file_help_text;
		break;
	case INFO_MODE:
		mode_help_lines = sizeof(info_help_text)/sizeof(*info_help_text);
		mode_help_text = info_help_text;
		break;
	case EDIT_MODE:
		mode_help_lines = 0;
		mode_help_text = NULL;
		break;
	case HELP_MODE:
		return;
	}
	n_lines = global_help_lines + end_help_lines + mode_help_lines
		+ (mode_help_lines > 0 ? 10 : 6);
	n_rows = n_lines > LINES-2 ? LINES-2 : n_lines;
	help_win = make_win(n_lines, HELP_TEXT_MAX_LINE + 10,
			    (LINES - n_rows) / 2, (COLS - HELP_TEXT_MAX_LINE - 10) / 2);
	box(help_win, 0, 0);
	curr_line = 1;
	for (i=0; i<global_help_lines; i++) {
		strlcpy(line, global_help_text[i], HELP_TEXT_MAX_LINE);
		mvwprintw(help_win, ++curr_line, 5, line);
	}
	if (mode_help_lines > 0) {
		curr_line += 2;
		mvwhline(help_win, curr_line, 4, ACS_HLINE, HELP_TEXT_MAX_LINE);
		curr_line++;
		for (i=0; i<mode_help_lines; i++) {
			strlcpy(line, mode_help_text[i], HELP_TEXT_MAX_LINE);
			mvwprintw(help_win, ++curr_line, 5, line);
		}
	}
	curr_line += 2;
	mvwhline(help_win, curr_line, 5, ACS_HLINE, HELP_TEXT_MAX_LINE);
	curr_line++;
	for (i=0; i<end_help_lines; i++) {
		strlcpy(line, end_help_text[i], HELP_TEXT_MAX_LINE);
		mvwprintw(help_win, ++curr_line, 5, line);
	}
	wrefresh(help_win);

	help_state = state;
	state = HELP_MODE;
}
void
hide_help()
{
        if (state != HELP_MODE) return;
	delwin(help_win);
	touchwin(stdscr);
	touchwin(dir.win);
	touchwin(file.win);
	touchwin(info.win);
	refresh();
	wrefresh(dir.win);
	wrefresh(file.win);
	wrefresh(info.win);
	help_win = NULL;
	state = help_state;
}
