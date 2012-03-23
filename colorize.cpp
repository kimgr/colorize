#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>

struct scoped_console_color
{
  scoped_console_color(int color)
    : stdout_handle(NULL)
    , previous_color(-1)
  {
    if (color != -1)
    {
      stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

      CONSOLE_SCREEN_BUFFER_INFO info = {0};
      GetConsoleScreenBufferInfo(stdout_handle, &info);
      previous_color = info.wAttributes;

      SetConsoleTextAttribute(stdout_handle, color);
    }
  }

  ~scoped_console_color()
  {
    if (stdout_handle != NULL)
      SetConsoleTextAttribute(stdout_handle, previous_color);
  }

private:
  HANDLE stdout_handle;
  int previous_color;
};

bool is_all_whitespace(const std::string& line)
{
  assert(!line.empty());

  // All lines in an SVN diff are prefixed with a space, + or -
  std::size_t length_without_prefix = line.length() - 1;

  // Empty string has no whitespace
  if (length_without_prefix == 0)
    return false;

  std::string::const_iterator begin = line.begin();
  std::size_t whitespace_count = std::count_if(++begin, line.end(), isspace);

  return (length_without_prefix == whitespace_count);
}

bool starts_with(const std::string& line, const char* prefix)
{
  const char* p = prefix;

  for (std::string::const_iterator i = line.begin(); i != line.end(); ++i, ++p)
  {
    if (*p == '\0')
      return true;

    if (*p != *i)
      return false;
  }

  return false;
}

int select_color(const std::string& line)
{
  if (line.empty())
    return -1;

  // Whitespace
  if (is_all_whitespace(line))
    return BACKGROUND_RED;

  // Diff metadata
  if (starts_with(line, "Index: "))
    return FOREGROUND_INTENSITY;

  if (starts_with(line, "======="))
    return FOREGROUND_INTENSITY;

  if (starts_with(line, "---"))
    return FOREGROUND_INTENSITY;

  if (starts_with(line, "+++"))
    return FOREGROUND_INTENSITY;

  // Change markers
  if (starts_with(line, "@@"))
    return FOREGROUND_BLUE | FOREGROUND_GREEN;

  if (starts_with(line, "-"))
    return FOREGROUND_RED;

  if (starts_with(line, "+"))
    return FOREGROUND_GREEN;

  return -1;
}

int main(int argc, char* argv[])
{
  std::string line;
  while (std::cin)
  {
    getline(std::cin, line);

    scoped_console_color recolor(select_color(line));

    std::cout << line << std::endl;
  }

  return 0;
}

