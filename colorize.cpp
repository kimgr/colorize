#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>

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
  // all lines in svn diffs are indented with a single leading space,
  // so a line with only one space is really empty.
  if (line == " ")
    return false;

  size_t whitespace_count = std::count_if(line.begin(), line.end(), isspace);
  return (line.length() == whitespace_count);
}

int select_color(const std::string& line)
{
  if (line.empty())
    return -1;

  // Whitespace
  if (is_all_whitespace(line))
    return BACKGROUND_RED;

  // Diff metadata
  if (line.substr(0, 7) == "Index: ")
    return FOREGROUND_INTENSITY;

  if (line.substr(0, 7) == "=======")
    return FOREGROUND_INTENSITY;

  if (line.substr(0, 3) == "---")
    return FOREGROUND_INTENSITY;

  if (line.substr(0, 3) == "+++")
    return FOREGROUND_INTENSITY;

  // Change markers
  if (line.substr(0, 2) == "@@")
    return FOREGROUND_BLUE | FOREGROUND_GREEN;

  if (line.substr(0, 1) == "-")
    return FOREGROUND_RED;

  if (line.substr(0, 1) == "+")
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

