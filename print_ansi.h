#ifndef INCLUDED_TRANSLATE_ANSI_H
#define INCLUDED_TRANSLATE_ANSI_H

/// \brief Return current console attributes on Windows.
/// On other platforms, returns -1.
int get_console_color();

/// \brief Print a line and interpret ANSI escape sequences to
/// change Windows console attributes accordingly.
/// For non-Windows platforms, assume the terminal can handle
/// ANSI escape sequences.
void print_ansi_line(const char* line, int default_console_color);

#endif // INCLUDED_TRANSLATE_ANSI_H
