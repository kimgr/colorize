#include "print_ansi.h"
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#include "ansi_to_win_colors-inl.h"

int get_console_color()
{
  CONSOLE_SCREEN_BUFFER_INFO info = {0};
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
  return info.wAttributes;
}

void print_ansi_line(const char* line, int default_console_color)
{
  // Call our ANSI-interpreting puts reimplemenation.
  ansi_aware_puts(line, default_console_color);
}

#else // _WIN32

// On other platforms, assume the console itself does the ANSI interpretation.
int get_console_color()
{
  return -1;
}

void print_ansi_line(const char* line, int)
{
  std::puts(line);
}

#endif // _WIN32
